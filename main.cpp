#include <j-boost-logger/BoostLogger.h>
#include "src/RestServer.h"
#include "src/LoadData.h"

// api들을 수행하는 콜백 함수, 람다로 표현
void example(RestServer& server1){
    //exam1) total
    server1.insert_work_call_function(
            "total", [&](http_request req, std::vector<std::string>& v) {
                LoadData& data = LoadData::get_instance();
                std::string id = v[3];
                unsigned long long total = data.get_total().get(id);
                web::json::value body = web::json::value::object();

                body["client_id"] = web::json::value::string(U(id));
                body["total"] = web::json::value::number(total);

                req.reply(status_codes::OK, body);}
    );

    //exam2) list
    server1.insert_work_call_function(
            "list", [&](http_request req, std::vector<std::string>& v) {
                LoadData& data = LoadData::get_instance();
                std::vector<web::json::value> send_json;
                for(SET::Set<std::string>::Iterator iter = data.get_clients().begin(); iter != data.get_clients().end(); ++iter)
                    send_json.push_back(web::json::value::string(U(*iter)));
                web::json::value body = web::json::value::object();
                body[U("clients")] = web::json::value::array(send_json);
                req.reply(status_codes::OK, body);
            });

    //exam3) range -> api/range/min/max
    server1.insert_work_call_function(
            "range", [&](http_request req, std::vector<std::string>& v) {
                LoadData& data = LoadData::get_instance();
                // is number ?
                for(char& c : v[3]){
                    if(!std::isdigit(c)){
                        req.reply(status_codes::NotFound, "invalid min\n");
                        BOOST_LOG_TRIVIAL(trace) << "invalid min";
                        return;
                    }
                }
                for(char& c : v[4]){
                    if(!std::isdigit(c)){
                        req.reply(status_codes::NotFound, "invalid max\n");
                        BOOST_LOG_TRIVIAL(trace) << "invalid max";
                        return;
                    }
                }

                int min = atoi(v[3].c_str());
                int max = atoi(v[4].c_str());
                if(min >= max){
                    req.reply(status_codes::NotFound, "max must be greater than min !\n");
                    BOOST_LOG_TRIVIAL(trace) << "max must be greater than min";
                    return;
                }

                std::vector<unsigned long long> volumes;
                unsigned long long v_sum = 0;
                for(HashMap<int, unsigned long long>::Iterator iter = data.get_range().begin(); iter != data.get_range().begin(); ++iter){
                    int price = iter.get_key();
                    if(price >= min && price < max){
                        unsigned long long volume = iter.get_value();
                        volumes.push_back(volume);
                        v_sum +=  volume;
                    }
                }
                double average = (double)v_sum/(double)volumes.size();
                double variance = 0;
                double sum = 0;
                for(auto ull : volumes){
                    sum += pow(ull-average,2);
                }
                variance = sum/volumes.size();
                double result = sqrt(variance);

                web::json::value body = web::json::value::object();
                body[U("average")] = web::json::value::number(average);
                body[U("std")] = web::json::value::number(result);
                req.reply(status_codes::OK, body);

                //int total = data.get_total().get(id);
            });

    //exam4) check
    server1.insert_work_call_function(
            "check", [&](http_request req, std::vector<std::string>& v) {
                LoadData& data = LoadData::get_instance();
                std::vector<web::json::value> send_json;

                for(int i = 0; i < data.get_miss().size(); i++){
                    send_json.push_back(web::json::value::number(data.get_miss()[i]));
                }
                web::json::value body = web::json::value::object();
                body[U("missing")] = web::json::value::array(send_json);
                req.reply(status_codes::OK, body);
            });

}

int main(){
    BoostLogger::get_instance().set_filter(boost::log::trivial::info);
    BOOST_LOG_TRIVIAL(trace) << "BoostLogger is initialized";

    LoadData::get_instance().init();
    RestServer server1("http://localhost:8090", 10);
    example(server1); // 예제문제 등록하는 함수
    server1.start_server();


    /**
    http_listener_config listen_config;     //설정값 선언(디폴트 120초 타임아웃)
    listen_config.set_timeout(utility::seconds(10));    //10초로 변경

//listener생성. Server URL, Port 지정, 설정값 전달.
    http_listener listener(U("http://localhos:8080"), listen_config);
    listener.support(methods::GET, FunctionHandler::client_total);

    listener.support(methods::GET, [&listener](http_request req){   //기본적인 GET 요청에 대한 명령어 정리.
        auto path = listener.uri().path();
        auto scheme = listener.uri().scheme();
        auto user_info = listener.uri().user_info();
        auto host = listener.uri().host();
        auto port = listener.uri().port();
        auto query = listener.uri().query();
        auto fragment = listener.uri().fragment();

        std::cout << "path : " << path << std::endl;
        std::cout << "scheme : " <<scheme << std::endl;
        std::cout << "user_info : " <<user_info << std::endl;
        std::cout << "host : " <<host << std::endl;
        std::cout << "port : " <<port << std::endl;
        std::cout << "query : " <<query << std::endl;
        std::cout << "fragment : " <<fragment << std::endl;

        std::cout << req.request_uri().path();

        req.reply(status_codes::OK, "hello word!\n");
    });
     **/
    while(true){}
    return 0;

}