#include "session.hpp"
#include <stdexcept>
#include <boost/filesystem.hpp>
#include <boost/filesystem/operations.hpp>

namespace
{
  const std::string DATA_PATH = "data";
}

http::session::Session::Session(const Session& rhs, const std::string& name)
{
  if (name == "Unknown")
  {
    throw std::logic_error("The name can't be Unknown");
  }
  name_ = name;
  history_ = rhs.history_;
  save();
}

http::session::Session::Session(std::string name)
{
  if (name == "Unknown")
  {
    throw std::logic_error("The name can't be Unknown");
  }
  name_ = name;
  read();
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
  boost::filesystem::rename(DATA_PATH + name_ + ".json", DATA_PATH + name + ".json");
  name_ = name;
}

void http::session::Session::removeSession()
{
  boost::filesystem::remove(DATA_PATH + name_ + ".json");
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
