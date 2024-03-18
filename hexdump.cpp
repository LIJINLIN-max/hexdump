#include <cstdint>
#include <iostream>
#include <random>
#include <cctype>
#include <iomanip>
#include <ranges>
#include <vector>
#include <random>
#include <set>

template <class Range> requires std::ranges::range<Range>
void hexdump(Range const &range, uint32_t width)
{
    uint32_t addr = 0;

    for (auto chunk : range | std::views::chunk(width))
    {
        using T = std::ranges::range_value_t<Range>;
        std::cout << std::setw(8) << std::setfill('0') << std::hex << addr << ' ';
        for (auto c : chunk)
        {
            std::cout << ' ' << std::right << std::hex << std::setw(2) << std::setfill('0') << (unsigned long long)(std::make_unsigned_t<T>)c;
            ++addr;
        }
        if constexpr (sizeof(T) == 1 && std::convertible_to<T, char>)
        {
            if (addr % width < width)
            {
                for (size_t i = 0; i < (width - addr % width) * 3; ++i)
                {
                    std::cout << ' ';
                }
            }
            std::cout << "   |";
            for (auto c0 : chunk)
            {
                char c = static_cast<char>(c0);
                if (!std::isprint(c))
                {
                    c = '.';
                }
                std::cout << c;
            }
            std::cout << "| \n";
        }
    }
}

int main(int argc, char const *argv[])
{
    std::set<uint8_t> s;
    std::mt19937 gen(3);
    std::uniform_int_distribution<uint16_t> dis;
    for (int i = 0; i < 100; ++i)
    {
        s.insert(dis(gen));
    }
    hexdump(s, 16);
    return 0;
}
