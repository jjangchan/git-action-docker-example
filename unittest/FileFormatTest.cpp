//
// Created by jjangchan on 2022/08/07.
//

#include <gtest/gtest.h>
#include <string>
#include <filesystem>
#include <vector>
#include <boost/format.hpp>
#include <j-boost-logger/BoostLogger.h>
#include <j-curl/CurlHandler.h>
#include "../src/LoadData.h"

using namespace std;

class FileFormatTest: public ::testing::Test {
protected:
public:
    std::string path = "./test_data/";
    std::string date = current_date::get_current_date();
    std::string directory_tree;
    FileFormatTest() {
        directory_tree = path+date;
        BoostLogger &logger = BoostLogger::get_instance();
        logger.set_filter(boost::log::trivial::info);
    }
    virtual ~FileFormatTest() {}
public:
public:
    virtual void SetUp() {
        BOOST_LOG_TRIVIAL(trace) << "setup";
        std::filesystem::create_directories(directory_tree);
        LoadData::get_instance().set_path(path, date);
        BoostLogger &logger = BoostLogger::get_instance();
        logger.set_filter(boost::log::trivial::info);

    }
    virtual void TearDown() {
        BOOST_LOG_TRIVIAL(trace) << "teardown ";
        std::filesystem::remove_all(path);
    }
};


std::ofstream& operator<<(std::ofstream& o, std::string& str){
    o << str;
    return o;
}

// 로그에 길이가 28개 맞는지?
TEST_F(FileFormatTest, LogLengthTest) {
    std::string bad_log_length_29 = "bbbbKR00010000030000310991000";
    std::ofstream out(directory_tree+"/test.txt");
    out << bad_log_length_29+"\n";
    out.close();
    ASSERT_EQ(LoadData::get_instance().init(), false);

}

// 작업고유번호, 체결량, 체결금액이 int가 맞는지?
TEST_F(FileFormatTest, LogIntFormatTest){
    std::string bad_log_not_int = "bbbbKR000100000300003109910a";
    std::ofstream out(directory_tree+"/test.txt");
    out << bad_log_not_int+"\n";
    out.close();
    ASSERT_EQ(LoadData::get_instance().init(), false);
}

// 최대 total 금액이 unit64_t에 담을수 있는지?
TEST_F(FileFormatTest, MaxTotalTest){
    std::string client_code = "aaaa";
    std::string stocks = "KR0001";
    int volume = 999999;
    int price = 999999;
    std::vector<std::string> times = {"/09_10.txt", "/10_11.txt", "/11_12.txt", "/12_13.txt", "/13_14.txt", "/14_15.txt", "/15_16.txt"};

    for(const auto& time : times){
        std::string log = "";
        for(int unique = 1; unique <= 999999; unique++){
            std::string num = boost::io::str(boost::format("%06d")%unique);
            log.append(client_code+stocks+num+std::to_string(volume)+ std::to_string(price)+"\n");
        }
        std::cout << directory_tree+time << std::endl;
        std::ofstream out(directory_tree+time);
        std::stringstream ss(log);
        // stringstream not error --> std::string is error code(signal 11)
        out << ss.str();
        out.close();
    }
    LoadData::get_instance().init();
    uint64_t total = LoadData::get_instance().get_total()[client_code];
    ASSERT_TRUE(total < 0xffffffffffffffff);
    std::filesystem::remove_all(path);
}
