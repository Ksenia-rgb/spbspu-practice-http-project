#ifndef SEND_HPP
#define SEND_HPP
#include <models.hpp>

namespace http
{
  namespace send
  {
    models::Response sendRequest(const models::Request& req);
  }
}

#endif
