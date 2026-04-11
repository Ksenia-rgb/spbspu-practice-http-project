#include <string>
#include <vector>
#include "../../libs/json.hh"
#include "../common/models.hpp"

using json = nlohmann::json;

namespace session
{
  class Session
  {
  public:
    Session();
    Session(const Session&) = delete;
    Session(Session&& rhs);
    Session(const Session& rhs, const std::string& name);
    explicit Session(std::string name);
    ~Session();
    Session operator=(const Session&) = delete;
    Session operator=(Session&& rhs);

    std::string getName() const noexcept;
    void setName();
    Session removeSession();
    Session switchSession(const std::string& name);
    const json& getHistory(const std::string& name, size_t limit = 10) const noexcept;
    Session clone(const std::string& name) const;
    void save();
    void setMark(const std::string& markName);
    void setMark(const std::string& reqName, const std::string& markName);
    void setComment(const std::string& comment);
    void setComment(const std::string& reqName, const std::string& comment);
    void addRequest(const std::string& name, const http::models::Request& request);
    void changeRequest(const std::string& name, const http::models::Request& request);

  private:
    std::string name_ = "Unknown";
    json history;
  };

  std::vector< std::string > sessionList();
}
