#include "send.hpp"
#include <httplib.h>
#include <models.hpp>
#include <stdexcept>

http::models::Response http::send::sendRequest(const http::models::Request& req)
{
  httplib::Client cli(req.host);
  cli.set_default_headers({{"Accept", "application/json"}, {"User-Agent", "HTTP-Client"}});

  httplib::Result res;
  if (req.method == "GET")
  {
    res = cli.Get(req.path, httplib::Headers(req.headers.begin(), req.headers.end()));
  }
  else if (req.method == "POST")
  {
    res = cli.Post(req.path, httplib::Headers(req.headers.begin(), req.headers.end()),
      req.body.dump(), "application/json");
  }
  else if (req.method == "HEAD")
  {
    res = cli.Head(req.path, httplib::Headers(req.headers.begin(), req.headers.end()));
  }
  else
  {
    throw std::logic_error("Unsupported method");
  }

  models::Response response;
  if (res)
  {
    response.status = res->status;
    response.body = res->body;
    for (const auto& pair : res->headers)
    {
      response.headers.insert(pair);
    }
  }
  else
  {
    throw std::logic_error(httplib::to_string(res.error()));
  }
  return response;
}
