#include "session.hpp"
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>
#include <boost/filesystem.hpp>
#include "json.hh"
#include "models.hpp"

http::session::Session::Session(const Session& rhs, const std::string& name, const std::string& path):
  name_(name),
  history_(rhs.history_),
  dataPath_(path)
{
  if (name == "Unknown")
  {
    throw std::logic_error("The name can't be Unknown");
  }
  save();
}

http::session::Session::Session(const std::string& name, const std::string& path):
  name_(name),
  dataPath_(path)
{
  if (name == "Unknown")
  {
    throw std::logic_error("The name can't be Unknown");
  }
  read();
  save();
}

std::string http::session::Session::getName() const noexcept
{
  return name_;
}

void http::session::Session::setName(const std::string& name)
{
  if (name == "Unknown")
  {
    throw std::logic_error("The name can't be Unknown");
  }
  try
  {
    boost::filesystem::rename(dataPath_ + name_ + ".json", dataPath_ + name + ".json");
  }
  catch (boost::filesystem::filesystem_error)
  {
    std::ofstream out(dataPath_ + name + ".json");
    out << history_.dump(2);
  }
  name_ = name;
}

void http::session::Session::removeSession()
{
  boost::filesystem::remove(dataPath_ + name_ + ".json");
  name_ = "Unknown";
  history_.clear();
}

void http::session::Session::switchSession(const std::string& name)
{
  if (name_ != "Unknown")
  {
    save();
  }
  name_ = name;
  read();
}

void http::session::Session::save()
{
  if (name_ != "Unknown")
  {
    std::ofstream out(dataPath_ + name_ + ".json");
    out << history_.dump(2);
  }
}

void http::session::Session::read()
{
  history_.clear();
  if (name_ != "Unknown")
  {
    std::ifstream in(dataPath_ + name_ + ".json");
    if (in.is_open())
    {
      in >> history_;
    }
  }
}

ordered_json http::session::Session::getHistory(size_t limit) const
{
  ordered_json res;
  if (limit >= history_.size())
  {
    limit = history_.size();
  }
  for (size_t i = history_.size() - limit; i != history_.size(); ++i)
  {
    res.push_back(history_[i]);
  }
  return res;
}

ordered_json http::session::Session::getHistoryByName(const std::string& reqName) const
{
  ordered_json res;
  for (size_t i = history_.size(); i > 0; --i)
  {
    ordered_json curr(history_[i - 1]);
    if (curr["name"] == reqName)
    {
      res.push_back(history_[i - 1]);
    }
  }
  return res;
}

ordered_json http::session::Session::getHistoryByMark(const std::string& markName, size_t limit) const
{
  ordered_json res;
  for (size_t i = history_.size(); limit && i; --i)
  {
    ordered_json curr(history_[i - 1]);
    if (curr["mark"] == markName)
    {
      res.push_back(history_[i - 1]);
      --limit;
    }
  }
  return res;
}

void http::session::Session::setMark(const std::string& markName)
{
  history_.back()["mark"] = markName;
  save();
}

void http::session::Session::setMark(const std::string& reqName, const std::string& markName)
{
  for (size_t i = 0; i < history_.size(); ++i)
  {
    if (history_[i]["name"] == reqName)
    {
      history_[i]["mark"] = markName;
      save();
      return;
    }
  }
  throw std::logic_error("Request not found");
}

void http::session::Session::setComment(const std::string& comment)
{
  history_.back()["comment"] = comment;
  save();
}

void http::session::Session::setComment(const std::string& reqName, const std::string& comment)
{
  for (size_t i = 0; i < history_.size(); ++i)
  {
    if (history_[i]["name"] == reqName)
    {
      history_[i]["comment"] = comment;
      save();
      return;
    }
  }
  throw std::logic_error("Request not found");
}

void http::session::Session::addRequest(
    const std::string& name, const http::models::Request& request, const http::models::Response& response)
{
  ordered_json j = createRequest(name, "", "", request, response);
  history_.push_back(j);
  if (history_.size() > 1000)
  {
    history_.erase(history_.begin());
  }
  save();
}

void http::session::Session::changeRequest(
    const std::string& name, const http::models::Request& request, const http::models::Response& response)
{
  for (size_t i = 0; i < history_.size(); ++i)
  {
    if (history_[i]["name"] == name)
    {
      history_[i] = createRequest(name, history_[i]["mark"], history_[i]["comment"], request, response);
      save();
      return;
    }
  }
  throw std::logic_error("Request not found");
}

ordered_json http::session::Session::createRequest(const std::string& name, const std::string& mark,
    const std::string& comment, const http::models::Request& request, const http::models::Response& response)
{
  ordered_json j;
  j["name"] = name;
  j["mark"] = mark;
  j["comment"] = comment;

  ordered_json req;
  req["method"] = request.method;
  req["host"] = request.host;
  req["path"] = request.path;
  req["headers"] = {};
  for (const auto& pair : request.headers)
  {
    req["headers"].push_back(pair.first + ": " + pair.second);
  }
  req["body"] = request.body;
  j["request"] = req;

  ordered_json res;
  res["status"] = response.status;
  res["headers"] = {};
  for (const auto& pair : response.headers)
  {
    res["headers"].push_back(pair.first + ": " + pair.second);
  }
  try
  {
    res["body"] = ordered_json::parse(response.body);
  }
  catch (const ordered_json::parse_error& e)
  {
    res["body"] = response.body;
  }
  j["response"] = res;

  return j;
}

std::pair< http::models::Request, http::models::Response > http::session::Session::getRequest(const std::string& name)
{
  for (size_t i = 0; i < history_.size(); ++i)
  {
    if (history_[i]["name"] == name)
    {
      http::models::Request req;
      req.method = history_[i]["request"]["method"];
      req.host = history_[i]["request"]["host"];
      req.path = history_[i]["request"]["path"];
      for (const std::string h : history_[i]["request"]["headers"])
      {
        size_t pos = h.find(": ");
        req.headers.insert({h.substr(0, pos), h.substr(pos + 2)});
      }
      req.body = history_[i]["request"]["body"];

      http::models::Response res;
      res.status = history_[i]["response"]["status"];
      for (const std::string h : history_[i]["response"]["headers"])
      {
        size_t pos = h.find(": ");
        res.headers.insert({h.substr(0, pos), h.substr(pos + 2)});
      }
      res.body = history_[i]["response"]["body"].dump();

      return {req, res};
    }
  }

  throw std::logic_error("Request not found");
}

std::vector< std::string > sessionList(const std::string& path)
{
  std::vector< std::string > sessions;
  boost::filesystem::path dir(path);
  for (boost::filesystem::directory_iterator it(dir), end; it != end; ++it)
  {
    sessions.push_back(it->path().stem().string());
  }
  return sessions;
}

std::vector< std::string > http::session::sessionList()
{
  return sessionList("data/");
}
