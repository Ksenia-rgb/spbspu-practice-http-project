#include "response.hpp"
#include <httplib.h>
#include <fstream>

json http::response::convertResponseToJson(const models::Response& response)
{
  json json_response;
  json_response["status-line"] =
    std::to_string(response.status) + ' ' + httplib::status_message(response.status);
  json_response["headers"] = json::array();
  for (const auto& header : response.headers)
  {
    json_response["headers"].push_back(header.first + ": " + header.second);
  }
  json body;
  try
  {
    body = json::parse(response.body);
  }
  catch (const json::parse_error& e)
  {
    json_response["body"] = response.body;
    return json_response;
  }

  json_response["body"] = body;
  return json_response;
}

void http::response::saveResponse(const models::Response& response, const std::string& path)
{
  if (response.status == -1)
  {
    throw std::logic_error("Request did not execute");
  }

  json json_response = convertResponseToJson(response);
  std::ofstream file(path);
  if (!file.is_open())
  {
    throw std::runtime_error("Error: can not create file");
  }
  file << std::setw(2) << json_response;
  file.close();
}
