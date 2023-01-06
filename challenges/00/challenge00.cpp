// AquaQ Challenge Hub
// Challenge 0: What's a numpad?
// https://challenges.aquaq.co.uk/challenge/0

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

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    constexpr std::array<uint16_t, 8> offsets{0, 3, 6, 9, 12, 15, 19, 22};

    for (const auto& line : lines) {
        uint16_t num, press;
        std::istringstream iss{line};
        iss >> num >> press;
        if (num == 0 && press == 1) {
            std::cout << ' ';
        } else if (num > 1 && num < 10) {
            const char letter = 'a' + offsets[num - 2] + press - 1;
            std::cout << letter;
        }
    }
    std::cout << std::endl;

    return EXIT_SUCCESS;
}
