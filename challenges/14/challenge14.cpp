// AquaQ Challenge Hub
// Challenge 14: That's a bingo
// https://challenges.aquaq.co.uk/challenge/14

#include <algorithm>
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

constexpr uint16_t bingoMarker{0};
using Bingo = std::array<uint16_t, 25>;
constexpr Bingo bingoTemplate{6,  17, 34, 50, 68, 10, 21, 45, 53, 66, 5,  25, 36,
                              52, 69, 14, 30, 33, 54, 63, 15, 23, 41, 51, 62};

void play(Bingo& bingo, uint16_t num)
{
    if (auto it = std::find(bingo.begin(), bingo.end(), num); it != bingo.end()) {
        *it = bingoMarker;
    }
}

bool isBingo(const Bingo& bingo)
{
    auto _isBingo = [&bingo](const auto& check) -> bool {
        const auto [start, stride] = check;
        size_t pos{start};
        for (size_t i = 0; i < 5; ++i) {
            if (bingo[pos] != bingoMarker) {
                return false;
            }
            pos += stride;
        }
        return true;
    };
    constexpr std::array<std::pair<size_t, size_t>, 12> checks{
        {{0, 1}, {5, 1}, {10, 1}, {15, 1}, {20, 1}, {0, 5}, {1, 5}, {2, 5}, {3, 5}, {4, 5}, {0, 6}, {4, 4}}};
    return std::any_of(checks.cbegin(), checks.cend(), [&](const auto& c) { return _isBingo(c); });
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
        size_t run{0};
        Bingo bingo{bingoTemplate};
        std::istringstream iss{line};
        uint16_t num;
        while (iss >> num) {
            run++;
            play(bingo, num);
            if (isBingo(bingo)) {
                sum += run;
                break;
            }
        }
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
