// AquaQ Challenge Hub
// Challenge 35: Columns
// https://challenges.aquaq.co.uk/challenge/35

#include <cctype>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
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

std::vector<std::string> dict{};
std::map<std::vector<size_t>, std::pair<std::vector<std::string>, size_t> > cache{};

std::vector<size_t> colIdx(const std::string& key)
{
    const auto keyLen = key.size();
    std::vector<size_t> idxSort(keyLen);
    std::iota(idxSort.begin(), idxSort.end(), 0);
    std::sort(idxSort.begin(), idxSort.end(), [&key](const auto a, const auto b) { return key[a] < key[b]; });
    std::vector<size_t> colIdx(keyLen);
    for (size_t i = 0; i < keyLen; ++i) {
        colIdx[i] = std::find(idxSort.cbegin(), idxSort.cend(), i) - idxSort.begin();
    }
    return colIdx;
}

std::vector<std::string> decode(const std::string& line, std::vector<size_t> colIdx)
{
    const auto keyLen = colIdx.size();
    const auto colLen = line.size() / keyLen;
    std::vector<std::string> words{};
    std::ostringstream word{};
    for (size_t i = 0; i < colLen; ++i) {
        for (auto j : colIdx) {
            if (const auto c = line[j * colLen + i]; isalpha(c)) {
                word << c;
            } else {
                if (const auto& next = word.str(); !next.empty()) {
                    words.push_back(next);
                }
                word.str("");
                word.clear();
            }
        }
    }
    if (const auto& next = word.str(); !next.empty()) {
        words.push_back(next);
    }
    return words;
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

    for (const auto& key : dict) {
        if (key.size() == 1) {
            continue;
        }
        const auto idx = colIdx(key);
        if (auto it = cache.find(idx); it != cache.end()) {
            it->second.first.push_back(key);
        } else {
            auto words = decode(lines[0], idx);
            // Convert to lower case and count how many are found in the given dictionary
            std::for_each(words.begin(), words.end(),
                          [](auto& word) { std::transform(word.begin(), word.end(), word.begin(), std::tolower); });
            const size_t count = std::count_if(words.cbegin(), words.cend(), [&](const auto& word) {
                return std::binary_search(dict.cbegin(), dict.cend(), word);
            });
            // Could also use a relative confidence measure instead of the absolute number
            cache[idx] = std::make_pair(std::vector<std::string>{key}, count);
        }
    }

    const auto it = std::max_element(cache.cbegin(), cache.cend(),
                                     [](const auto& a, const auto& b) { return a.second.second < b.second.second; });
    const auto& keys = it->second.first;
    for (const auto& key : keys) {
        std::cout << key << std::endl;
    }

    return EXIT_SUCCESS;
}
