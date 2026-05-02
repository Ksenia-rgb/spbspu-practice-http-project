#ifndef REPL_HPP
#define REPL_HPP
#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>
#include <memory>
#include "app_state.hpp"
#include "request.hpp"

namespace http
{
  namespace repl
  {
    bool exitConfirmation(std::istream& in, std::ostream& out);
    std::unique_ptr< cli::Menu > init(cli::LoopScheduler& scheduler, state::AppState& state);
  }
}
#endif
