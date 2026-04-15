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
    std::unique_ptr< cli::Menu > init();
  }
}

#endif
