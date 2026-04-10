#include <istream>
#include "../common/models.hpp"

namespace http
{
  namespace send
  {
    http::models::Response sendRequest(const http::models::Request&);
    http::models::Response sendRequest(std::istream& in);
  }
}
