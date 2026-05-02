#include "repl.hpp"
#include <cstdio>

std::unique_ptr< cli::Menu > http::repl::init(cli::LoopScheduler& scheduler, state::AppState& state)
{
  auto menu = std::make_unique< cli::Menu >("http-cli");
  menu->Insert("req",
    [&scheduler, &state](std::ostream&)
    {
      scheduler.Stop();
      state.cli_state = state::CliState::REQ_INPUT;
    });
  menu->Insert("file",
    [&scheduler, &state](std::ostream&, const std::string& name, const std::string& path)
    {
      state.req_name = name;
      req::inputFromFile(state.request, path);
      scheduler.Stop();
      state.cli_state = state::CliState::REQ_FILE;
    });
  menu->Insert("edit",
    [&scheduler, &state](std::ostream&, const std::string& name)
    {
      state.req_name = name;
      std::string path = "/tmp/http_request.txt";
      req::createTemplateFile(path);
      req::openTemplateFile(path);
      req::inputFromFile(state.request, path);
      std::remove(path.c_str());
      scheduler.Stop();
      state.cli_state = state::CliState::REQ_FILE;
    });
  menu->Insert("session",
    [](std::ostream& out)
    {
      out << "Unknown\n";
    });
  menu->Insert("session-list",
    [](std::ostream& out)
    {
      out << "Unknown\nBanking\nShop\n";
    });
  menu->Insert("session-list",
    [](std::ostream&, const std::string& name)
    {
      // renameSession(name)
    });
  menu->Insert("session-rm",
    [](std::ostream&, const std::string& name)
    {
      // rmSession(name)
    });
  menu->Insert("session-switch",
    [](std::ostream& out, const std::string& name)
    {
      // switchSession(name)
      out << "switch to " << name << "\n";
    });
  menu->Insert("history",
    [](std::ostream& out, int limit)
    {
      // getHistory(limit)
      out << "here will be history\n";
    });
  menu->Insert("history",
    [](std::ostream& out, const std::string& name)
    {
      // getHistory(name)
      out << "here will be history\n";
    });
  menu->Insert("mark",
    [](std::ostream& out, const std::string& req_name, const std::string& mark)
    {
      // mark(req_name, mark)
      out << "here will be mark\n";
    });
  menu->Insert("mark",
    [](std::ostream& out, const std::string& req_name, const std::string& comment)
    {
      // comment(req_name, comment)
      out << "here will be mark\n";
    });
  return menu;
}

bool http::repl::exitConfirmation(std::istream& in, std::ostream& out)
{
  out << "Request did not execute, are you sure, you want to exit? (Yes (y) or No (n))\n> ";
  std::string answer;
  in >> answer;
  in.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
  for (size_t i = 0; i < answer.size(); ++i)
  {
    answer[i] = static_cast< char >(std::toupper(static_cast< unsigned char >(answer[i])));
  }
  if (answer != "YES" && answer != "Y")
  {
    return false;
  }
  return true;
}
