#include "repl.hpp"
#include <cstdio>
#include <help.hpp>

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
    "req",
    [&scheduler, &state](std::ostream&, const std::string& name)
    {
      scheduler.Stop();
      state.req_name = name;
      try
      {
        state.request = state.session.getRequest(name).first;
        state.response = state.session.getRequest(name).second;
        state.cli_state = state::CliState::REQ_FILE;
      }
      catch (...)
      {
        state.cli_state = state::CliState::REQ_INPUT;
      }
    },
    "Start interactive request menu");
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
      try
      {
        req::createTemplateFile(path, state.session.getRequest(name).first);
        state.response = state.session.getRequest(name).second;
      }
      catch (...)
      {
        req::createTemplateFile(path);
      }

      req::openTemplateFile(path);
      req::inputFromFile(state.request, path);
      std::remove(path.c_str());
      scheduler.Stop();
      state.cli_state = state::CliState::REQ_FILE;
    },
    "Edit a request in text editor (GNU nano by default)");
  menu->Insert(
    "edit",
    [&scheduler, &state](std::ostream&, const std::string& new_name, const std::string& old_name)
    {
      state.req_name = new_name;
      std::string path = "/tmp/http_request.txt";
      req::createTemplateFile(path, state.session.getRequest(old_name).first);
      req::openTemplateFile(path);
      req::inputFromFile(state.request, path);
      std::remove(path.c_str());
      scheduler.Stop();
      state.cli_state = state::CliState::REQ_FILE;
    },
    "Edit a request in text editor (GNU nano by default)");
  menu->Insert(
    "session",
    [&state](std::ostream& out)
    {
      out << state.session.getName() << "\n";
    },
    "Show current session name ('Unknown' if anonymous)");
  menu->Insert(
    "session-list",
    [&state](std::ostream& out)
    {
      std::vector< std::string > session_list = session::sessionList();
      for (size_t i = 0; i < session_list.size(); ++i)
      {
        out << session_list[i] << "\n";
      }
    },
    "List all saved sessions");
  menu->Insert(
    "session-name",
    [&state](std::ostream&, const std::string& name)
    {
      state.session.setName(name);
    },
    "Name (or rename) the current session");
  menu->Insert(
    "session-rm",
    [&state](std::ostream&)
    {
      state.session.removeSession();
    },
    "Delete the current session");
  menu->Insert(
    "session-switch",
    [&state](std::ostream&, const std::string& name)
    {
      state.session.switchSession(name);
    },
    "Switch to another session (creates if not exists).\n        Use 'Unknown' to switch to a "
    "new "
    "anonymous session.");
  menu->Insert(
    "history",
    [&state](std::ostream& out, size_t limit)
    {
      out << std::setw(2) << state.session.getHistory(limit) << "\n";
    },
    "Show N last requests");
  menu->Insert(
    "history",
    [&state](std::ostream& out, const std::string& mark, size_t limit)
    {
      out << std::setw(2) << state.session.getHistoryByMark(mark, limit) << "\n";
    },
    "Show N last requests by mark");
  menu->Insert(
    "history",
    [&state](std::ostream& out, const std::string& name)
    {
      out << std::setw(2) << state.session.getHistoryByName(name) << "\n";
    },
    "Show request from history by name");
  menu->Insert(
    "mark",
    [&state](std::ostream&, const std::string& mark)
    {
      state.session.setMark(mark);
    },
    "Mark last request in history");
  menu->Insert(
    "mark",
    [&state](std::ostream&, const std::string& req_name, const std::string& mark)
    {
      state.session.setMark(req_name, mark);
    },
    "Mark request in history");
  menu->Insert(
    "comment",
    [&state](std::ostream&, const std::string& comment)
    {
      state.session.setComment(comment);
    },
    "Comment last request in history");
  menu->Insert(
    "comment",
    [&state](std::ostream&, const std::string& req_name, const std::string& comment)
    {
      state.session.setComment(req_name, comment);
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
