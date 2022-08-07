//
// Created by jjangchan on 2022/08/07.
//

#include <gtest/gtest.h>
#include <string>
#include <vector>
#include <j-boost-logger/BoostLogger.h>
#include <j-curl/CurlHandler.h>
#include "../src/LoadData.h"

using namespace std;

class FileFormatTest: public ::testing::Test {
protected:
public:
    FileFormatTest() {
        BoostLogger &logger = BoostLogger::get_instance();
        logger.set_filter(boost::log::trivial::info);
    }
    virtual ~FileFormatTest() {}
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

// 로그에 길이가 28개 맞는지?
TEST_F(FileFormatTest, LogLengthTest) {


}

// 작업고유번호, 체결량, 체결금액이 int가 맞는지?
TEST_F(FileFormatTest, LogIntFormatTest){

}

// 최대 total 금액이 unit64_t에 담을수 있는지?
TEST_F(FileFormatTest, MaxTotalTest){

}

// 로그파일 path가 잘못 됐는지?
TEST_F(FileFormatTest, LogPathTest){

}

