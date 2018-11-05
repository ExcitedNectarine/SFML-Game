#pragma once

#include <iostream>

#define OUTPUT_ENABLED

// i use this because i can turn on and off console output by simply commenting out the line above
#ifdef OUTPUT_ENABLED
#define OUTPUT(output) (std::cout << output << std::endl)
#define OUTPUT_ERROR(output) (std::cerr << output << std::endl)
#else
#define OUTPUT
#define OUTPUT_ERROR
#endif