#include <memory>
#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>

namespace commands
{
  inline void sum(std::ostream& out, int a, int b)
  {
    out << (a + b) << '\n';
  }

  inline void sub(std::ostream& out, int a, int b)
  {
    out << (a - b) << '\n';
  }

  inline std::unique_ptr< cli::Menu > init()
  {
    auto rootMenu = std::make_unique< cli::Menu >("mycli");
    rootMenu->Insert("sum", sum);
    rootMenu->Insert("sub", sub);
    return rootMenu;
  }
}
