#include "app_state.hpp"

void http::repl::state::clear(AppState& state)
{
  state.req_name.clear();
  state.request.body.clear();
  state.request.headers.clear();
  state.request.host.clear();
  state.request.method.clear();
  state.request.path.clear();
  state.response.body.clear();
  state.response.headers.clear();
  state.response.status = -1;
  state.cli_state = CliState::NORMAL;
}
