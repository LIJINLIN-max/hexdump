#include <cstdint>
#include <iostream>
#include <random>
#include <cctype>
#include <iomanip>
#include <ranges>
void hexdump (std::string const & s) {
    uint32_t addr = 0;
    
    for (auto chunk: s | std::views::chunk(8))
}

int main(int argc, char const *argv[])
{
    std::string s = "hello world sdad";
    hexdump(s);
    return 0;
}
