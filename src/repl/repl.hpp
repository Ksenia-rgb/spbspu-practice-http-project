#ifndef REPL_HPP
#define REPL_HPP
#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>
#include <memory>
#include "app_state.hpp"
#include "request.hpp"

namespace http::repl
{
  std::unique_ptr< cli::Menu > init(cli::LoopScheduler& scheduler, AppState& state);
}

#endif
