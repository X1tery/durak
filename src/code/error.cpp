#include <error.hpp>
#include <print>
#include <iostream>

void except(std::string msg, int exit_code) {
    std::println(std::cerr, "{}", msg);
    exit(exit_code);
}