#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>
#include <memory>
#include "repl/app_state.hpp"
#include "repl/repl.hpp"

int main()
{
  http::repl::state::AppState state;

  while (true)
  {
    cli::LoopScheduler scheduler;
    if (state.cli_state == http::repl::state::CliState::REQ_MENU)
    {
      std::unique_ptr< cli::Menu > reqMenu =
        http::repl::req::reqInit(state.req_name, state.request, state.response);
      cli::Cli app(std::move(reqMenu));
      app.ExitAction(
        [&scheduler, &state](std::ostream&)
        {
          state.cli_state = http::repl::state::CliState::EXIT_CONFIRMATION;
          scheduler.Stop();
        });
      cli::CliLocalTerminalSession session(app, scheduler, std::cout);
      scheduler.Run();
    }
    else
    {
      std::unique_ptr< cli::Menu > rootMenu = http::repl::init(scheduler, state);
      cli::Cli app(std::move(rootMenu));
      app.ExitAction(
        [&scheduler, &state](std::ostream&)
        {
          scheduler.Stop();
          state.cli_state = http::repl::state::CliState::EXIT;
        });
      cli::CliLocalTerminalSession session(app, scheduler, std::cout);
      scheduler.Run();
    }

    if (state.cli_state == http::repl::state::CliState::REQ_INPUT)
    {
      std::cout << "\n";
      http::repl::req::reqInput(state.req_name, state.request, std::cin, std::cout);
      state.cli_state = http::repl::state::CliState::REQ_MENU;
    }
    else if (state.cli_state == http::repl::state::CliState::EXIT)
    {
      break;
    }
    else if (state.cli_state == http::repl::state::CliState::REQ_FILE)
    {
      std::cout << "\n";
      state.cli_state = http::repl::state::CliState::REQ_MENU;
    }
    else if (state.cli_state == http::repl::state::CliState::EXIT_CONFIRMATION)
    {
      if (state.response.status == -1 && !http::repl::exitConfirmation(std::cin, std::cout))
      {
        state.cli_state = http::repl::state::CliState::REQ_MENU;
      }
      else
      {
        http::repl::state::clear(state);
      }
    }
  }
}
