#include "send.hpp"
#include <httplib.h>
#include <stdexcept>
#include "models.hpp"

http::models::Response http::send::sendRequest(const http::models::Request& req)
{
  httplib::Client cli(req.host);
  httplib::Result res;
  if (req.method == "GET")
  {
    res = cli.Get(req.path);
  }
  else if (req.method == "POST")
  {
    res = cli.Post(req.path);
  }
  else if (req.method == "HEAD")
  {
    res = cli.Head(req.path);
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
    if (res->status != 200)
    {
      response.error = "HTTP error";
    }
  }
  else if (!res)
  {
    response.error = httplib::to_string(res.error());
  }
  return response;
}
