#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <ostream>
#include <memory>
#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>
#include "../common/models.hpp"

namespace http
{
  namespace repl
  {
    void req(std::ostream& out);
    void show(std::ostream& out, const std::string& name, const models::Request& request);
    inline std::unique_ptr< cli::Menu > reqInit(std::string& name, models::Request& request)
    {
      auto reqMenu = std::make_unique< cli::Menu >("req command");
      reqMenu->Insert("show", [&name, &request](std::ostream& out)
      {
        show(out, name, request);
      });
      return reqMenu;
    }
  }
}

#endif
