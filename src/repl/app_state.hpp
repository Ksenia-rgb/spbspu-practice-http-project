#ifndef APP_STATE_HPP
#define APP_STATE_HPP
#include <models.hpp>

namespace http::repl
{
  enum class CliState
  {
    NORMAL,
    REQ_INPUT,
    REQ_MENU,
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
