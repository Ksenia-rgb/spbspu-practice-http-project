#include <httplib.h>
#include <iostream>

int main()
{
  httplib::Server svr;

  svr.Get("/hi", [](const auto& req, auto& res) {
    if (req.has_header("User-Agent"))
    {
      auto user_agent = req.get_header_value("User-Agent");
      res.set_content("Hello, " + user_agent, "text/plain");
    }
    else
    {
      res.set_content("Hello!", "text/plain");
    }
  });

  svr.Get("/search", [](const auto& req, auto& res) {
    auto q = req.get_param_value("q");
    res.set_content("Query: " + q, "text/plain");
  });

  svr.Post("/post", [](const auto& req, auto& res) {
    res.set_content(req.body, "text/plain");
  });

  svr.Post("/submit", [](const auto& req, auto& res) {
    std::string result;
    for (const auto& pair : req.params)
    {
      result += pair.first + " = " + pair.second + "\n";
    }
    res.set_content(result, "text/plain");
  });

  svr.Post("/upload", [](const auto& req, auto& res) {
    auto f = req.form.get_file("file");
    auto content = f.filename + " (" + std::to_string(f.content.size()) + " bytes)";
    res.set_content(content, "text/plain");
  });

  svr.Get("/users/:id", [](const auto& req, auto& res) {
    auto id = req.path_params.at("id");
    res.set_content("User ID: " + id, "text/plain");
  });

  svr.Get(R"(/files/(\d+))", [](const auto& req, auto& res) {
    auto id = req.matches[1];
    res.set_content("File ID: " + std::string(id), "text/plain");
  });

  std::cout << "Listening on port 8080..." << std::endl;
  svr.listen("0.0.0.0", 8080);
}
