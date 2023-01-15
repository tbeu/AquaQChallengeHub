// AquaQ Challenge Hub
// Challenge 26: Typo Theft
// https://challenges.aquaq.co.uk/challenge/26

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

uint64_t toNumber(std::string::const_iterator begin, std::string::const_iterator end)
{
    uint64_t num{};
    for (auto it = begin; it != end; ++it) {
        num *= 10;
        num += (*it - '0');
    }
    return num;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    uint64_t sum{};
    for (auto& line : lines) {
        if (line.size() < 2) {
            continue;
        }
        size_t len{line.size()};
        for (; len > 0; --len) {
            if (line[len] > line[len - 1]) {
                len--;
                break;
            }
        }
        const auto num = toNumber(line.cbegin() + len, line.cend());
        if (std::next_permutation(line.begin() + len, line.end())) {
            const auto next = toNumber(line.cbegin() + len, line.cend());
            sum += next - num;
        }
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
