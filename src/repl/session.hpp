#ifndef SESSION_HPP
#define SESSION_HPP
#include <json.hh>
#include <models.hpp>
#include <string>
#include <vector>

using ordered_json = nlohmann::ordered_json;

namespace http
{
  namespace session
  {
    class Session
    {
    public:
      Session() = default;
      Session(const Session&) = delete;
      Session(const Session& rhs, const std::string& name);
      explicit Session(std::string name);
      Session operator=(const Session&) = delete;

      std::string getName() const noexcept;
      void setName(const std::string& name);
      void removeSession();
      void switchSession(const std::string& name);
      ordered_json getHistory(size_t limit = 10) const;
      ordered_json getHistoryByName(const std::string& reqName) const;
      ordered_json getHistoryByMark(const std::string& markName, size_t limit = 10) const;
      void save();
      void setMark(const std::string& markName);
      void setMark(const std::string& reqName, const std::string& markName);
      void setComment(const std::string& comment);
      void setComment(const std::string& reqName, const std::string& comment);
      void addRequest(const std::string& name, const http::models::Request& request);
      void changeRequest(const std::string& name, const http::models::Request& request);
      std::pair< http::models::Request, http::models::Response > getRequest(const std::string& name);

    private:
      std::string name_ = "Unknown";
      ordered_json history_;

      void read();
    };

    std::vector< std::string > sessionList();
  }
}

#endif
