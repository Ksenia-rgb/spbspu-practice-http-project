#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>
#include <memory>
#include "repl/repl.hpp"

int main()
{
  std::unique_ptr< cli::Menu > rootMenu = http::repl::init();
  cli::Cli app(std::move(rootMenu));
  cli::LoopScheduler scheduler;
  app.ExitAction(
    [&scheduler](std::ostream&)
    {
      scheduler.Stop();
    });

  cli::CliLocalTerminalSession session(app, scheduler, std::cout);

  scheduler.Run();
}
