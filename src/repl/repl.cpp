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
