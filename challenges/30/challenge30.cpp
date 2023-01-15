// AquaQ Challenge Hub
// Challenge 30: Flip Out
// https://challenges.aquaq.co.uk/challenge/30

#include <algorithm>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
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

constexpr bool verbose{true};
constexpr static const char zro{'0'};
constexpr static const char one{'1'};
static std::map<std::string, bool> cache{};

inline void toggle(std::string& line, size_t pos)
{
    if (zro == line[pos]) {
        line[pos] = one;
    } else if (one == line[pos]) {
        line[pos] = zro;
    }
}

bool flipSplit(const std::string& line, size_t pos);

bool flip(const std::string& line)
{
    constexpr static const std::string_view ones{"1"};
    if (line.empty() || line.compare(ones) == 0) {
        return true;
    }

    if (const auto it = cache.find(line); it != cache.end()) {
        return it->second;
    }

    auto it = std::find_first_of(line.begin(), line.end(), ones.cbegin(), ones.cend());
    if (it == line.end()) {
        cache[line] = false;
        return false;
    }

    bool found;
    do {
        found = flipSplit(line, it - line.begin());
        it = std::find_first_of(it + 1, line.end(), ones.cbegin(), ones.cend());
    } while (it != line.end() && !found);
    cache[line] = found;

    return found;
}

bool flipSplit(const std::string& line, size_t pos)
{
    auto first = line.substr(0, pos);
    if (pos > 0) {
        toggle(first, pos - 1);
    }
    auto second = line.substr(pos + 1, line.size() - pos - 1);
    if (pos < line.size() - 1) {
        toggle(second, 0);
    }
    return flip(first) && flip(second);
}

size_t play(const std::string& line)
{
    size_t count{};
    bool first{true};
    for (size_t pos = 0; pos < line.size(); ++pos) {
        if (one != line[pos]) {
            continue;
        }
        if (!flipSplit(line, pos)) {
            continue;
        }
        count++;
        if (verbose) {
            if (first) {
                std::cout << pos;
                first = false;
            } else {
                std::cout << ", " << pos;
            }
        }
    }
    cache[line] = count > 0;

    return count;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    size_t count{};
    for (size_t i = 0; i < lines.size(); ++i) {
        if (verbose) {
            std::cout << i << ": ";
        }
        count += play(lines[i]);
        if (verbose) {
            std::cout << std::endl;
        }
    }
    if (verbose) {
        auto validCount = std::count_if(cache.cbegin(), cache.cend(), [](const auto& p) { return p.second; });
        std::cout << "cache size: " << cache.size() << ", valid: " << validCount
                  << ", invalid: " << cache.size() - validCount << std::endl;
    }
    std::cout << count << std::endl;

    return EXIT_SUCCESS;
}
