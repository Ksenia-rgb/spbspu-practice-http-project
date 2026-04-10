#ifndef REPL_HPP
#define REPL_HPP
#include <memory>
#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>
#include "request.hpp"

namespace http
{
  namespace repl
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
      auto rootMenu = std::make_unique< cli::Menu >("http-cli");
      rootMenu->Insert("sum", static_cast<std::function<void(std::ostream&, int, int)>>(sum));
      rootMenu->Insert("sub", static_cast<std::function<void(std::ostream&, int, int)>>(sub));
      rootMenu->Insert("req", static_cast<std::function<void(std::ostream&)>>(req));
      return rootMenu;
    }
  }
}

#endif
