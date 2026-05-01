#include "repl.hpp"
#include <cstdio>

std::unique_ptr< cli::Menu > http::repl::init(cli::LoopScheduler& scheduler, AppState& state)
{
  auto menu = std::make_unique< cli::Menu >("http-cli");
  menu->Insert("req",
    [&scheduler, &state](std::ostream&)
    {
      scheduler.Stop();
      state.cli_state = CliState::REQ_INPUT;
    });
  menu->Insert("file",
    [&scheduler, &state](std::ostream&, const std::string& name, const std::string& path)
    {
      state.req_name = name;
      req::inputFromFile(state.request, path);
      scheduler.Stop();
      state.cli_state = CliState::REQ_FILE;
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
      state.cli_state = CliState::REQ_FILE;
    });

  return menu;
}
