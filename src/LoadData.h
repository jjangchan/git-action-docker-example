//
// Created by jjangchan on 2022/08/02.
//

#ifndef QRAFT_EXCHANGE_API_LOADDATA_H
#define QRAFT_EXCHANGE_API_LOADDATA_H

#include <iostream>
#include <string>
#include <chrono>
#include <ctime>
#include <filesystem>
#include <fstream>
#include <set>
#include <map>
#include <vector>
#include <j-boost-logger/BoostLogger.h>

class current_date{
public:
    static std::string get_current_date(){
        time_t t = std::time(nullptr);
        auto tm = *std::localtime(&t);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%Y-%m-%d");
        return oss.str();
    }
};

class LoadData{
private:
    /**
    struct InfoData{
        std::string stocks_code;
        int unique_num;
        int volume;
        int price;
        InfoData(std::string _stocks_code, int _unique_num, int _volume, int _price) :
                stocks_code(_stocks_code),
                unique_num(_unique_num),
                volume(_volume),
                price(_price){}
    };
    HashMap<std::string, Vector<InfoData>> m_infors;
    Vector<InfoData> infos;
    **/

    // exam) api/total, api/range
    // key 값 기준으로 검색해야 하므로, 중복을 허용 하지 않고 최대한 빠르게 검색하는 자료구조는 map(O(logN)) 또는 unordered_map(O(1)) 이여서 hashp_map 선택
    // 한 고객이 최대 담을수 있는 금액은 아래와 같다
    // 최대 고유 번호, 체결량, 체결금액 : 999,999 , 최대 파일 생성 개수 : 6 (9-10, 10-11, 11-12, 12-13, 13-14, 14-15, 15-16)
    // 999,999 * 999,999 * 999,999 * 6 는 2^64-1 보다 작다, 따라서 total max 값을 uint64_t 으로 표현 가능 하다?
    // 만약 2^64-1 범위를 넘는 경우가 있으면 value 타입을 vector로 변경해야한다.
    std::map<int, uint64_t> range; // -> std::map, 정렬되어있는상태
    std::map<std::string, uint64_t> total;

    // exam) api/list, api/range, api/check
    // set은 중복되지 않고 정렬되어 있는 상태에 자료구조 이여서 선택
    std::set<std::string> clients;
    std::set<int> u_nums; // 최대 고유 번호 -> 999999
    std::vector<int> miss;
    std::string path;
    std::string date;

    LoadData(){}
    ~LoadData(){}


public:

    bool init(){ // 오후 5시 돠면 init() 함수로 호출해서 log를 담는 자료구조를 초기화 해야함..
        /** 현재 날짜 조회해서 해당 data 가져오기
        date = get_date();
        **/
        map<std::string ,int> file_to_index={{"09_10.txt",0},
                                                     {"10_11.txt",1},
                                                     {"11_12.txt", 2},
                                                     {"12_13.txt",3},
                                                     {"13_14.txt", 4},
                                                     {"14_15.txt",5}};
        // get file list
        try{
            for(const auto& file : std::filesystem::directory_iterator(path+date)){
                std::ifstream in(file.path().c_str());
                std::cout << "read file = " << file.path() << std::endl;
                //int u_index =  file_to_index[file.path().string().substr(path.size()+date.size()+1)];
                std::stringstream buffer;
                buffer << in.rdbuf();
                std::string str;
                while(getline(buffer, str, '\n')){
                    if(str.size() != 28){
                        BOOST_LOG_TRIVIAL(error) << "invalid log length format, length = "+ std::to_string(str.size())+", log = "+ str;
                        return false;
                    }

                    for(int i = 10; i < 28; i++){
                        if(!std::isdigit(str[i])){
                            BOOST_LOG_TRIVIAL(error) << "invalid log format(not int), log = "+str;
                            return false;
                        }
                    }
                    if(str.size() == 28){
                        std::string id = str.substr(0,4);
                        std::string stocks_code = str.substr(4,6);
                        int unique_num = atoi(str.substr(10,6).c_str());
                        int volume = atoi(str.substr(16,6).c_str());
                        int price = atoi(str.substr(22,6).c_str());
                        int cal = volume*price;
                        total[id] = (total[id]+cal);
                        range[price] = (range[price]+volume);
                        clients.insert(id);
                        if(unique_num >= 1){
                            u_nums.insert(unique_num);
                        }else{
                            BOOST_LOG_TRIVIAL(warning) << "unique number is less than 0, log = "+ str;
                        }
                    }

                }
            }

            /**
            for(SET::Set<std::string>::Iterator iter = clients.begin(); iter != clients.end(); ++iter){
                BOOST_LOG_TRIVIAL(trace) << *iter << ", total : " << total.get(*iter);
            }
             **/


            // 빈고유번호 찾기
            std::set<int>::iterator iter = u_nums.begin();
            int step = 1;
            for(; iter != u_nums.end(); ++iter, step++){
                int current = *iter;
                if(step != current){ // step 이랑 일치하지 않으면
                    while(step < current){ //빈 고유번호 담기
                        miss.push_back(step);
                        step++;
                    }
                }
            }

            /**
            for(int i = 0; i < miss.size(); i++){
                std::cout << miss[i] << " ";
            }
            std::cout << std::endl;
            **/
            return true;
        } catch (std::exception& e){
            BOOST_LOG_TRIVIAL(warning) << "empty directory , path : " + path+date;
            return false;
        }
    }

    std::map<int, uint64_t>& get_range(){return range;}; // -> std::map, 정렬되어있는상태
    std::map<std::string, uint64_t>& get_total(){return total;};
    std::vector<int>& get_miss(){return miss;};
    std::set<std::string>& get_clients(){return clients;};


public:
    inline static LoadData &get_instance(){
        static LoadData instance;
        return instance;
    }


    void set_path(const std::string path, const std::string date){
        // 특정 날짜 data 가져오기
        //path = "../datas/";
        //date = "2022-08-02";
        this->path = path;
        this->date = date;
        BOOST_LOG_TRIVIAL(trace) << this->path+this->date;
    }

};


#endif //QRAFT_EXCHANGE_API_LOADDATA_H
