#ifndef APP_STATE_HPP
#define APP_STATE_HPP
#include <models.hpp>

namespace http
{
  namespace repl
  {
    namespace state
    {
      enum class CliState
      {
        NORMAL,
        REQ_INPUT,
        REQ_FILE,
        REQ_MENU,
        EXIT_CONFIRMATION,
        EXIT
      };

      struct AppState
      {
        CliState cli_state = CliState::NORMAL;
        std::string req_name;
        models::Request request;
        models::Response response;
      };

      void clear(AppState& state);
    }
  }
}

#endif
