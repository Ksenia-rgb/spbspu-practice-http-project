#include "repl.hpp"

namespace http
{
  namespace repl
  {
    std::unique_ptr< cli::Menu > init()
    {
      auto rootMenu = std::make_unique< cli::Menu >("http-cli");
      rootMenu->Insert("req", static_cast< std::function< void(std::ostream&) > >(req::req));
      return rootMenu;
    }
  }
}
