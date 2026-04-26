#include "request.hpp"
#include <iostream>
#include <fstream>
#include <http/send.hpp>
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
            std::cout << "Error: " << e.what() << "\n";
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
                                      " choose between (POST, GET)");
        }
      }

      void setURL(models::Request& request, const std::string& url)
      {
        if (url.empty())
        {
          throw std::invalid_argument("URL cannot be empty");
        }
        std::string host;
        std::string path;
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
          host += url[i];
        }
        for (; url[i] != '\0'; ++i)
        {
          path += url[i];
        }
        if (path.empty())
        {
          path = "/";
        }
        if (host.empty())
        {
          throw std::invalid_argument("host cannot be empty");
        }
        request.host = host;
        request.path = path;
      }

      void setHeaders(models::Request& request, const std::string& headers)
      {
        if (headers.empty())
        {
          request.headers.clear();
          return;
        }
        std::unordered_map< std::string, std::string > new_headers;
        for (size_t i = 0; i < headers.size(); ++i)
        {
          std::string header, value;
          while (i < headers.size() && headers[i] != '=' && headers[i] != ' ')
          {
            header += headers[i];
            ++i;
          }
          if (header.empty())
          {
            throw std::invalid_argument("Header name cannot be empty");
          }
          while (i < headers.size() && headers[i] != '=')
          {
            ++i;
          }
          if (i >= headers.size())
          {
            throw std::invalid_argument("Missing '=' after header: " + header);
          }
          ++i;
          while (i < headers.size() && headers[i] == ' ')
          {
            ++i;
          }
          if (i >= headers.size() || (headers[i] != '"' && headers[i] != '\''))
          {
            throw std::invalid_argument("Header value must be quoted: " + header);
          }
          char quote = headers[i];
          ++i;
          while (i < headers.size() && headers[i] != quote)
          {
            value += headers[i];
            ++i;
          }
          if (i >= headers.size() || headers[i] != quote)
          {
            throw std::invalid_argument("Missing closing quote for header: " + header);
          }
          ++i;
          if (value.empty())
          {
            throw std::invalid_argument("Header value cannot be empty: " + header);
          }
          new_headers[header] = value;
        }
        request.headers = new_headers;
      }

      void setBody(models::Request& request, const std::string& body)
      {
        if (body.empty())
        {
          request.body = json::object();
          return;
        }

        if (body.find("{") == std::string::npos)
        {
          std::ifstream f(body);
          if (!f.is_open())
          {
            throw std::invalid_argument("Cannot open file: " + body);
          }

          f.peek();
          if (f.eof())
          {
            f.close();
            throw std::invalid_argument("File is empty: " + body);
          }

          try
          {
            request.body = json::parse(f);
          } catch (const json::parse_error& e)
          {
            f.close();
            throw std::invalid_argument("Invalid JSON in file: " + body +
                                        "\nJson error: " + std::string(e.what()));
          }
          f.close();
        }
        else
        {
          try
          {
            request.body = json::parse(body);
          } catch (const json::parse_error& e)
          {
            throw std::invalid_argument("Invalid JSON string\nJson error: " +
                                        std::string(e.what()));
          }
        }
      }

      void startReqMenu(std::string& name, models::Request& request)
      {
        models::Response response;
        std::unique_ptr< cli::Menu > reqMenu = reqInit(name, request, response);
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
        validInput(in, out, "req URL> ", request, setURL);
        validInput(in, out, "req headers> ", request, setHeaders);
        validInput(in, out, "req body> ", request, setBody);
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

      std::unique_ptr< cli::Menu >
      reqInit(std::string& name, models::Request& request, models::Response& response)
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
        reqMenu->Insert("execute", [&request, &response](std::ostream& out) {
          response = send::sendRequest(request);
        });
        reqMenu->Insert("save", [&response](std::ostream& out) {
          out << "Here will be save\n";
        });
        return reqMenu;
      }
    }
  }
}
