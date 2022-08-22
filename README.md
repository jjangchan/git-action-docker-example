# c++ 서버 구축 및 CI/CD 예제

## 목적

- c++ Casablanca 로 rest 서버를 구축해서 요청하는 api를 응답하는 서버 구축
- git action으로 유닛 테스트가 통과하면 변경된 소스를 도커 이미지로 배포



## 시스템 환경 및 사용툴

- c++
- cmake
- server : cpprestSDK(Casablanca) 
- curl
- boost
- Google test
- CICD : github action 



## 실행 방법

### docker image pull and run

```shell
$ docker pull jjangchan/axe-test-web-server:6dcf8d48c5fe1541af58a83c19f2b04adccf1c1e
$ docker run -p 8091:8091 -it --name jjangchan-test-server jjangchan/axe-test-web-server:6dcf8d48c5fe1541af58a83c19f2b04adccf1c1e /bin/bash
```

### Build (camke and make)

도커 컨테이너 내부 접속후..

```shell
$ cd /source/
$ mkdir build
$ cd build
$ cmake ..
$ make
```

### 서버 실행

host : 0.0.0.0 , port : 8091

```shell
$ ./jjangchan_exchange_api
```



### 유닛 테스트 실행

```shell
$ cd unittest/
$ ./unittest
```



## 디렉터리 정보

```shell
/source./qraft-exchange-api
│ 
├─ main.cpp : 서버 가동 되는 메인함수
│
├─ Dockerfile : 도커 이미지 빌드 파일
│
├─ src
│    ├─ LoadData.h : 로그를 읽어오고 적합한 자료구조에 담아서 보관하는 코드
│    ├─ RestHandler.h : api별로 요청이 들어오면 작업을 수행하는 코드.     
│    └─ RestServer.h : 작업함수 등록, 서버시작, 서버중단을 하는 코드
│     
├─ include
│       ├─ j-boost-logger/BoostLogger.h : 로그를 핸들링 하는 코드
│       └─ j-curl/CurlHandler.h : curl로 api을 요청하고 결과값을 받는 코드
│   
│
├─ .github/workflows/blank.yml : CI, CD를 수행하는 파일
│
└─ unittest
        ├─ GetPathTest.cpp : path가 잘 전달되는지 확인하는 테스트 코드
        ├─ CICDTest.cpp
        └─ FileFormat.cpp
```



## 추후 보완해야할점
