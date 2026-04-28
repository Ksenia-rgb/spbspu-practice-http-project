#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>
#include <memory>
#include <models.hpp>
#include <ostream>

namespace http::repl::req
{
  void reqInput(std::string& name, models::Request& request, std::istream& in, std::ostream& out);
  void inputFromFile(models::Request& request, const std::string& path);
  void show(std::ostream& out, const std::string& name, const models::Request& request);
  void setName(std::string& name, const std::string& new_name);
  void setMethod(models::Request& request, const std::string& method);
  void setURL(models::Request& request, const std::string& url);
  void setHeaders(models::Request& request, const std::string& headers);
  void setBody(models::Request& request, const std::string& body);
  std::unique_ptr< cli::Menu > reqInit(
    std::string& name, models::Request& request, models::Response& response);
}

#endif
