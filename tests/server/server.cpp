#include <httplib.h>
#include <json.hh>

using ordered_json = nlohmann::ordered_json;

int main()
{
  httplib::Server svr;

  svr.Get("/get", [](const auto&, auto& res)
  {
    ordered_json j;
    j["response"] = "Hello!";
    res.set_content(j.dump(), "application/json");
  });

  svr.Post("/post", [](const auto& req, auto& res)
  {
    res.set_content(req.body, "application/json");
  });

  svr.Get("/head", [](const auto& req, auto& res)
  {
    if (req.method == "HEAD")
    {
      return;
    }
    res.status = 405;
  });
  svr.listen("0.0.0.0", 8080);
}
