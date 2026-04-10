#include "request.hpp"
#include <iostream>

namespace http
{
  namespace repl
  {
    void req(std::ostream& out)
    {
      std::istream& in = std::cin;
      models::Request request;
      std::string name;
      out << "req name> ";
      std::getline(in, name);
      out << "req method> ";
      std::getline(in, request.method);
      std::string url;
      out << "req URL> ";
      std::getline(in, url);
      size_t i = url.find("://") + 3;
      if (i == std::string::npos)
      {
        i = 0;
      }
      for (; url[i] != '/'; ++i)
      {
        request.host += url[i];
      }
      for (; url[i] != 0; ++i)
      {
        request.path += url[i];
      }
      std::string headers;
      out << "req headers> ";
      std::getline(in, headers);
      for (size_t i = 0; i < headers.size(); ++i)
      {
        std::string header, value;
        for (; headers[i] != '='; ++i)
        {
          header+=headers[i];
        }
        ++i;
        for (; headers[i] != ' ' &&  headers[i] != '\0'; ++i)
        {
          value+=headers[i];
        }
        request.headers[header] = value;
      }
      out << "req body> ";
      std::getline(in, request.body);

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
