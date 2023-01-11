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
using Pos = std::array<size_t, 2>;

size_t dijkstra(const Floor& floor)
{
    std::map<Pos, size_t> visited{};
    auto cmp = [&visited](const auto& a, const auto& b) { return visited[a] > visited[b]; };
    std::priority_queue<Pos, std::vector<Pos>, decltype(cmp)> q(cmp);
    auto pushQ = [&](const Pos& next, size_t sumOld = 0) {
        size_t sum{sumOld};
        for (size_t i = 0; i < width; ++i) {
            sum += someMax - floor[next[0]][next[1] + i];
        }
        if (auto it = visited.find(next); it != visited.end()) {
            if (sum < it->second) {
                it->second = sum;
                q.push(next);
            }
        } else {
            visited[next] = sum;
            q.push(next);
        }
    };
    for (size_t i = 0; i <= floor[0].size() - width; ++i) {
        pushQ({0, i});
    }
    while (!q.empty()) {
        auto pos = q.top();
        const auto sum = visited[pos];
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
