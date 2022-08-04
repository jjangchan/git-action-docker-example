//
// Created by jjangchan on 2022/08/02.
//

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <j-boost-logger/BoostLogger.h>
#include <j-curl/CurlHandler.h>
#include "../src/RestServer.h"


using namespace std;

class GetPathTest: public ::testing::Test {
protected:
public:
    GetPathTest() {
        BoostLogger &logger = BoostLogger::get_instance();
        logger.set_filter(boost::log::trivial::info);
    }
    virtual ~GetPathTest() {}
public:
public:
    virtual void SetUp() {
        BOOST_LOG_TRIVIAL(trace) << "setup";

        BoostLogger &logger = BoostLogger::get_instance();
        logger.set_filter(boost::log::trivial::info);

    }
    virtual void TearDown() {
        BOOST_LOG_TRIVIAL(trace) << "teardown ";
    }
};

TEST_F(GetPathTest, PathTest) {
    std::string url = "http://localhost:8090";
    RestServer server1(url, 10);
    std::vector<std::string> sends = {"/api/check", "/api/list", "/api/total", "/api/range"};

    server1.insert_work_call_function(
            "check", [&](http_request req, std::vector<std::string>& v) {
                req.reply(status_codes::OK, req.request_uri().path());
            });
    server1.insert_work_call_function(
            "list", [&](http_request req, std::vector<std::string>& v) {
                req.reply(status_codes::OK, req.request_uri().path());
            });
    server1.insert_work_call_function(
            "total", [&](http_request req, std::vector<std::string>& v) {
                req.reply(status_codes::OK, req.request_uri().path());
            });
    server1.insert_work_call_function(
            "range", [&](http_request req, std::vector<std::string>& v) {
                req.reply(status_codes::OK, req.request_uri().path());
            });
    server1.start_server();
    for(auto str : sends){
        std::string full = url+str;
        std::string req_path = CurlHandler::url_get_json(full);
        ASSERT_EQ(str, req_path);
    }
    server1.stop_server();


}
