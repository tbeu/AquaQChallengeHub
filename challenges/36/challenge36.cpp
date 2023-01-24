// AquaQ Challenge Hub
// Challenge 36: Tetonor Terror
// https://challenges.aquaq.co.uk/challenge/36

#include <array>
#include <fstream>
#include <iostream>
#include <map>
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

constexpr bool verbose{true};
using Grid = std::array<uint16_t, 16>;
using Pair = std::pair<uint16_t, uint16_t>;
using Pairs = std::vector<Pair>;
using OccMap = std::map<uint16_t, uint8_t>;

std::ostream& operator<<(std::ostream& stream, const Grid& grid)
{
    std::string sep{""};
    for (const auto n : grid) {
        stream << sep << n;
        sep = " ";
    }
    return stream;
}

std::ostream& operator<<(std::ostream& stream, const Pairs& pairs)
{
    std::string sep{""};
    for (const auto [first, second] : pairs) {
        stream << sep << "(" << first << "," << second << ")";
        sep = ", ";
    }
    return stream;
}

Grid toArray(std::string& line)
{
    std::for_each(line.begin(), line.end(), [](auto& c) {
        if (c == '*') {
            c = '0';
        }
    });
    Grid grid;
    std::istringstream iss{line};
    iss.ignore(2);
    for (size_t i = 0; i < grid.size(); ++i) {
        iss >> grid[i];
    }
    return grid;
}

OccMap toOccMap(const Grid& grid)
{
    OccMap occ;
    for (const auto n : grid) {
        if (auto it = occ.find(n); it != occ.end()) {
            it->second++;
        } else {
            occ[n] = 1;
        }
    }
    return occ;
}

bool isSolution(const OccMap& gridOcc, const Grid& input, const Pairs& guess)
{
    if (guess.size() != 8) {
        return false;
    }

    Grid guessGrid{};
    for (size_t i = 0; i < 8; ++i) {
        guessGrid[2 * i] = guess[i].first + guess[i].second;
        guessGrid[2 * i + 1] = guess[i].first * guess[i].second;
    }
    if (toOccMap(guessGrid) != gridOcc) {
        return false;
    }

    for (size_t i = 0; i < 8; ++i) {
        guessGrid[2 * i] = guess[i].first;
        guessGrid[2 * i + 1] = guess[i].second;
    }
    return toOccMap(guessGrid) == toOccMap(input);
}

std::set<Grid> insertInput(const Grid& input, uint16_t n)
{
    std::set<Grid> inputs{};
    size_t imin{0};
    size_t imax{16};
    for (size_t i = input.size(); i > 0; --i) {
        if (input[i - 1] != 0 && input[i - 1] < n) {
            imin = i;
            break;
        }
    }
    for (size_t i = 0; i < input.size(); ++i) {
        if (input[i] > n) {
            imax = i;
            break;
        }
    }
    for (size_t i = imin; i < imax; ++i) {
        auto nextInput{input};
        nextInput[i] = n;
        inputs.insert(nextInput);
    }
    return inputs;
}

size_t solve(const OccMap& gridOcc, const Grid& input, const Pairs& potential, const Pairs& guess,
             std::set<std::pair<Grid, Pairs> >& visitor, size_t start = 0)
{
    if (isSolution(gridOcc, input, guess)) {
        if (verbose) {
            std::cout << "i:" << input << '\n' << guess << std::endl;
        }
        size_t sum{};
        for (const auto [first, second] : guess) {
            sum += std::abs(first - second);
        }
        return sum;
    }

    const auto depth = guess.size();
    if (depth == 8) {
        return 0;
    }

    visitor.insert({input, guess});
    for (size_t i = start; i < potential.size() - 7 + depth; ++i) {
        auto nextGuess{guess};
        nextGuess.push_back(potential[i]);
        std::sort(nextGuess.begin(), nextGuess.end());
        const auto nextInputs1 = insertInput(input, potential[i].first);
        for (const auto& nextInput1 : nextInputs1) {
            const auto nextInputs = insertInput(nextInput1, potential[i].second);
            for (const auto& nextInput : nextInputs) {
                if (visitor.find({nextInput, nextGuess}) != visitor.end()) {
                    continue;
                }
                if (const auto sum = solve(gridOcc, nextInput, potential, nextGuess, visitor, i + 1); sum > 0) {
                    return sum;
                }
            }
        }
    }
    return 0;
}

size_t solve(const Grid& grid, const Grid& input, const Pairs& potential)
{
    Pairs guess{};
    std::set<std::pair<Grid, Pairs> > visitor{};
    return solve(toOccMap(grid), input, potential, guess, visitor);
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    // Precalculate the potential factorizations
    std::map<uint16_t, Pairs> factorizations{};
    for (uint16_t i = 1; i <= 400; ++i) {
        Pairs pairs{{{1, i}}};
        for (uint16_t j = 2; j <= static_cast<uint16_t>(sqrt(i)); ++j) {
            if (i % j == 0) {
                pairs.push_back({j, i / j});
            }
        }
        factorizations[i] = pairs;
    }

    size_t sum{};
    for (size_t i = 0; i < lines.size(); i += 3) {
        const auto grid = toArray(lines[i]);
        const auto input = toArray(lines[i + 1]);

        // Determine the potential pairs given the grid and the input
        Pairs potential{};
        for (const auto n : grid) {
            const auto& factors = factorizations[n];
            for (const auto& f : factors) {
                if (std::find(grid.cbegin(), grid.cend(), f.first + f.second) != grid.end() &&
                    !insertInput(input, f.first).empty() && !insertInput(input, f.second).empty()) {
                    potential.push_back(f);
                }
            }
        }

        if (verbose) {
            std::cout << "g:" << grid << std::endl;
        }

        if (potential.size() < 8) {
            continue;
        }

        sum += solve(grid, input, potential);

        if (verbose) {
            std::cout << std::endl;
        }
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
