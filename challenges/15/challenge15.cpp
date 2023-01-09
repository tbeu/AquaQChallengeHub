// AquaQ Challenge Hub
// Challenge 15: word wore more mare maze
// https://challenges.aquaq.co.uk/challenge/15

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <queue>
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

std::vector<std::string> words{};

size_t bfs(const std::string& start, const std::string& end)
{
    std::map<std::string, size_t> visited{};
    std::queue<std::string> q{};
    q.push(start);
    visited[start] = 1;
    while (!q.empty()) {
        const auto word = q.front();
        if (word == end) {
            break;
        }
        q.pop();
        const auto len = visited[word];
        for (size_t i = 0; i < word.size(); ++i) {
            for (uint16_t j = 0; j < 26; ++j) {
                std::string next{word};
                next[i] = 'a' + j;
                if (next[i] == word[i]) {
                    continue;
                }
                if (!std::binary_search(words.cbegin(), words.cend(), next)) {
                    continue;
                }
                if (auto it = visited.find(next); it == visited.end()) {
                    visited[next] = len + 1;
                    q.push(next);
                }
            }
        }
    }
    if (auto it = visited.find(end); it != visited.end()) {
        return it->second;
    }
    return SIZE_MAX;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc >= 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }
    words.reserve(172820);
    if (argc >= 3) {
        if (!readFile(argv[2], words)) {
            return EXIT_FAILURE;
        }
    }

    size_t prod{1};
    for (const auto& line : lines) {
        const auto pos = line.find(',', 0);
        prod *= bfs(line.substr(0, pos), line.substr(pos + 1, line.size() - pos));
    }
    std::cout << prod << std::endl;

    return EXIT_SUCCESS;
}
