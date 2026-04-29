#ifndef APP_STATE_HPP
#define APP_STATE_HPP
#include <models.hpp>

namespace http::repl
{
  enum class CliState
  {
    NORMAL,
    REQ_INPUT,
    REQ_FILE,
    REQ_MENU,
    FILE_EXEPTION,
    EXIT
  };

  struct AppState
  {
    CliState cli_state = CliState::NORMAL;
    std::string req_name;
    models::Request request;
    models::Response response;
  };
}

#endif
