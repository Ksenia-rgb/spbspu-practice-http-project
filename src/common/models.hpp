#ifndef MODELS_HPP
#define MODELS_HPP
#include <string>
#include <unordered_map>

namespace http
{
  namespace models
  {
    struct Request
    {
      std::string method;
      std::string host;
      std::string path;
      std::unordered_map< std::string, std::string > headers;
      std::string body;
    };

    struct Response
    {
      std::string status_line;
      std::unordered_map< std::string, std::string > headers;
      std::string body;
      std::string error;
    };
  }
}
#endif
