project(tmdbpp)

cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)


set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")

FIND_PACKAGE(CURL)
IF(CURL_FOUND)
  MESSAGE(STATUS "Curl libraries found at: ${CURL_LIBRARIES}")
  MESSAGE(STATUS "Curl includes found at: ${CURL_INCLUDE_DIRS}")
else()
  MESSAGE(SEND_ERROR "Could not find cURL on your system")
ENDIF(CURL_FOUND)

find_path(CURLPP_INCLUDE_DIRS "curlpp/Easy.hpp" HINTS "/usra/include/*"     )
find_library(CURLPP_LIBRARIES "curlpp" HINTS "/usr/lib/*" "/usr/lib64/*")

IF(CURLPP_INCLUDE_DIRS AND CURLPP_LIBRARIES)
 MESSAGE(STATUS "CurlPP libraries found at: ${CURLPP_LIBRARIES}")
 MESSAGE(STATUS "CurlPP includes  found at: ${CURLPP_INCLUDE_DIRS}")
ELSE()
 MESSAGE(SEND_ERROR "Could not find cURLPP on your system")
ENDIF(CURLPP_INCLUDE_DIRS AND CURLPP_LIBRARIES)

INCLUDE_DIRECTORIES( "."  ${CURLPP_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})

add_executable(tmdbquery
 main.cpp
 tmdbpp/search.cpp
 tmdbpp/get.cpp
 tmdbpp/api.cpp
 tmdbpp/search.h
 tmdbpp/get.h
 tmdbpp/api.h
 tmdbpp/apiagent.h
 tmdbpp/util.h
 tmdbpp/movie.h
 tmdbpp/movies.h
 tmdbpp/companies.h
 tmdbpp/movies.h
 tmdbpp/pagedcollection.h
)

target_link_libraries(tmdbquery ${CURL_LIBRARIES} ${CURLPP_LIBRARIES})
