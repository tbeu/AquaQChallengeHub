// AquaQ Challenge Hub
// Challenge 29: On the up and up
// https://challenges.aquaq.co.uk/challenge/29

#include <algorithm>
#include <fstream>
#include <iostream>
#include <numeric>
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

bool isSorted(uint64_t num)
{
    bool isSorted{true};
    uint8_t second = num % 10;
    do {
        num /= 10;
        uint8_t first = num % 10;
        isSorted = first <= second;
        second = first;
    } while (isSorted && num > 9);
    return isSorted;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    size_t count{};
    uint64_t num = std::stoull(lines[0]);
    for (uint64_t i = 0; i <= num; ++i) {
        if (isSorted(i)) {
            count++;
        }
    }
    std::cout << count << std::endl;

    return EXIT_SUCCESS;
}
