// AquaQ Challenge Hub
// Challenge 22: Veni Vidi Vitavi
// https://challenges.aquaq.co.uk/challenge/22

#include <array>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include <gsl/util>

static bool readFile(const std::string& fileName, std::vector<std::string>& lines)
{
    std::ifstream in{fileName};
    if (!in) {
        std::cerr << "Cannot open file " << fileName << std::endl;
        return false;
    }
    auto closeStream = gsl::finally([&in] { in.close(); });
    std::string str;
    while (std::getline(in, str)) {
        lines.push_back(str);
    }
    return true;
}

std::string toRoman(uint16_t num)
{
    constexpr static std::array<std::pair<uint16_t, const char*>, 13> map{{{1000, "M"},
                                                                           {900, "CM"},
                                                                           {500, "D"},
                                                                           {400, "CD"},
                                                                           {100, "C"},
                                                                           {90, "XC"},
                                                                           {50, "L"},
                                                                           {40, "XL"},
                                                                           {10, "X"},
                                                                           {9, "IX"},
                                                                           {5, "V"},
                                                                           {4, "IV"},
                                                                           {1, "I"}}};

    std::ostringstream oss{};
    for (const auto& [div, digits] : map) {
        if (num < div) {
            continue;
        }
        const auto amt = num / div;
        for (uint16_t i = 0; i < amt; ++i) {
            oss << digits;
        }
        num -= amt * div;
    }
    return oss.str();
}

uint64_t encodeCaesar(const std::string& num)
{
    uint64_t code{};
    for (const auto digit : num) {
        code += digit - 'A' + 1;
    }
    return code;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    std::istringstream iss{lines[0]};
    uint16_t num;
    uint64_t sum{};
    while (iss >> num) {
        sum += encodeCaesar(toRoman(num));
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
