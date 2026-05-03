#include <json.hh>
#include <models.hpp>
#include <send.hpp>
#include <stdexcept>
#include <boost/test/tools/old/interface.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>
#define HOST "localhost:8080"

using ordered_json = nlohmann::ordered_json;

BOOST_AUTO_TEST_CASE(get)
{
  http::models::Request req;
  req.method = "GET";
  req.host = HOST;
  req.path = "/get";
  http::models::Response res = http::send::sendRequest(req);
  BOOST_TEST(res.status == 200);
  BOOST_TEST(ordered_json::parse(res.body)["response"] == "Hello!");
}

BOOST_AUTO_TEST_CASE(post)
{
  http::models::Request req;
  req.method = "POST";
  req.host = HOST;
  req.path = "/post";
  req.body["test"] = "Hello2!";
  http::models::Response res = http::send::sendRequest(req);
  BOOST_TEST(res.status == 200);
  BOOST_TEST(ordered_json::parse(res.body)["test"] == "Hello2!");
}

BOOST_AUTO_TEST_CASE(head)
{
  http::models::Request req;
  req.method = "HEAD";
  req.host = HOST;
  req.path = "/head";
  http::models::Response res = http::send::sendRequest(req);
  BOOST_TEST(res.status == 200);
  BOOST_TEST(res.body.empty());
}

BOOST_AUTO_TEST_CASE(incorrect_host)
{
  http::models::Request req;
  req.method = "GET";
  req.host = "invalid.host.that.does.not.exist";
  req.path = "/";
  BOOST_CHECK_THROW(http::send::sendRequest(req), std::logic_error);
}

BOOST_AUTO_TEST_CASE(incorrect_path)
{
  http::models::Request req;
  req.method = "GET";
  req.host = HOST;
  req.path = "/my_endpoint";
  http::models::Response res = http::send::sendRequest(req);
  BOOST_TEST(res.status == 404);
}
