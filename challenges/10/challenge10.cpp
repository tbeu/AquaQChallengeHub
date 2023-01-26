// AquaQ Challenge Hub
// Challenge 10: Troll Toll
// https://challenges.aquaq.co.uk/challenge/10

#include <fstream>
#include <iostream>
#include <numeric>
#include <map>
#include <queue>
#include <set>
#include <string>
#include <unordered_map>
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

template <typename T>
struct Graph
{
    void addEdge(const T& s, const T& d, uint32_t c)
    {
        adjs[s].insert({d, c});
    }

    std::unordered_map<T, std::set<std::pair<T, uint32_t> > > adjs{};
};

Graph<std::string> g{};

uint32_t dijkstra(const std::string& start, const std::string& end)
{
    std::map<std::string, uint32_t> costs{};
    using Pair = std::pair<uint32_t, std::string>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair> > q{};
    q.push({0, start});
    costs[start] = 0;
    while (!q.empty()) {
        const auto [c, node] = q.top();
        if (node == end) {
            return c;
        }
        q.pop();
        for (const auto& [next, cNext] : g.adjs[node]) {
            if (auto it = costs.find(next); it == costs.end() || c + cNext < it->second) {
                costs[next] = c + cNext;
                q.push({c + cNext, next});
            }
        }
    }
    return UINT32_MAX;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 1; i < lines.size(); ++i) {
        const auto& line = lines[i];
        const auto pos1 = line.find(',', 0);
        const auto s = line.substr(0, pos1);
        const auto pos2 = line.find(',', pos1 + 1);
        const auto d = line.substr(pos1 + 1, pos2 - pos1 - 1);
        const auto c = static_cast<uint32_t>(std::stoi(line.substr(pos2 + 1, line.size() - pos2)));
        g.addEdge(s, d, c);
        g.addEdge(d, s, c);
    }

    std::cout << dijkstra("TUPAC", "DIDDY") << std::endl;

    return EXIT_SUCCESS;
}
