#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <models.hpp>
#include <nlohmann/json.hpp>

using ordered_json = nlohmann::ordered_json;

namespace http
{
  namespace response
  {
    ordered_json convertResponseToJson(const models::Response& response);
    void saveResponse(const models::Response& response, const std::string& path);
  }
}

#endif
