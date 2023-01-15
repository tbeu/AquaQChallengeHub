// AquaQ Challenge Hub
// Challenge 23: Fair Play
// https://challenges.aquaq.co.uk/challenge/23

#include <array>
#include <fstream>
#include <iostream>
#include <numeric>
#include <set>
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

    std::array<char, 25> grid{};
    {
        constexpr std::string_view keyWord{"power plant"};
        size_t pos{};
        std::set<char> letters{};
        for (const auto k : keyWord) {
            if (letters.find(k) != letters.end() || k == ' ') {
                continue;
            }
            letters.insert(k);
            grid[pos++] = k;
        }
        for (uint8_t i = 0; i < 26; ++i) {
            char k = 'a' + i;
            if (letters.find(k) != letters.end() || k == 'j') {
                continue;
            }
            grid[pos++] = k;
        }
    }

    std::ostringstream oss{};
    for (size_t i = 0; i < lines[0].size(); i += 2) {
        const auto p1 = std::find(grid.cbegin(), grid.cend(), lines[0][i]) - grid.begin();
        const auto p2 = std::find(grid.cbegin(), grid.cend(), lines[0][i + 1]) - grid.begin();
        const auto r1 = p1 / 5;
        const auto c1 = p1 % 5;
        const auto r2 = p2 / 5;
        const auto c2 = p2 % 5;
        if (r1 == r2) {
            oss << grid[r1 * 5 + (c1 + 4) % 5] << grid[r1 * 5 + (c2 + 4) % 5];
        } else if (c1 == c2) {
            oss << grid[(p1 + 20) % 25] << grid[(p2 + 20) % 25];
        } else {
            oss << grid[r1 * 5 + c2] << grid[r2 * 5 + c1];
        }
    }
    std::cout << oss.str() << std::endl;

    return EXIT_SUCCESS;
}
