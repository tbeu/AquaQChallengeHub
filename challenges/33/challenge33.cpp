// AquaQ Challenge Hub
// Challenge 33: Bit of Bully
// https://challenges.aquaq.co.uk/challenge/33

#include <array>
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

constexpr static const std::array<uint8_t, 43> scores{1,  2,  3,  4,  5,  6,  7,  8,  9,  10, 11, 12, 13, 14, 15,
                                                      16, 17, 18, 19, 20, 21, 22, 24, 25, 26, 27, 28, 30, 32, 33,
                                                      34, 36, 38, 39, 40, 42, 45, 48, 50, 51, 54, 57, 60};

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    const uint64_t num = std::stoull(lines[0]);
    std::vector<uint32_t> counts(num + 1);
    for (uint32_t i = 1; i <= std::min<uint32_t>(static_cast<uint32_t>(num), 60); ++i) {
        if (std::find(scores.cbegin(), scores.cend(), i) != scores.end()) {
            counts[i] = 1;
        } else {
            counts[i] = 2;
        }
    }
    for (uint32_t i = 61; i <= num; ++i) {
        const auto rem = i % 60;
        const auto div = i / 60;
        if (rem == 0) {
            counts[i] = div;
        } else if (std::find(scores.cbegin(), scores.cend(), rem) != scores.end()) {
            counts[i] = div + 1;
        } else {
            counts[i] = div + 2;
            for (uint32_t j = 1; j <= i / 2; ++j) {
                counts[i] = std::min<uint32_t>(counts[i], counts[j] + counts[i - j]);
                if (counts[i] == div + 1) {
                    break;
                }
            }
        }
    }
    std::cout << std::accumulate(counts.cbegin() + 1, counts.cend(), uint64_t{0}) << std::endl;

    return EXIT_SUCCESS;
}
