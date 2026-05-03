#ifndef MODELS_HPP
#define MODELS_HPP
#include <string>
#include <json.hh>
#include <unordered_map>

using ordered_json = nlohmann::ordered_json;

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
      ordered_json body;
    };

    struct Response
    {
      int status = -1;
      std::unordered_multimap< std::string, std::string > headers;
      std::string body;
    };
  }
}

#endif
