// AquaQ Challenge Hub
// Challenge 37: GUESS WORDS
// https://challenges.aquaq.co.uk/challenge/37

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

constexpr bool verbose{false};
std::vector<std::string> dict{};

bool checkUnique(std::array<char, 5>& res, const std::map<char, std::set<size_t> >& wrong, const std::set<char>& bad,
                 const std::map<char, size_t>& occ)
{
    std::ostringstream oss{};
    for (const auto c : bad) {
        oss << c;
    }
    const auto bads = oss.str();

    std::string unique{};
    size_t count{};
    for (const auto& word : dict) {
        // Check for bad letters
        if (word.find_first_of(bads) != std::string::npos) {
            continue;
        }
        // Check for expected letters in correct positions
        bool mismatch{false};
        for (size_t i = 0; i < 5; ++i) {
            if (res[i] != '\0' && word[i] != res[i]) {
                mismatch = true;
                break;
            }
        }
        if (mismatch) {
            continue;
        }
        // Check for expected letters in wrong positions
        for (const auto& [c, poss] : wrong) {
            size_t i = 0;
            bool foundCorrect{false};
            do {
                const auto found = word.find(c, i);
                if (found == std::string::npos) {
                    mismatch = !foundCorrect;
                    break;
                } else {
                    if (poss.find(found) != poss.end()) {
                        mismatch = true;
                        break;
                    } else {
                        foundCorrect = true;
                        i = found + 1;
                    }
                }
            } while (!mismatch || i < 5);
            if (mismatch) {
                break;
            }
        }
        if (mismatch) {
            continue;
        }
        // Check for exact occurences
        for (const auto& [c, o] : occ) {
            if (static_cast<size_t>(std::count(word.cbegin(), word.cend(), c)) != o) {
                mismatch = true;
                break;
            }
        }
        if (mismatch) {
            continue;
        }
        if (++count > 1) {
            return false;
        }
        unique = word;
    }
    if (count == 1) {
        // Update result with found word
        for (size_t i = 0; i < 5; ++i) {
            res[i] = unique[i];
        }
        return true;
    }
    return false;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc >= 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }
    dict.reserve(172820);
    if (argc >= 3) {
        if (!readFile(argv[2], dict)) {
            return EXIT_FAILURE;
        }
    }
    dict.erase(std::remove_if(dict.begin(), dict.end(), [](const auto& word) { return word.size() != 5; }), dict.end());

    lines.erase(lines.begin());
    std::set<char> bad{};
    std::map<char, std::set<size_t> > wrong{};
    std::map<char, size_t> occ{};
    const auto updateWrong = [&wrong](char c, size_t pos) {
        if (auto it = wrong.find(c); it != wrong.end()) {
            it->second.insert(pos);
        } else {
            wrong[c] = std::set<size_t>{pos};
        }
    };
    uint64_t sum{};
    for (const auto& line : lines) {
        const auto word = line.substr(0, 5);
        std::array<char, 5> res{};
        for (size_t i = 0; i < 5; ++i) {
            const auto c = line[6 + 2 * i];
            if (c == '2') {
                res[i] = word[i];
            } else if (c == '1') {
                updateWrong(word[i], i);
            }
        }
        for (size_t i = 0; i < 5; ++i) {
            const auto c = line[6 + 2 * i];
            if (c == '0') {
                const auto foundInRes = std::find(res.cbegin(), res.cend(), word[i]) != res.end();
                const auto foundInWrong = wrong.find(word[i]) != wrong.end();
                if (!foundInRes && !foundInWrong) {
                    bad.insert(word[i]);
                } else if (foundInRes && !foundInWrong) {
                    for (size_t j = 0; j < 5; ++j) {
                        if (res[j] != word[i]) {
                            updateWrong(word[i], j);
                        }
                    }
                } else if (!foundInRes && foundInWrong) {
                    updateWrong(word[i], i);
                    const auto subWord = word.substr(0, i);
                    occ[word[i]] = std::count(subWord.cbegin(), subWord.cend(), word[i]);
                } else {
                    updateWrong(word[i], i);
                }
            }
        }
        if (checkUnique(res, wrong, bad, occ)) {
            if (verbose) {
                std::cout << word << " -> ";
                for (size_t i = 0; i < 5; ++i) {
                    std::cout << res[i];
                }
                std::cout << std::endl;
            }
            sum += std::accumulate(res.cbegin(), res.cend(), uint64_t{0},
                                   [](auto s, const auto c) { return s + c - 'a'; });
            wrong.clear();
            bad.clear();
            occ.clear();
        }
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
