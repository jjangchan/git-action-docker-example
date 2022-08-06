project(unittest)
add_subdirectory(lib)

if(APPLE)
    message(STATUS ">>> OS: APPLE")
elseif(LINUX OR UNIX)
    message(STATUS ">>> OS: LINUX OR UNIX")
    set(CMAKE_BUILD_TYPE Release)
elseif(WIN32)
    message(STATUS ">>> OS: WINDOWS")
endif()

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")

SET(INCDIR ../../include)
include_directories(${INCDIR})
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
link_directories(/usr/local/lib)
link_directories(/usr/local/lib64)

######################################################################
# Boost
set(Boost_USE_STATIC_LIBS        OFF)  # only find static libs
set(Boost_USE_DEBUG_LIBS         OFF) # ignore debug libs and
set(Boost_USE_RELEASE_LIBS       ON)  # only find release libs
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME    OFF)
#set(Boost_NO_BOOST_CMAKE         ON)
find_package(Boost
        1.79.0
        COMPONENTS
        program_options
        thread
        log_setup
        log
        system
        date_time
        filesystem
        regex
        chrono
        atomic
        exception
        random
        serialization
        REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
    set(Boost_LIB
            ${Boost_LIBRARIES}
            #        ${Boost_SYSTEM_LIBRARY}
            #        ${Boost_LOG_SETUP_LIBRARY}
            #        ${Boost_LOG_LIBRARY}
            )
    add_definitions(-DBOOST_LOG_DYN_LINK=1)
    message(STATUS "Boost Version: ${Boost_VERSION}")
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_LOG_LIBARY: ${Boost_LOG_LIBRARY}")
else()
    message(FATAL_ERROR "Could not find Boost")
endif()
######################################################################

if(APPLE)
    ######################################################################
    # Open SSL Package
    set(OpenSSL_INCLUDE /usr/local/opt/openssl/include)
    set(OpenSSL_LIBRARIES
            /usr/local/opt/openssl/lib/libcrypto.dylib
            /usr/local/opt/openssl/lib/libssl.dylib)
    include_directories(${OpenSSL_INCLUDE})
    ######################################################################

    ######################################################################
    # Microsoft RESTful API Package (Casablanca)
    set(CppREST_INCLUDE /usr/local/opt/cpprestsdk/include)
    set(CppREST_LIBRARIES /usr/local/opt/cpprestsdk/lib/libcpprest.dylib)
    include_directories(${CppREST_INCLUDE})
    ######################################################################
elseif(LINUX OR UNIX)
    find_package(cpprestsdk REQUIRED)
endif()

######################################################################
# CURL
find_package(CURL REQUIRED)
if(CURL_FOUND)
    include_directories(${CURL_INCLUDE_DIR})
    message(STATUS "CURL Version: ${CURL_VERSION_STRING}")
else()
    message(FATAL_ERROR "Could not find Curl")
endif()
######################################################################

######################################################################
# THREAD
set(CMAKE_THREAD_PREFER_PTHREAD TRUE)
set(THREADS_PREFER_PTHREAD_FLAG TRUE)
find_package(Threads REQUIRED)
######################################################################


# Compile and link
# Build the core library and executable
set(SOURCE_FILES
        GetPathTest.cpp
        CICDTest.cpp)
if(APPLE)
    set(LINK_LIBRARIES
            ${OpenSSL_LIBRARIES}
            ${CppREST_LIBRARIES}
            ${Boost_LIBRARIES}
            ${CURL_LIBRARIES}
            Threads::Threads
            gtest
            gtest_main
            )
elseif(LINUX OR UNIX)
    set(LINK_LIBRARIES
            cpprestsdk::cpprest
            ${Boost_LIBRARIES}
            ${CURL_LIBRARIES}
            Threads::Threads
            gtest
            gtest_main
            )
endif()

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${LINK_LIBRARIES})