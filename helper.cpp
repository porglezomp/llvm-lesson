#include "helper.h"
#include <iostream>

void fail(std::string message) {
    std::cerr << message << std::endl;
    exit(1);
}