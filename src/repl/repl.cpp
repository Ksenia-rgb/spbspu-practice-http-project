#include "repl.hpp"

namespace http::repl
{
  std::unique_ptr< cli::Menu > init(cli::LoopScheduler& scheduler, AppState& state)
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
        scheduler.Stop();
        state.req_name = name;
        req::inputFromFile(state.request, path);
        state.cli_state = CliState::REQ_FILE;
      });

    return menu;
  }
}
