#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <models.hpp>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace http
{
  namespace response
  {
    json convertResponseToJson(const models::Response& response);
    void saveResponse(const models::Response& response, const std::string& path);
  }
}

#endif
