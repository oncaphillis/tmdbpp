#include <tmdbpp/win_wget.h>
#include <iostream>

int main(int argc, char **argv) {
    tmdbpp::WGet::Result r = tmdbpp::WGet::instance().get("http://www.google.com/BLA");

    std::cerr << " H E L L O   W G E T" << std::endl
        << r.code << std::endl
        << " --------------------------- " << std::endl
        << r.code << std::endl;

}