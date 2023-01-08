// AquaQ Challenge Hub
// Challenge 5: Snake eyes
// https://challenges.aquaq.co.uk/challenge/5

#include <array>
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

constexpr size_t front{0};
constexpr size_t left{1};
constexpr size_t top{2};
using Dice = std::array<uint16_t, 3>;
using Dices = std::array<Dice, 2>;

bool update(Dices& dices, char dir)
{
    for (auto& dice : dices) {
        const auto tmp{dice[front]};
        if (dir == 'L') {
            dice[front] = 7 - dice[left];
            dice[left] = tmp;
        } else if (dir == 'R') {
            dice[front] = dice[left];
            dice[left] = 7 - tmp;
        } else if (dir == 'U') {
            dice[front] = 7 - dice[top];
            dice[top] = tmp;
        } else if (dir == 'D') {
            dice[front] = dice[top];
            dice[top] = 7 - tmp;
        }
    }
    return dices[0][front] == dices[1][front];
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    Dices dices{{{1, 2, 3}, {1, 3, 2}}};
    size_t count{0};
    for (size_t i = 0; i < lines[0].size(); ++i) {
        if (update(dices, lines[0][i])) {
            count += i;
        }
    }
    std::cout << count << std::endl;

    return EXIT_SUCCESS;
}
