#include "request.hpp"
#include <iostream>
#include "../../libs/json.hh"

using json = nlohmann::json;

namespace http
{
  namespace repl
  {
    namespace req
    {
      void setName(std::ostream& out, std::string& name, const std::string& new_name)
      {
        name = new_name;
      }

      void setMethod(std::ostream& out, models::Request& request, const std::string& method)
      {
        request.method = method;
      }

      void setURL(std::ostream& out, models::Request& request, const std::string& url)
      {
        request.host.clear();
        request.path.clear();
        size_t i = url.find("://");
        if (i == std::string::npos)
        {
          i = 0;
        }
        else {
          i += 3;
        }
        for (; url[i] != '/' && url[i] != '\0'; ++i)
        {
          request.host += url[i];
        }
        for (; url[i] != '\0'; ++i)
        {
          request.path += url[i];
        }
        if (request.path.empty()) {
          request.path = "/";
        }
      }

      void setHeaders(std::ostream& out, models::Request& request, const std::string& headers)
      {
        request.headers.clear();
        for (size_t i = 0; i < headers.size(); ++i)
        {
          std::string header, value;
          for (; headers[i] != '='; ++i)
          {
            header+=headers[i];
          }
          i += 2;
          for (; headers[i] != '\'' &&  headers[i] != '"'; ++i)
          {
            value+=headers[i];
          }
          ++i;
          request.headers[header] = value;
        }
      }

      void setBody(std::ostream& out, models::Request& request, const std::string& body)
      {
        if (body.empty()) {
          request.body = json::object();
        } else
        {
          request.body = json::parse(body);
        }
      }

      void startReqMenu(std::string& name, models::Request& request)
      {
        std::unique_ptr< cli::Menu > reqMenu = reqInit(name, request);
        cli::Cli req(std::move(reqMenu));
        cli::LoopScheduler scheduler;
        req.ExitAction([&scheduler](std::ostream& out)
        {
          scheduler.Stop();
        });
        cli::CliLocalTerminalSession session(req, scheduler, std::cout);
        scheduler.Run();
      }

      void req(std::ostream& out)
      {
        std::istream& in = std::cin;
        models::Request request;
        std::string name;
        out << "req name> ";
        std::getline(in, name);
        std::string method;
        out << "req method> ";
        std::getline(in, method);
        setMethod(out, request, method);
        std::string url;
        out << "req URL> ";
        std::getline(in, url);
        setURL(out, request, url);
        std::string headers;
        out << "req headers> ";
        std::getline(in, headers);
        setHeaders(out, request, headers);
        out << "req body> ";
        std::string body;
        std::getline(in, body);
        setBody(out, request, body);

        startReqMenu(name, request);
      }

      void show(std::ostream& out, const std::string& name, const models::Request& request)
      {
        out << "name: " << name << "\n";
        out << "method: " << request.method << "\n";
        out << "host: " << request.host << "\n";
        out << "path: " << request.path << "\n";
        out << "headers:\n";
        for (auto it = request.headers.begin(); it != request.headers.end(); ++it)
        {
          out << "  " << it->first << ": " << it->second << "\n";
        }
        out << "body: " << request.body << "\n";
      }
    }
  }
}
