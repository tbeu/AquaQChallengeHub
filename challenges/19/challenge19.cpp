// AquaQ Challenge Hub
// Challenge 19: It's alive
// https://challenges.aquaq.co.uk/challenge/19

#include <array>
#include <fstream>
#include <iostream>
#include <map>
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

using Map = std::vector<std::vector<bool> >;

size_t simulate(Map& map, size_t round)
{
    std::map<Map, size_t> cache{};
    bool foundCycle{false};
    constexpr std::array<std::array<int16_t, 2>, 4> adjs{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
    const size_t dim{map.size()};
    Map next{dim, std::vector<bool>(dim, false)};
    while (round-- > 0) {
        for (size_t r = 1; r < dim - 1; ++r) {
            for (size_t c = 1; c < dim - 1; ++c) {
                const auto on = std::count_if(adjs.cbegin(), adjs.cend(),
                                              [&](const auto& adj) { return map[r + adj[0]][c + adj[1]]; });
                next[r][c] = on % 2 == 1;
            }
        }
        map.swap(next);
        if (const auto it = cache.find(map); it != cache.end()) {
            const auto period = it->second - round;
            const auto cycles = round / period;
            round -= cycles * period;
            cache.clear();
            foundCycle = true;
        } else if (!foundCycle) {
            cache[map] = round;
        }
    }
    return std::accumulate(map.cbegin() + 1, map.cend() - 1, size_t{0}, [](auto s, const auto& row) {
        return s + std::count(row.cbegin() + 1, row.cend() - 1, true);
    });
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
        std::istringstream iss{line};
        size_t round, dim;
        iss >> round >> dim;
        Map map{dim + 2, std::vector<bool>(dim + 2, false)};
        uint16_t r, c;
        while (iss >> r >> c) {
            map[r + 1][c + 1] = true;
        }
        sum += simulate(map, round);
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
