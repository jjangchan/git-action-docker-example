# AXE 벡엔드 엔지니어 샘플 태스크(장경찬)



## 시스템 환경 및 사용툴

- c++
- cmake
- server : cpprestSDK(Casablanca) 
- curl
- boost
- Google test
- CICD : github action 



## 코드 실행



```shell
$ cd /{code directory}/qraft-exchange-api/
$ mkdir build
$ cd build/
$ cmake ..
$ make 
```

서버 실행은 아래와 같습니다.

```shell
$ ./qraft_exchange_api
```

유닛 테스트 실행은 아래와 같습니다.

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
│       ├─ j-curl/CurlHandler.h : curl로 api을 요청하고 결과값을 받는 코드
│       └─ j-data-structure/HashMap.h, LinkerList.h, Set.h, Vector.h : 문제 1-4에 필요한 자료구조를 구현한 코드 
│
│
├─ .github/workflows/blank.yml : CI, CD를 수행하는 파일
│
└─ unittest
        ├─ GetPathTest.cpp : path가 잘 전달되는지 확인하는 테스트 코드
        └─ CICDTest.cpp : 문제2-1에 결과값이 잘 나오는지 확인하는 테스트 코드
        └─ FileFormat.cpp : 문제1-5 테스트 파일
```



## 추후 보완해야할점

1. ~~osx에서 작업을 하였는데 도커 이미지를 빌드하는 OS들은 리눅스 계열이라고 알고있습니다..따라서 cmake 패키지들에 경로,버전이 달라서 빌드 오류가 나므로 매크로를 걸어서 패키지 path, 버전 등을 호환할 수 있게 수정해야합니다. 코드 또한 Mac 이랑 유닉스 계열에 gcc가 차이가 있어서 컴파일 오류가 나므로 이점도 매크로를 걸어서 수정해야합니다.~~  

   
   
2. ~~문제 1-5에 edge case 파일 과 코드를 구현해야 합니다.~~



3. ~~2.2 문제에 docker 로그인 과정에서 오류가 발생해서 이 문제를 해결해야 합니다.~~
