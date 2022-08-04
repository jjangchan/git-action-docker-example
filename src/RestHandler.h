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

        work_count++;
        if(work_call_back_func.count(v[2])){ // api가 있다면...
            work_call_back_func[v[2]](req, v);
        }else{
            req.reply(status_codes::NotFound, "invalid api\n");
            BOOST_LOG_TRIVIAL(info) << "not contain key";
        }
        work_count--;

    }

};

#endif //QRAFT_EXCHANGE_API_RESTHANDLER_H
