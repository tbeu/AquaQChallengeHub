// AquaQ Challenge Hub
// Challenge 4: This is good co-primen
// https://challenges.aquaq.co.uk/challenge/4

#include <fstream>
#include <iostream>
#include <numeric>
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

    uint32_t num;
    {
        std::istringstream iss{lines[0]};
        iss >> num;
    }

    uint64_t sum{};
    for (uint32_t i = 1; i < num; ++i) {
        if (i * num == std::lcm(i, num)) {
            sum += i;
        }
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
