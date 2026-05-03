#include <algorithm>
#include <boost/test/unit_test.hpp>
#include <cstdio>
#include <fstream>
#include <sstream>
#include "models.hpp"
#include "response.hpp"

using ordered_json = nlohmann::ordered_json;

BOOST_AUTO_TEST_SUITE(convert_response_to_json_suite)

BOOST_AUTO_TEST_CASE(valid_json_body_test)
{
  http::models::Response response;
  response.status = 200;
  response.body = R"({"message": "ok"})";
  response.headers = {{"Content-Type", "application/json"}, {"X-Custom", "value"}};

  ordered_json result = http::response::convertResponseToJson(response);

  BOOST_TEST(result["status-line"] == "200 OK");
  BOOST_TEST(result["headers"].size() == 2);
  std::vector< std::string > headers = result["headers"];
  bool found_ct =
    std::find(headers.begin(), headers.end(), "Content-Type: application/json") != headers.end();
  BOOST_TEST(found_ct);
  bool found_custom = std::find(headers.begin(), headers.end(), "X-Custom: value") != headers.end();
  BOOST_TEST(found_custom);
  BOOST_TEST(result["body"]["message"] == "ok");
}

BOOST_AUTO_TEST_CASE(invalid_json_body_test)
{
  http::models::Response response;
  response.status = 404;
  response.body = "Not Found (plain text)";
  response.headers = {};

  ordered_json result = http::response::convertResponseToJson(response);

  BOOST_TEST(result["status-line"] == "404 Not Found");
  BOOST_TEST(result["headers"].size() == 0);
  BOOST_TEST(result["body"] == "Not Found (plain text)");
}

BOOST_AUTO_TEST_CASE(empty_body_test)
{
  http::models::Response response;
  response.status = 204;
  response.body = "";

  ordered_json result = http::response::convertResponseToJson(response);

  BOOST_TEST(result["status-line"] == "204 No Content");
  BOOST_TEST(result["body"] == "");
}

BOOST_AUTO_TEST_CASE(non_200_status_test)
{
  http::models::Response response;
  response.status = 500;
  response.body = "{\"error\": \"internal\"}";

  ordered_json result = http::response::convertResponseToJson(response);

  BOOST_TEST(result["status-line"] == "500 Internal Server Error");
  BOOST_TEST(result["body"]["error"] == "internal");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(save_response_suite)

BOOST_AUTO_TEST_CASE(save_valid_response_test)
{
  http::models::Response response;
  response.status = 200;
  response.body = R"({"saved": true})";
  response.headers = {{"Content-Type", "application/json"}};

  std::string filename = "test_save_response.json";
  http::response::saveResponse(response, filename);

  std::ifstream file(filename);
  BOOST_TEST(file.is_open());
  ordered_json saved = ordered_json::parse(file);
  file.close();

  BOOST_TEST(saved["status-line"] == "200 OK");
  BOOST_TEST(saved["body"]["saved"] == true);

  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(save_not_executed_test)
{
  http::models::Response response;
  response.status = -1;

  BOOST_CHECK_THROW(http::response::saveResponse(response, "any.json"), std::logic_error);
}

BOOST_AUTO_TEST_CASE(save_invalid_path_test)
{
  http::models::Response response;
  response.status = 200;
  response.body = "{}";

  BOOST_CHECK_THROW(
    http::response::saveResponse(response, "/invalid/path/file.json"), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()