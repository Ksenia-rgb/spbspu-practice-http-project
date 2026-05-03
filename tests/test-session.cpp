#include <session.hpp>
#include <boost/filesystem.hpp>
#include <boost/test/unit_test.hpp>
#include <boost/test/unit_test_suite.hpp>

using namespace http::session;
namespace filesystem = boost::filesystem;

struct SessionFixture
{
  filesystem::path temp_dir;

  SessionFixture()
  {
    temp_dir = filesystem::current_path() / filesystem::unique_path("test_data_%%%%-%%%%");
    filesystem::create_directories(temp_dir);
  }

  ~SessionFixture()
  {
    if (filesystem::exists(temp_dir))
    {
      filesystem::remove_all(temp_dir);
    }
  }

  std::string getPath() const
  {
    return temp_dir.string() + "/";
  }
};
