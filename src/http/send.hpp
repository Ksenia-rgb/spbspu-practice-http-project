#ifndef SEND_HPP
#define SEND_HPP
#include <istream>
#include <models.hpp>

namespace http
{
  namespace send
  {
    models::Response sendRequest(const models::Request& req);
    models::Response sendRequest(std::istream& in);
  }
}

#endif
