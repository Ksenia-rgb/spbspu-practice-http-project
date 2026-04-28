#ifndef MODELS_HPP
#define MODELS_HPP
#include <json.hh>
#include <string>
#include <unordered_map>

using json = nlohmann::json;

namespace http
{
  namespace models
  {
    struct Request
    {
      std::string method;
      std::string host;
      std::string path;
      std::unordered_multimap< std::string, std::string > headers;
      json body;
    };

    struct Response
    {
      int status;
      std::unordered_multimap< std::string, std::string > headers;
      json body;
    };
  }
}
#endif
