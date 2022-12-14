//
// Created by jjangchan on 2022/08/02.
//

#ifndef QRAFT_EXCHANGE_API_RESTHANDLER_H
#define QRAFT_EXCHANGE_API_RESTHANDLER_H

#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <map>
#include <functional>
#include <sstream>
#include <vector>
#include "LoadData.h"
#include <j-boost-logger/BoostLogger.h>

using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace utility;

enum {
    ERROR_CODE_1, // -> 요청되는 api 형식이 잘못된 에러 코드
    ERROR_CODE_2, // -> api/range/min/max -> min 값이 정수가 아닐때 나는 에러 코드
    ERROR_CODE_3, // -> api/range/min/max -> max 값이 정수가 아닐떄 나는 에러 코드
    ERROR_CODE_4  // -> api/range/min/max -> min 값이 max값 보다 크거나 같은때 나는 에러 코드
};

inline static const std::string error_str = "error_code = ";



class RestHandler{
private:
    http_listener listener;
    std::map<std::string, std::function<void(http_request, std::vector<std::string>&)>> work_call_back_func;
    std::atomic<int> work_count;

public:
    RestHandler(utility::string_t &url, http_listener_config config) : listener(url, config) {
        listener.support(methods::GET, std::bind(&RestHandler::get_handle, this, std::placeholders::_1));
    }
    pplx::task<void> open() { return listener.open(); }
    pplx::task<void> close() { return listener.close(); }
    std::string get_url() const {return listener.uri().to_string();};

    // insert call back function
    std::map<std::string, std::function<void(http_request, std::vector<std::string>&)>>& get_work_call_back_func(){return work_call_back_func;};


private:
    void get_handle(http_request req){
        std::vector<std::string> v;
        std::istringstream ss(req.request_uri().path());
        std::string temp;
        while(getline(ss, temp, '/')) v.push_back(temp);
        //req.reply(status_codes::OK, path);

        if(v[1] != "api"){
            req.reply(status_codes::NotFound, error_str+std::to_string(ERROR_CODE_1)+"\n");
            BOOST_LOG_TRIVIAL(info) << "invalid path, path : "+req.request_uri().path();
            return;
        }

        work_count++;
        if(work_call_back_func.count(v[2])){ // api가 있다면...
            work_call_back_func[v[2]](req, v);
        }else{
            req.reply(status_codes::NotFound, error_str+std::to_string(ERROR_CODE_1)+"\n");
            BOOST_LOG_TRIVIAL(info) << "not contain key, requested key : "+v[2];
        }
        work_count--;

    }

};


#endif //QRAFT_EXCHANGE_API_RESTHANDLER_H
