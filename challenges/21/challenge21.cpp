// AquaQ Challenge Hub
// Challenge 21: Clean Sweep
// https://challenges.aquaq.co.uk/challenge/21

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
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

constexpr size_t width{5};
constexpr size_t someMax{100};
using Tile = uint32_t;
using Row = std::vector<Tile>;
using Floor = std::vector<Row>;

size_t dijkstra(const Floor& floor)
{
    using Pos = std::array<size_t, 2>;
    using Pair = std::pair<size_t, Pos>;
    std::map<Pos, size_t> visited{};
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair> > q{};
    auto pushQ = [&](const Pos& next, size_t sumOld = 0) {
        size_t sum{sumOld};
        for (size_t i = 0; i < width; ++i) {
            sum += someMax - floor[next[0]][next[1] + i];
        }
        if (auto it = visited.find(next); it == visited.end() || sum < it->second) {
            visited[next] = sum;
            q.push({sum, next});
        }
    };
    for (size_t i = 0; i <= floor[0].size() - width; ++i) {
        pushQ({0, i});
    }
    while (!q.empty()) {
        const auto sum = q.top().first;
        auto pos = q.top().second;
        if (++pos[0] == floor.size()) {
            return someMax * width * floor.size() - sum;
        }
        q.pop();
        pushQ({pos[0], pos[1]}, sum);
        if (pos[1] > 0) {
            pushQ({pos[0], pos[1] - 1}, sum);
        }
        if (pos[1] < floor[pos[0]].size() - width) {
            pushQ({pos[0], pos[1] + 1}, sum);
        }
    }
    return 0;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    Floor floor{};
    floor.reserve(lines.size());
    for (const auto& line : lines) {
        std::istringstream iss{line};
        std::string num;
        Row row{};
        while (iss >> num) {
            row.push_back(std::stoul(num));
        }
        floor.emplace_back(row);
    }

    std::cout << dijkstra(floor) << std::endl;

    return EXIT_SUCCESS;
}
