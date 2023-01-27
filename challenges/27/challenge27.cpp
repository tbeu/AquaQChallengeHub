// AquaQ Challenge Hub
// Challenge 27: Snake Eater
// https://challenges.aquaq.co.uk/challenge/27

#include <array>
#include <fstream>
#include <iostream>
#include <map>
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

using Pos = std::array<int32_t, 2>;
using PosDirs = std::map<Pos, Pos>;

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    size_t len{lines[0].size()};
    {
        std::string border(len, ' ');
        lines.insert(lines.begin(), border);
        lines.emplace_back(border);
    }
    std::for_each(lines.begin(), lines.end(), [](auto& row) { row = " " + row + " "; });

    // Precalculate the possible head/tail positions of the word snakes
    PosDirs headOrTails{};
    constexpr std::array<Pos, 4> adjs{{{0, 1}, {1, 0}, {0, -1}, {-1, 0}}};
    for (int32_t r = 1; r < lines.size() - 1; ++r) {
        for (int32_t c = 1; c < lines[r].size() - 1; ++c) {
            if (lines[r][c] == ' ') {
                continue;
            }
            int32_t count{};
            for (const auto [rr, cc] : adjs) {
                if (lines[r + rr][c + cc] == ' ') {
                    count++;
                }
            }
            if (count == 3) {
                for (const auto [rr, cc] : adjs) {
                    if (lines[r + rr][c + cc] != ' ') {
                        headOrTails[{r, c}] = {rr, cc};
                        break;
                    }
                }
            }
        }
    }

    uint64_t sumSnake{};
    while (!headOrTails.empty()) {
        const auto& pos = headOrTails.begin()->first;
        const auto& dir = headOrTails.begin()->second;
        auto r = pos[0];
        auto c = pos[1];
        auto rr = dir[0];
        auto cc = dir[1];
        uint64_t sumWord{};
        uint16_t count{};
        const auto yieldLetter = [&]() {
            sumWord += lines[r][c] - 'a' + 1;
            count++;
        };
        const auto yieldWord = [&]() {
            yieldLetter();
            sumSnake += sumWord * count;
            count = 0;
            sumWord = 0;
        };
        do {
            yieldLetter();
            r += rr;
            c += cc;
            // Check for tail
            if (headOrTails.find({r, c}) != headOrTails.end()) {
                yieldWord();
                break;
            }
            // Check for word boundaries
            if (lines[r + !rr][c + !cc] != ' ') {
                rr = !rr;
                cc = !cc;
                yieldWord();
            } else if (lines[r - !rr][c - !cc] != ' ') {
                rr = -!rr;
                cc = -!cc;
                yieldWord();
            }
        } while (true);
        // Delete head and tail positions from snake
        headOrTails.erase(headOrTails.begin());
        headOrTails.erase({r, c});
    }
    std::cout << sumSnake << std::endl;

    return EXIT_SUCCESS;
}
