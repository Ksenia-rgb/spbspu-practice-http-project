#include <memory>
#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>
#include "commands/commands.hpp"

int main()
{
  std::unique_ptr< cli::Menu > rootMenu = commands::init();
  cli::Cli app(std::move(rootMenu));
  cli::LoopScheduler scheduler;
  app.ExitAction([&scheduler](std::ostream& out)
  {
    scheduler.Stop();
  });

  cli::CliLocalTerminalSession session(app, scheduler, std::cout);

  scheduler.Run();
}
