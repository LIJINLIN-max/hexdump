#include <cstdint>
#include <iostream>
#include <random>
#include <cctype>
#include <iomanip>
#include <ranges>
#include <vector>
#include <random>
#include <set>
#include <fstream>
#include <cxxopts.hpp>
#include <fstream>

template <class Range>
    requires std::ranges::range<Range>
void hexdump(Range const &range, uint32_t width)
{
    uint32_t addr = 0;
    std::vector<char> saved;
    for (auto chunk : range | std::views::chunk(width))
    {
        using T = std::ranges::range_value_t<Range>;
        std::cout << std::setw(8) << std::setfill('0') << std::hex << addr << ' ';
        for (auto c : chunk)
        {
            std::cout << ' ' << std::right << std::hex << std::setw(2) << std::setfill('0') << (unsigned long long)(std::make_unsigned_t<T>)c;
            ++addr;
            if constexpr (sizeof(T) == sizeof(char) && std::convertible_to<T, char>)
            {
                saved.push_back(static_cast<char>(c));
            }
        }
        if constexpr (sizeof(T) == sizeof(char) && std::convertible_to<T, char>)
        {
            if (addr % width != 0)
            {
                for (std::size_t i = 0; i < (width - addr % width) * 3; i++)
                {
                    std::cout << ' ';
                }
            }
            std::cout << "  |";
            for (auto c : saved)
            {
                if (!std::isprint(c))
                {
                    c = '.';
                }
                std::cout << c;
            }
            std::cout << "|";
            saved.clear();
        }
        std::cout << '\n';
    }
}

struct IstreamRange
{
    std::istreambuf_iterator<char> b, e;

    IstreamRange(std::istream &is)
        : b(std::istreambuf_iterator<char>(is)), e(std::istreambuf_iterator<char>()) {}

    auto begin() const
    {
        return b;
    }

    auto end() const
    {
        return e;
    }
};

int main(int argc, char **argv)
{
    cxxopts::Options options("hexdump", "A command line utility for printing the hexadecimal and ASCII representation of a file.");
    options.add_options()("f,file", "Input file", cxxopts::value<std::string>()->default_value("-"));
    options.add_options()("w,width", "Number of bytes per line", cxxopts::value<std::size_t>()->default_value("16"));
    // add help
    options.add_options()("h,help", "Print usage");
    auto args = options.parse(argc, argv);
    if (args.count("help"))
    {
        std::cout << options.help() << '\n';
        return 0;
    }
    auto path = args["file"].as<std::string>();
    auto width = args["width"].as<std::size_t>();
    if (path == "-")
    {
        hexdump(IstreamRange(std::cin), width);
        return 0;
    }
    std::ifstream ifs(path);
    if (!ifs.good())
    {
        std::cerr << std::strerror(errno) << ": " << path << '\n';
        return errno;
    }
    hexdump(IstreamRange(ifs), width);
    return 0;
}