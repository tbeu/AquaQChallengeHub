// AquaQ Challenge Hub
// Challenge 2: One is all you need
// https://challenges.aquaq.co.uk/challenge/2

#include <algorithm>
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

    std::vector<uint16_t> numbers{};
    {
        std::istringstream iss{lines[0]};
        uint16_t num;
        while (iss >> num) {
            numbers.push_back(num);
        }
    }

    uint16_t sum{};
    for (auto it = numbers.begin(); it != numbers.end(); ++it) {
        it = std::find_end(it, numbers.end(), it, it + 1);
        sum += *it;
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
