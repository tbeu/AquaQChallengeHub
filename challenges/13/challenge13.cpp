// AquaQ Challenge Hub
// Challenge 13: O RLE?
// https://challenges.aquaq.co.uk/challenge/13

#include <algorithm>
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

    size_t sum{};
    for (const auto& line : lines) {
        size_t max{0};
        for (size_t len = 1; len < line.size(); ++len) {
            // Forward check - ignoring trailing extra characters, e.g., ABABABCD
            if (const auto it = std::mismatch(line.begin(), line.end(), line.begin() + len, line.end());
                it.second != line.begin() + len) {
                max = std::max(max, (it.second - line.begin()) / len);
            }
            // Reverse check - ignoring leading extra characters, e.g., CDABABAB
            if (const auto it = std::mismatch(line.rbegin(), line.rend(), line.rbegin() + len, line.rend());
                it.second != line.rbegin() + len) {
                max = std::max(max, (it.second - line.rbegin()) / len);
            }
        }
        sum += max;
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
