#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include <json.hh>
#include <models.hpp>

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
