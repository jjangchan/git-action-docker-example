//
// Created by jjangchan on 2022/08/05.
//

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <assert.h>
#include <j-boost-logger/BoostLogger.h>
#include <j-curl/CurlHandler.h>
#include "../src/RestServer.h"


using namespace std;

class CICDTest: public ::testing::Test {
protected:
public:
    CICDTest() {
        BoostLogger &logger = BoostLogger::get_instance();
        logger.set_filter(boost::log::trivial::info);
    }
    virtual ~CICDTest() {}
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

TEST_F(CICDTest, PathTest) {
    std::string url = "http://localhost:8090/api/total/abcd";
    RestServer server1(url, 10);
    std::string req_str;
    server1.insert_work_call_function(
            "total", [&](http_request req, std::vector<std::string>& v) {
                std::string id = v[3];
                web::json::value body = web::json::value::object();
                body["client_id"] = web::json::value::string(U(id));
                body["total"] = web::json::value::number(1);
                req_str = body.serialize();
                req.reply(status_codes::OK, body);
            });
    server1.start_server();

    std::string res_str = CurlHandler::url_get_json(url);
    ASSERT_EQ(res_str+"s", req_str); // assert가 발생하면 git action 다음 스텝 run을 가지 못한다. 따라서 도커 이미지 배포가 불가능
    server1.stop_server();
}
