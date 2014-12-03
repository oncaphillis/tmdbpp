project(tmdbpp)

cmake_minimum_required(VERSION 2.8)
aux_source_directory(. SRC_LIST)


set(CMAKE_CXX_FLAGS "-std=c++11 ${CMAKE_CXX_FLAGS}")


INCLUDE_DIRECTORIES( "." )

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

target_link_libraries(tmdbquery curl curlpp)
