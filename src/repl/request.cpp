#include "request.hpp"
#include <iostream>
#include <fstream>
#include <json.hh>

using json = nlohmann::json;

namespace http
{
  namespace repl
  {
    namespace req
    {
      using setFunc = void (*)(models::Request&, const std::string&);
      void validInput(std::istream& in,
                      std::ostream& out,
                      const std::string& text,
                      models::Request& request,
                      setFunc setFunc)
      {
        std::string input;
        while (true)
        {
          try
          {
            out << text;
            std::getline(in, input);
            setFunc(request, input);
            break;
          } catch (const std::invalid_argument& e)
          {
            std::cout << e.what();
          }
        }
      }

      void setName(std::string& name, const std::string& new_name)
      {
        name = new_name;
      }

      void setMethod(models::Request& request, const std::string& method)
      {
        std::string copy_method = method;
        for (size_t i = 0; i < method.size(); ++i)
        {
          copy_method[i] =
              static_cast< char >(std::toupper(static_cast< unsigned char >(method[i])));
        }
        if (copy_method == "GET" || copy_method == "POST")
        {
          request.method = copy_method;
        }
        else
        {
          throw std::invalid_argument("invalid method: " + copy_method +
                                      " choose between (POST, GET)\n");
        }
      }

      void setURL(models::Request& request, const std::string& url)
      {
        request.host.clear();
        request.path.clear();
        size_t i = url.find("://");
        if (i == std::string::npos)
        {
          i = 0;
        }
        else
        {
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
        if (request.path.empty())
        {
          request.path = "/";
        }
      }

      void setHeaders(models::Request& request, const std::string& headers)
      {
        request.headers.clear();
        for (size_t i = 0; i < headers.size(); ++i)
        {
          std::string header, value;
          for (; headers[i] != '='; ++i)
          {
            header += headers[i];
          }
          i += 2;
          for (; headers[i] != '\'' && headers[i] != '"'; ++i)
          {
            value += headers[i];
          }
          ++i;
          request.headers[header] = value;
        }
      }

      void setBody(models::Request& request, const std::string& body)
      {
        if (body.empty())
        {
          request.body = json::object();
        }
        else if (body.find("{") == std::string::npos)
        {
          std::ifstream f(body);
          request.body = json::parse(f);
        }
        else
        {
          request.body = json::parse(body);
        }
      }

      void startReqMenu(std::string& name, models::Request& request)
      {
        std::unique_ptr< cli::Menu > reqMenu = reqInit(name, request);
        cli::Cli req(std::move(reqMenu));
        cli::LoopScheduler scheduler;
        req.ExitAction([&scheduler](std::ostream&) {
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
        validInput(in, out, "req method> ", request, setMethod);
        std::string url;
        out << "req URL> ";
        std::getline(in, url);
        setURL(request, url);
        std::string headers;
        out << "req headers> ";
        std::getline(in, headers);
        setHeaders(request, headers);
        out << "req body> ";
        std::string body;
        std::getline(in, body);
        setBody(request, body);
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

      std::unique_ptr< cli::Menu > reqInit(std::string& name, models::Request& request)
      {
        auto reqMenu = std::make_unique< cli::Menu >("req command");
        reqMenu->Insert("show", [&name, &request](std::ostream& out) {
          show(out, name, request);
        });
        reqMenu->Insert("name", [&name](std::ostream&, const std::string& new_name) {
          setName(name, new_name);
        });
        reqMenu->Insert("method", [&request](std::ostream&, const std::string& new_method) {
          setMethod(request, new_method);
        });
        reqMenu->Insert("url", [&request](std::ostream&, const std::string& new_url) {
          setURL(request, new_url);
        });
        reqMenu->Insert("headers", [&request](std::ostream&, const std::string& new_headers) {
          setHeaders(request, new_headers);
        });
        reqMenu->Insert("body", [&request](std::ostream&, const std::string& new_body) {
          setBody(request, new_body);
        });
        return reqMenu;
      }
    }
  }
}
