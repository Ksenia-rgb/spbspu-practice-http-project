#include <iostream>
#include <models.hpp>
#include "send.hpp"

int main()
{
  http::models::Request req;
  req.host = "http://localhost:8080";
  req.path = "/hello";
  req.method = "GET";
  http::models::Response resp = http::send::sendRequest(req);
  std::cout << "STATUS: " << resp.status << '\n';
  std::cout << "HEADERS:\n";
  for (const auto& pair : resp.headers)
  {
    std::cout << pair.first << ": " << pair.second << '\n';
  }
  std::cout << "BODY: " << resp.body << '\n';
}
