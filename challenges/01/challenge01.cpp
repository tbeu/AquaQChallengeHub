// AquaQ Challenge Hub
// Challenge 1: Rose by any other name
// https://challenges.aquaq.co.uk/challenge/1

#include <cctype>
#include <fstream>
#include <iostream>
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

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    const auto len{lines[0].size()};
    const auto stride{(len + 2) / 3};
    for (uint16_t i = 0; i < 3; ++i) {
        for (uint16_t j = 0; j < 2; ++j) {
            if (const auto c{lines[0][i * stride + j]}; std::isxdigit(c)) {
                std::cout << c;
            } else {
                std::cout << '0';
            }
        }
    }

    std::cout << std::endl;

    return EXIT_SUCCESS;
}
