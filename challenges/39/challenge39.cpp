// AquaQ Challenge Hub
// Challenge 39: Game of Throwns
// https://challenges.aquaq.co.uk/challenge/39

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

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    std::array<uint16_t, 2> counts{};
    size_t wins{};
    size_t sum{};
    uint8_t first{};
    uint8_t player{first};
    uint8_t count{};
    std::istringstream iss{lines[0]};
    uint16_t num;
    while (iss >> num) {
        counts[player] += num;
        if (counts[player] == 501) {
            count = 0;
            counts = {0, 0};
            sum += num;
            wins += player == 0 ? 1 : 0;
            first = !first;
            player = first;
        } else if (++count == 3) {
            count = 0;
            player = !player;
        }
    }

    std::cout << sum * wins << std::endl;

    return EXIT_SUCCESS;
}
