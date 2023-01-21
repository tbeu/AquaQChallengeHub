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
using Grid = std::array<int, 16>;
using Grids = std::vector<Grid>;
using Pair = std::pair<int, int>;
using Pairs = std::vector<Pair>;

Grid toArray(std::string& line)
{
    std::for_each(line.begin(), line.end(), [](auto& c) {
        if (c == '*') {
            c = '0';
        }
    });
    Grid a;
    std::istringstream iss{line};
    iss.ignore(2);
    int val;
    for (size_t i = 0; i < 16; ++i) {
        iss >> val;
        a[i] = val;
    }
    return a;
}

bool isSolution(const Grid& grid, const Grid& input, const Pairs& guess)
{
    if (guess.size() != 8) {
        return false;
    }

    std::array<bool, 16> testGrid{};
    std::array<bool, 16> testInput{};
    for (const auto& [first, second] : guess) {
        for (size_t i = 0; i < grid.size(); ++i) {
            bool found{false};
            if (first + second == grid[i]) {
                for (size_t j = 0; j < grid.size(); ++j) {
                    if (first * second == grid[j] && !testGrid[i] && !testGrid[j] && i != j) {
                        testGrid[i] = true;
                        testGrid[j] = true;
                        found = true;
                        break;
                    }
                }
            }
            if (!found) {
                continue;
            }
            size_t iFirst = 16;
            for (size_t j = 0; j < input.size(); ++j) {
                if (first == input[j] && !testInput[j]) {
                    iFirst = j;
                    break;
                }
            }
            if (iFirst == 16) {
                continue;
            }
            for (size_t j = 0; j < input.size(); ++j) {
                if (second == input[j] && !testInput[j] && j != iFirst) {
                    testInput[iFirst] = true;
                    testInput[j] = true;
                    found = true;
                    break;
                }
            }
            if (found) {
                break;
            }
        }
    }
    return std::all_of(testGrid.cbegin(), testGrid.cend(), [](const auto b) { return b; }) &&
           std::all_of(testInput.cbegin(), testInput.cend(), [](const auto b) { return b; });
}

std::set<Grid> insertInput(const Grid& input, int n)
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

void print(const Grid& grid, const std::string& prefix)
{
    std::string sep{prefix};
    for (const auto n : grid) {
        std::cout << sep << n;
        sep = " ";
    }
    std::cout << std::endl;
}

void print(const Pairs& pairs)
{
    std::string sep{""};
    for (const auto [first, second] : pairs) {
        std::cout << sep << "(" << first << "," << second << ")";
        sep = ", ";
    }
    std::cout << std::endl;
}

size_t solve(const Grid& grid, const Grid& input, const Pairs& potential, const Pairs& guess,
             std::set<std::pair<Grid, Pairs> >& visitor, size_t start = 0)
{
    if (isSolution(grid, input, guess)) {
        if (verbose) {
            print(input, "i:");
            print(guess);
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
                if (const auto sum = solve(grid, nextInput, potential, nextGuess, visitor, i + 1); sum > 0) {
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
    return solve(grid, input, potential, guess, visitor);
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
    std::map<int, Pairs> factors{};
    for (int i = 1; i <= 400; ++i) {
        Pairs pairs{};
        for (int j = 1; j <= static_cast<int>(sqrt(i)); ++j) {
            if (i % j == 0) {
                pairs.push_back({j, i / j});
            }
        }
        factors[i] = pairs;
    }

    size_t sum{};
    for (size_t i = 0; i < lines.size(); i += 3) {
        const auto grid = toArray(lines[i]);
        const auto input = toArray(lines[i + 1]);

        if (verbose) {
            print(grid, "g:");
        }

        // Determine the potential pairs given the grid and the input
        Pairs potential{};
        for (const auto n : grid) {
            const auto& allMuls = factors[n];
            for (const auto& mul : allMuls) {
                if (std::find(grid.cbegin(), grid.cend(), mul.first + mul.second) != grid.end()) {
                    if (!insertInput(input, mul.first).empty() && !insertInput(input, mul.second).empty()) {
                        potential.push_back(mul);
                    }
                }
            }
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
