#ifndef REQUEST_HPP
#define REQUEST_HPP
#include <ostream>
#include <memory>
#include <cli/cli.h>
#include <cli/clilocalsession.h>
#include <cli/loopscheduler.h>
#include "../common/models.hpp"

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
      inline std::unique_ptr< cli::Menu > reqInit(std::string& name, models::Request& request)
      {
        auto reqMenu = std::make_unique< cli::Menu >("req command");
        reqMenu->Insert("show", [&name, &request](std::ostream& out)
        {
          show(out, name, request);
        });
        reqMenu->Insert("name", [&name](std::ostream& out, const std::string& new_name)
        {
          setName(out, name, new_name);
        });
        reqMenu->Insert("method", [&request](std::ostream& out, const std::string& new_method)
        {
          setMethod(out, request, new_method);
        });
        reqMenu->Insert("url", [&request](std::ostream& out, const std::string& new_url)
        {
          setURL(out, request, new_url);
        });
        reqMenu->Insert("headers", [&request](std::ostream& out, const std::string& new_headers)
        {
          setHeaders(out, request, new_headers);
        });
        reqMenu->Insert("body", [&request](std::ostream& out, const std::string& new_body)
        {
          setBody(out, request, new_body);
        });
        return reqMenu;
      }
    }
  }
}

#endif
