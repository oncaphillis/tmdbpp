project(tmdbpp)

cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)

find_package(Boost)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif(Boost_FOUND)

IF(UNIX)
set(CMAKE_CXX_FLAGS "-ggdb -std=c++11 ${CMAKE_CXX_FLAGS}")

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

INCLUDE_DIRECTORIES( ${CURLPP_INCLUDE_DIRS} ${CURL_INCLUDE_DIRS})
ENDIF(UNIX)

IF(WIN32)
 SET(WINSRC tmdbpp/win_wget.cpp tmdbpp/win_wget.h )
ENDIF(WIN32)


INCLUDE_DIRECTORIES( "." )

add_executable(tmdbquery
 main.cpp
 tmdbpp/api.cpp
 tmdbpp/get.cpp
 tmdbpp/search.cpp
 tmdbpp/moviefilter.cpp
 tmdbpp/tvfilter.cpp
 tmdbpp/personfilter.cpp

 tmdbpp/tvfilter.h
 tmdbpp/api.h
 tmdbpp/apiagent.h
 tmdbpp/companies.h
 tmdbpp/credits.h
 tmdbpp/get.h
 tmdbpp/movie.h
 tmdbpp/moviefilter.h
 tmdbpp/movies.h
 tmdbpp/pagedcollection.h
 tmdbpp/person.h
 tmdbpp/personfilter.h
 tmdbpp/persons.h
 tmdbpp/search.h
 tmdbpp/tvfilter.h
 tmdbpp/tvseries.h
 tmdbpp/tvseriescollection.h
 tmdbpp/util.h
 ${WINSRC}
)




IF(WIN32)
 target_link_libraries(tmdbquery winhttp)
ENDIF(WIN32)

IF(UNIX)
 target_link_libraries(tmdbquery ${CURL_LIBRARIES} ${CURLPP_LIBRARIES})
ENDIF(UNIX)
