#include "repl.hpp"
#include <cstdio>
#include "help.hpp"

std::unique_ptr< cli::Menu > http::repl::init(cli::LoopScheduler& scheduler, state::AppState& state)
{
  auto menu = std::make_unique< cli::Menu >("http-cli");
  menu->Insert(
    "req",
    [&scheduler, &state](std::ostream&)
    {
      scheduler.Stop();
      state.cli_state = state::CliState::REQ_INPUT;
    },
    "Start interactive request builder");
  menu->Insert(
    "file",
    [&scheduler, &state](std::ostream&, const std::string& name, const std::string& path)
    {
      state.req_name = name;
      req::inputFromFile(state.request, path);
      scheduler.Stop();
      state.cli_state = state::CliState::REQ_FILE;
    },
    "Load HTTP request from a file (raw HTTP format).");
  menu->Insert(
    "edit",
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
    },
    "Edit a request in text editor (GNU nano by default)");
  menu->Insert(
    "session",
    [](std::ostream& out)
    {
      out << "Unknown\n";
    },
    "Show current session name ('Unknown' if anonymous)");
  menu->Insert(
    "session-list",
    [](std::ostream& out)
    {
      out << "Unknown\nBanking\nShop\n";
    },
    "List all saved sessions");
  menu->Insert(
    "session-name",
    [](std::ostream&, const std::string& name)
    {
      // renameSession(name)
    },
    "Name (or rename) the current session");
  menu->Insert(
    "session-rm",
    [](std::ostream&, const std::string& name)
    {
      // rmSession(name)
    },
    "Delete the specified saved session");
  menu->Insert(
    "session-switch",
    [](std::ostream& out, const std::string& name)
    {
      // switchSession(name)
      out << "switch to " << name << "\n";
    },
    "Switch to another session (creates if not exists).\n        Use 'Unknown' to switch to a new "
    "anonymous session.");
  menu->Insert(
    "history",
    [](std::ostream& out, int limit)
    {
      // getHistory(limit)
      out << "here will be history\n";
    },
    "Show N last requests");
  menu->Insert(
    "history",
    [](std::ostream& out, const std::string& mark, int limit)
    {
      // getHistory(limit)
      out << "here will be history\n";
    },
    "Show N last requests by mark");
  menu->Insert(
    "history",
    [](std::ostream& out, const std::string& mark)
    {
      // getHistory(limit)
      out << "here will be history\n";
    },
    "Show 10 last requests by mark");
  menu->Insert(
    "history",
    [](std::ostream& out, const std::string& name)
    {
      // getHistory(name)
      out << "here will be history\n";
    },
    "Show request from history by name");
  menu->Insert(
    "mark",
    [](std::ostream& out, const std::string& req_name, const std::string& mark)
    {
      // mark(req_name, mark)
      out << "here will be mark\n";
    },
    "Mark request in history");
  menu->Insert(
    "comment",
    [](std::ostream& out, const std::string& req_name, const std::string& comment)
    {
      // comment(req_name, comment)
      out << "here will be mark\n";
    },
    "Comment request in history");
  menu->Insert(
    "helpDetails",
    [](std::ostream& out)
    {
      out << help::MAIN_HELP_TEXT;
    },
    "Show full help");
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
