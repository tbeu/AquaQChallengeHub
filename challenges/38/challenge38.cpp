// AquaQ Challenge Hub
// Challenge 38: Number Neighbours
// https://challenges.aquaq.co.uk/challenge/38

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

    uint64_t sum{};
    for (const auto& line : lines) {
        std::vector<uint32_t> numbers;
        {
            std::istringstream iss{line};
            uint32_t n;
            while (iss >> n) {
                numbers.push_back(n);
            }
        }
        const auto len = numbers.size();
        sum += len;
        for (size_t i = 0; i < len; ++i) {
            // For each number
            for (size_t j = 1; j < len; ++j) {
                // For each streak size
                const size_t start = j < i ? i - j : 0;
                const size_t end = i + j + 1 > len ? len - j : i + 1;
                bool found{false};
                for (size_t k = start; k < end; ++k) {
                    if (const auto s = std::accumulate(numbers.cbegin() + k, numbers.cbegin() + k + j + 1, size_t{0});
                        s % (j + 1) == 0) {
                        sum++;
                        found = true;
                        break;
                    }
                }
                if (!found) {
                    break;
                }
            }
        }
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
