// AquaQ Challenge Hub
// Challenge 7: What is best in life?
// https://challenges.aquaq.co.uk/challenge/7

#include <fstream>
#include <iostream>
#include <map>
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

using Rating = double;
using Map = std::map<std::string, Rating>;

void updateRatings(Map& map, const std::string& winner, const std::string& loser)
{
    Rating rWinner{1200};
    if (auto it = map.find(winner); it != map.end()) {
        rWinner = it->second;
    }
    Rating rLoser{1200};
    if (auto it = map.find(loser); it != map.end()) {
        rLoser = it->second;
    }
    const double update{20 * (1 - (1. / (1 + std::pow(10, (rLoser - rWinner) / 400))))};
    map[winner] = rWinner + update;
    map[loser] = rLoser - update;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    Map map{};
    for (size_t i = 1; i < lines.size(); ++i) {
        const auto& line = lines[i];
        const auto pos1 = line.find(',', 0);
        const auto player1 = line.substr(0, pos1);
        const auto pos2 = line.find(',', pos1 + 1);
        const auto player2 = line.substr(pos1 + 1, pos2 - pos1 - 1);
        const auto pos3 = line.find('-', pos2 + 1);
        const auto points1 = std::stoi(line.substr(pos2 + 1, pos3 - pos2 - 1));
        const auto points2 = std::stoi(line.substr(pos3 + 1, line.size() - pos3 - 1));
        if (points1 < points2) {
            updateRatings(map, player2, player1);
        } else if (points1 > points2) {
            updateRatings(map, player1, player2);
        }
    }

    auto [min, max] =
        std::minmax_element(map.cbegin(), map.cend(), [](const auto& a, const auto& b) { return a.second < b.second; });
    std::cout << static_cast<uint32_t>(max->second) - static_cast<uint32_t>(min->second) << std::endl;

    return EXIT_SUCCESS;
}
