#include <boost/test/unit_test.hpp>
#include <cstdio>
#include <fstream>
#include <models.hpp>
#include <request.hpp>
#include <sstream>

BOOST_AUTO_TEST_SUITE(set_method_suite)

BOOST_AUTO_TEST_CASE(valid_get_test)
{
  http::models::Request request;
  http::repl::req::setMethod(request, "GET");
  BOOST_TEST(request.method == "GET");
}

BOOST_AUTO_TEST_CASE(valid_post_test)
{
  http::models::Request request;
  http::repl::req::setMethod(request, "POST");
  BOOST_TEST(request.method == "POST");
}

BOOST_AUTO_TEST_CASE(valid_head_test)
{
  http::models::Request request;
  http::repl::req::setMethod(request, "HEAD");
  BOOST_TEST(request.method == "HEAD");
}

BOOST_AUTO_TEST_CASE(lowercase_test)
{
  http::models::Request request;
  http::repl::req::setMethod(request, "get");
  BOOST_TEST(request.method == "GET");
}

BOOST_AUTO_TEST_CASE(mixed_case_test)
{
  http::models::Request request;
  http::repl::req::setMethod(request, "PoSt");
  BOOST_TEST(request.method == "POST");
}

BOOST_AUTO_TEST_CASE(invalid_method_test)
{
  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::setMethod(request, "PUT"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(empty_method_test)
{
  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::setMethod(request, ""), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(set_name_suite)

BOOST_AUTO_TEST_CASE(set_new_name_test)
{
  std::string name = "old_name";
  http::repl::req::setName(name, "new_name");
  BOOST_TEST(name == "new_name");
}

BOOST_AUTO_TEST_CASE(set_empty_name_test)
{
  std::string name = "old_name";
  http::repl::req::setName(name, "");
  BOOST_TEST(name == "");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(set_url_suite)

BOOST_AUTO_TEST_CASE(full_url_test)
{
  http::models::Request request;
  http::repl::req::setURL(request, "https://example.com/path/to/resource");
  BOOST_TEST(request.host == "example.com");
  BOOST_TEST(request.path == "/path/to/resource");
}

BOOST_AUTO_TEST_CASE(no_scheme_test)
{
  http::models::Request request;
  http::repl::req::setURL(request, "example.com/path");
  BOOST_TEST(request.host == "example.com");
  BOOST_TEST(request.path == "/path");
}

BOOST_AUTO_TEST_CASE(root_path_test)
{
  http::models::Request request;
  http::repl::req::setURL(request, "example.com");
  BOOST_TEST(request.host == "example.com");
  BOOST_TEST(request.path == "/");
}

BOOST_AUTO_TEST_CASE(http_scheme_test)
{
  http::models::Request request;
  http::repl::req::setURL(request, "http://test.org/api");
  BOOST_TEST(request.host == "test.org");
  BOOST_TEST(request.path == "/api");
}

BOOST_AUTO_TEST_CASE(empty_url_test)
{
  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::setURL(request, ""), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(only_scheme_test)
{
  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::setURL(request, "https://"), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(set_headers_suite)

BOOST_AUTO_TEST_CASE(single_header_test)
{
  http::models::Request request;
  http::repl::req::setHeaders(request, "Content-Type=\"application/json\"");
  BOOST_TEST(request.headers.size() == 1);
  BOOST_TEST(request.headers.count("Content-Type") == 1);

  auto range = request.headers.equal_range("Content-Type");
  bool found = false;
  for (auto it = range.first; it != range.second; ++it)
  {
    if (it->second == "application/json")
    {
      found = true;
      break;
    }
  }
  BOOST_TEST(found);
}

BOOST_AUTO_TEST_CASE(multiple_headers_test)
{
  http::models::Request request;
  http::repl::req::setHeaders(
    request, "Content-Type=\"application/json\" Authorization=\"Bearer token123\"");
  BOOST_TEST(request.headers.size() == 2);
  BOOST_TEST(request.headers.count("Content-Type") == 1);
  BOOST_TEST(request.headers.count("Authorization") == 1);

  auto ct_range = request.headers.equal_range("Content-Type");
  bool ct_found = false;
  for (auto it = ct_range.first; it != ct_range.second; ++it)
  {
    if (it->second == "application/json")
    {
      ct_found = true;
      break;
    }
  }
  BOOST_TEST(ct_found);

  auto auth_range = request.headers.equal_range("Authorization");
  bool auth_found = false;
  for (auto it = auth_range.first; it != auth_range.second; ++it)
  {
    if (it->second == "Bearer token123")
    {
      auth_found = true;
      break;
    }
  }
  BOOST_TEST(auth_found);
}

BOOST_AUTO_TEST_CASE(single_quotes_test)
{
  http::models::Request request;
  http::repl::req::setHeaders(request, "Key='value with spaces'");
  BOOST_TEST(request.headers.size() == 1);
  BOOST_TEST(request.headers.count("Key") == 1);

  auto range = request.headers.equal_range("Key");
  bool found = false;
  for (auto it = range.first; it != range.second; ++it)
  {
    if (it->second == "value with spaces")
    {
      found = true;
      break;
    }
  }
  BOOST_TEST(found);
}

BOOST_AUTO_TEST_CASE(empty_headers_test)
{
  http::models::Request request;
  request.headers.insert({"Existing", "Header"});
  http::repl::req::setHeaders(request, "");
  BOOST_TEST(request.headers.empty());
}

BOOST_AUTO_TEST_CASE(missing_equals_test)
{
  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::setHeaders(request, "HeaderName"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(unquoted_value_test)
{
  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::setHeaders(request, "Header=unquoted"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(missing_closing_quote_test)
{
  http::models::Request request;
  BOOST_CHECK_THROW(
    http::repl::req::setHeaders(request, "Header=\"incomplete"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(empty_value_test)
{
  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::setHeaders(request, "Header=\"\""), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(empty_name_test)
{
  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::setHeaders(request, "=\"value\""), std::invalid_argument);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(set_body_suite)

BOOST_AUTO_TEST_CASE(json_string_test)
{
  http::models::Request request;
  http::repl::req::setBody(request, "{\"key\": \"value\"}");
  BOOST_TEST(request.body["key"] == "value");
}

BOOST_AUTO_TEST_CASE(empty_body_test)
{
  http::models::Request request;
  request.body = ordered_json::object();
  request.body["existing"] = "data";
  http::repl::req::setBody(request, "");
  BOOST_TEST(request.body.empty());
}

BOOST_AUTO_TEST_CASE(invalid_json_string_test)
{
  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::setBody(request, "{invalid json}"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(from_file_test)
{
  std::string filename = "test_body.json";
  std::ofstream file(filename);
  file << "{\"file_key\": \"file_value\"}";
  file.close();

  http::models::Request request;
  http::repl::req::setBody(request, filename);
  BOOST_TEST(request.body["file_key"] == "file_value");

  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(nonexistent_file_test)
{
  http::models::Request request;
  BOOST_CHECK_THROW(
    http::repl::req::setBody(request, "nonexistent_file.json"), std::invalid_argument);
}

BOOST_AUTO_TEST_CASE(empty_file_test)
{
  std::string filename = "empty_test.json";
  std::ofstream file(filename);
  file.close();

  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::setBody(request, filename), std::invalid_argument);

  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(invalid_json_file_test)
{
  std::string filename = "invalid.json";
  std::ofstream file(filename);
  file << "{invalid json content}";
  file.close();

  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::setBody(request, filename), std::invalid_argument);

  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(input_from_file_suite)

BOOST_AUTO_TEST_CASE(get_request_test)
{
  std::string filename = "test_request.txt";
  std::ofstream file(filename);
  file << "GET /api/users HTTP/1.1\n";
  file << "Host: example.com\n";
  file << "Content-Type: application/json\n";
  file << "\n";
  file << "{\"user\": \"test\"}";
  file.close();

  http::models::Request request;
  http::repl::req::inputFromFile(request, filename);

  BOOST_TEST(request.method == "GET");
  BOOST_TEST(request.host == "example.com");
  BOOST_TEST(request.path == "/api/users");
  BOOST_TEST(request.headers.count("Host") > 0);
  BOOST_TEST(request.headers.count("Content-Type") > 0);
  BOOST_TEST(request.body["user"] == "test");

  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(post_request_test)
{
  std::string filename = "test_post.txt";
  std::ofstream file(filename);
  file << "POST /submit HTTP/1.1\n";
  file << "Host: test.com\n";
  file << "\n";
  file << "{\"data\": \"value\"}";
  file.close();

  http::models::Request request;
  http::repl::req::inputFromFile(request, filename);

  BOOST_TEST(request.method == "POST");
  BOOST_TEST(request.host == "test.com");
  BOOST_TEST(request.path == "/submit");

  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(nonexistent_file_test)
{
  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::inputFromFile(request, "nonexistent.txt"), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(empty_file_test)
{
  std::string filename = "empty_req.txt";
  std::ofstream file(filename);
  file.close();

  http::models::Request request;
  BOOST_CHECK_THROW(http::repl::req::inputFromFile(request, filename), std::runtime_error);

  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(show_suite)

BOOST_AUTO_TEST_CASE(show_request_test)
{
  http::models::Request request;
  request.method = "GET";
  request.host = "example.com";
  request.path = "/test";
  request.headers.insert({"Content-Type", "application/json"});
  request.body = ordered_json::parse("{\"key\": \"value\"}");

  std::ostringstream out;
  http::repl::req::show(out, "test_request", request);

  std::string output = out.str();
  BOOST_TEST(output.find("name: test_request") != std::string::npos);
  BOOST_TEST(output.find("method: GET") != std::string::npos);
  BOOST_TEST(output.find("host: example.com") != std::string::npos);
  BOOST_TEST(output.find("path: /test") != std::string::npos);
  BOOST_TEST(output.find("Content-Type: application/json") != std::string::npos);
  BOOST_TEST(output.find("\"key\"") != std::string::npos);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(create_template_file_suite)

BOOST_AUTO_TEST_CASE(create_valid_template_test)
{
  std::string filename = "test_template.tmpl";
  http::repl::req::createTemplateFile(filename);

  std::ifstream file(filename);
  BOOST_TEST(file.is_open());

  std::string content;
  std::getline(file, content);
  BOOST_TEST(content.find("<METHOD> <PATH> HTTP/1.1") != std::string::npos);

  file.close();
  std::remove(filename.c_str());
}

BOOST_AUTO_TEST_CASE(create_invalid_path_test)
{
  BOOST_CHECK_THROW(
    http::repl::req::createTemplateFile("/invalid/path/template.tmpl"), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(req_input_suite)

BOOST_AUTO_TEST_CASE(get_request_test)
{
  std::istringstream in("test_req\nGET\nexample.com/api\n\n");
  std::ostringstream out;
  std::string name;
  http::models::Request request;

  http::repl::req::reqInput(name, request, in, out);

  BOOST_TEST(name == "test_req");
  BOOST_TEST(request.method == "GET");
  BOOST_TEST(request.host == "example.com");
  BOOST_TEST(request.path == "/api");
}

BOOST_AUTO_TEST_CASE(post_request_test)
{
  std::istringstream in("post_req\nPOST\nhttps://test.com/submit\nContent-Type=\"application/"
                        "json\"\n{\"key\":\"val\"}\n");
  std::ostringstream out;
  std::string name;
  http::models::Request request;

  http::repl::req::reqInput(name, request, in, out);

  BOOST_TEST(name == "post_req");
  BOOST_TEST(request.method == "POST");
  BOOST_TEST(request.host == "test.com");
  BOOST_TEST(request.path == "/submit");
  BOOST_TEST(request.body["key"] == "val");
}

BOOST_AUTO_TEST_CASE(invalid_method_retry_test)
{
  std::istringstream in("req\nINVALID\nGET\nexample.com\n\n");
  std::ostringstream out;
  std::string name;
  http::models::Request request;

  http::repl::req::reqInput(name, request, in, out);

  BOOST_TEST(request.method == "GET");
}

BOOST_AUTO_TEST_SUITE_END()