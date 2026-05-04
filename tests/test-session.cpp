#include <session.hpp>
#include <stdexcept>
#include <vector>
#include <boost/filesystem.hpp>
#include <boost/test/tools/interface.hpp>
#include <boost/test/tools/old/interface.hpp>
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

BOOST_AUTO_TEST_CASE(create_default_session)
{
  Session s;
  BOOST_TEST(s.getName() == "Unknown");
  BOOST_TEST(s.getHistory().empty());
}

BOOST_FIXTURE_TEST_CASE(create_session_with_name, SessionFixture)
{
  const std::string testName = "Test";
  Session s(testName, getPath());
  BOOST_TEST(s.getName() == testName);
  BOOST_TEST(filesystem::exists(temp_dir / (testName + ".json")));
}

BOOST_FIXTURE_TEST_CASE(create_session_with_name_Unknown, SessionFixture)
{
  const std::string unknownName = "Unknown";
  BOOST_CHECK_THROW(Session(unknownName, getPath()), std::logic_error);
  BOOST_TEST(!filesystem::exists(temp_dir / (unknownName + ".json")));
}

BOOST_FIXTURE_TEST_CASE(set_name, SessionFixture)
{
  const std::string testName1 = "Test1", testName2 = "Test2";
  Session s(testName1, getPath());
  s.setName(testName2);
  BOOST_TEST(s.getName() == testName2);
  BOOST_TEST(filesystem::exists(temp_dir / (testName2 + ".json")));
  BOOST_TEST(!filesystem::exists(temp_dir / (testName1 + ".json")));
}

BOOST_FIXTURE_TEST_CASE(set_name_Unknown, SessionFixture)
{
  const std::string testName1 = "Test1", unknownName = "Unknown";
  Session s(testName1, getPath());
  BOOST_CHECK_THROW(s.setName(unknownName), std::logic_error);
  BOOST_TEST(filesystem::exists(temp_dir / (testName1 + ".json")));
  BOOST_TEST(!filesystem::exists(temp_dir / (unknownName + ".json")));
}

BOOST_FIXTURE_TEST_CASE(session_list, SessionFixture)
{
  Session s1("s1", getPath()), s2("s2", getPath()), s3("s3", getPath());
  std::vector< std::string > v = {"s1", "s2", "s3"};
  BOOST_TEST(sessionList(getPath()) == v);
}

BOOST_FIXTURE_TEST_CASE(remove_session, SessionFixture)
{
  const std::string testName = "DeleteMe";
  Session s(testName, getPath());
  BOOST_TEST(filesystem::exists(temp_dir / (testName + ".json")));

  s.removeSession();
  BOOST_TEST(!filesystem::exists(temp_dir / (testName + ".json")));
  BOOST_TEST(s.getName() == "Unknown");
}

BOOST_FIXTURE_TEST_CASE(add_and_get_request, SessionFixture)
{
  Session s("HistoryTest", getPath());
  http::models::Request req;
  http::models::Response res;

  const std::string reqName = "GetUser";
  s.addRequest(reqName, req, res);

  auto pair = s.getRequest(reqName);
  BOOST_TEST(s.getHistory().size() == 1);
  BOOST_TEST(s.getHistory()[0]["name"] == reqName);
}

BOOST_FIXTURE_TEST_CASE(set_mark_and_comment, SessionFixture)
{
  Session s("MetadataTest", getPath());
  http::models::Request req;
  http::models::Response res;

  s.addRequest("Req1", req, res);

  s.setMark("Req1", "Important");
  s.setComment("Req1", "Fix this later");

  ordered_json history = s.getHistoryByName("Req1")[0];
  BOOST_TEST(history["mark"] == "Important");
  BOOST_TEST(history["comment"] == "Fix this later");

  s.setMark("Critical");
  BOOST_TEST(s.getHistory(1)[0]["mark"] == "Critical");
}

BOOST_FIXTURE_TEST_CASE(history_filtering, SessionFixture)
{
  Session s("FilterTest", getPath());
  http::models::Request req;
  http::models::Response res;

  s.addRequest("R1", req, res);
  s.setMark("R1", "TagA");

  s.addRequest("R2", req, res);
  s.setMark("R2", "TagB");

  s.addRequest("R3", req, res);
  s.setMark("R3", "TagA");

  BOOST_TEST(s.getHistoryByMark("TagA").size() == 2);
  BOOST_TEST(s.getHistoryByMark("TagB").size() == 1);
  BOOST_TEST(s.getHistory(1).size() == 1);
}

BOOST_FIXTURE_TEST_CASE(switch_session, SessionFixture)
{
  Session s1("SessionA", getPath());
  Session s2("SessionB", getPath());
  s1.addRequest("ReqA", {}, {});

  s1.switchSession("SessionB");
  BOOST_TEST(s1.getName() == "SessionB");
  BOOST_TEST(s1.getHistory().empty());
  BOOST_TEST(filesystem::exists(temp_dir / "SessionB.json"));
}

BOOST_FIXTURE_TEST_CASE(copy_constructor_with_rename, SessionFixture)
{
  Session s1("Original", getPath());
  s1.addRequest("Data", {}, {});
  s1.save();

  Session s2(s1, "Clone", getPath());

  BOOST_TEST(s2.getName() == "Clone");
  BOOST_TEST(s2.getHistory().size() == 1);
  BOOST_TEST(filesystem::exists(temp_dir / "Clone.json"));
}

BOOST_FIXTURE_TEST_CASE(change_request, SessionFixture)
{
  Session s("UpdateTest", getPath());
  http::models::Request req;
  http::models::Response res1, res2;

  s.addRequest("Request", req, res1);
  s.changeRequest("Request", req, res2);

  BOOST_TEST(s.getHistory().size() == 1);
}
