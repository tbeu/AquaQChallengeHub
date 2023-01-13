// AquaQ Challenge Hub
// Challenge 16: Keming
// https://challenges.aquaq.co.uk/challenge/16

#include <array>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
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

constexpr size_t height{6};
using Pos = std::array<uint8_t, 2>;
using Letter = std::vector<Pos>;
using Alphabet = std::vector<std::pair<Letter, uint8_t> >;

Alphabet toAlphabet(std::vector<std::string>&& lines)
{
    Alphabet letters{};
    for (size_t i = 0; i < lines.size(); i += height) {
        Letter letter{};
        uint8_t width{};
        for (uint8_t j = 0; j < height; ++j) {
            const auto& line = lines[i + j];
            width = std::max<uint8_t>(width, static_cast<uint8_t>(line.size()));
            for (uint8_t k = 0; k < line.size(); ++k) {
                if (line[k] == '#') {
                    letter.push_back({j, k});
                }
            }
        }
        letters.push_back({letter, width});
    }
    lines.clear();
    return letters;
}

std::vector<uint8_t> calcSpaces(const Alphabet& letters)
{
    std::vector<uint8_t> spaces{};
    std::transform(letters.cbegin(), letters.cend(), std::back_inserter(spaces), [](const auto& letterPair) {
        const auto& [letter, width] = letterPair;
        return static_cast<uint8_t>(width * height - letter.size());
    });
    return spaces;
}

std::map<Pos, uint8_t> calcKerningTable(const Alphabet& letters)
{
    std::map<Pos, uint8_t> kerningTable{};
    for (uint8_t i = 0; i < letters.size(); ++i) {
        const auto& [first, width] = letters[i];
        for (uint8_t j = 0; j < letters.size(); ++j) {
            auto second = letters[j].first;
            std::for_each(second.begin(), second.end(), [&](auto& pos) { pos[1] += width - 1; });
            uint8_t kerning{};
            while (std::find_first_of(first.begin(), first.end(), second.begin(), second.end()) == first.end()) {
                std::for_each(second.begin(), second.end(), [](auto& pos) { pos[1]--; });
                kerning += height;
            }
            kerningTable.emplace_hint(kerningTable.end(), Pos{i, j}, kerning);
        }
    }
    return kerningTable;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc >= 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    std::vector<std::string> alphabet{};
    if (argc == 3) {
        if (!readFile(argv[2], alphabet)) {
            return EXIT_FAILURE;
        }
    }

    const auto letters = toAlphabet(std::move(alphabet));
    // Precalculate the letter spaces
    const auto spaces = calcSpaces(letters);
    // Precalculate the kerning table
    auto kerningTable = calcKerningTable(letters);

    const auto& line = lines[0];
    // First, set default space (of height) between letter pairs
    size_t count{height * (line.size() - 1)};
    // Second, add regular spaces w/o kerning
    count += std::accumulate(line.cbegin(), line.cend(), size_t{0},
                             [&spaces](auto s, const auto c) { return s + spaces[c - 'A']; });
    // Third, apply kerning for the letter pairs
    for (size_t i = 0; i < line.size() - 1; ++i) {
        count -= kerningTable[{static_cast<uint16_t>(line[i] - 'A'), static_cast<uint16_t>(line[i + 1] - 'A')}];
    }
    std::cout << count << std::endl;

    return EXIT_SUCCESS;
}
