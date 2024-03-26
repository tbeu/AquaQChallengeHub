// AquaQ Challenge Hub
// Challenge 20: Blackjack
// https://challenges.aquaq.co.uk/challenge/20

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

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    uint16_t wins{};
    uint16_t sum{};
    uint16_t aces{};
    const auto reset = [&sum, &aces] {
        sum = 0;
        aces = 0;
    };
    for (size_t i = 0; i < lines[0].size(); i += 2) {
        const auto card = lines[0][i];
        if (card == 'J' || card == 'Q' || card == 'K') {
            sum += 10;
        } else if (card == '1') {
            sum += 10;
            ++i;
        } else if (card == 'A') {
            sum++;
            aces++;
        } else {
            sum += card - '0';
        }
        if (sum > 21) {
            reset();
            continue;
        }
        for (uint16_t i = 0; i <= aces; ++i) {
            if (sum + i * 10 == 21) {
                wins++;
                reset();
                break;
            }
        }
    }
    std::cout << wins << std::endl;

    return EXIT_SUCCESS;
}
