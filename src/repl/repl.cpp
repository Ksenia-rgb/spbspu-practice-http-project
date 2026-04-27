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

    return menu;
  }
}
