#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <memory>
#include <models.hpp>
#include <ostream>
#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>

namespace http
{
  namespace repl
  {
    namespace req
    {
      void req(std::ostream& out);
      void show(std::ostream& out, const std::string& name, const models::Request& request);
      void setName(std::ostream& out, std::string& name, const std::string& new_name);
      void setMethod(std::ostream& out, models::Request& request, const std::string& method);
      void setURL(std::ostream& out, models::Request& request, const std::string& url);
      void setHeaders(std::ostream& out, models::Request& request, const std::string& headers);
      void setBody(std::ostream& out, models::Request& request, const std::string& body);
      std::unique_ptr< cli::Menu > reqInit(std::string& name, models::Request& request);
    }
  }
}

#endif
