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
    inline std::unique_ptr< cli::Menu > init()
    {
      auto rootMenu = std::make_unique< cli::Menu >("http-cli");
      rootMenu->Insert("req", static_cast<std::function<void(std::ostream&)>>(req::req));
      return rootMenu;
    }
  }
}

#endif
