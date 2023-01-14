// AquaQ Challenge Hub
// Challenge 25: S'morse
// https://challenges.aquaq.co.uk/challenge/25

#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

#include <constexpr_hash_map/constexpr_hash_map.hpp>
#include <date/date.h>
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

using namespace std::chrono_literals;
using Data = std::vector<uint64_t>;

uint64_t toMilliSec(const std::string& time)
{
    std::istringstream iss{time};
    std::chrono::milliseconds ms;
    // Format: 11:33:49.982
    iss >> date::parse("%T", ms);
    return ms / 1ms;
}

std::string decode(Data& data)
{
    std::ostringstream out{};
    std::adjacent_difference(data.begin(), data.end(), data.begin());
    data.erase(data.begin());
    const auto period = *std::min_element(data.begin(), data.end());
    std::ostringstream code{};
    auto print = [&]() {
        static constexpr burda::ct::hash_map<26, const char*, char> morseAlphabet{
            std::make_pair(".-", 'a'),   std::make_pair("-...", 'b'), std::make_pair("-.-.", 'c'),
            std::make_pair("-..", 'd'),  std::make_pair(".", 'e'),    std::make_pair("..-.", 'f'),
            std::make_pair("--.", 'g'),  std::make_pair("....", 'h'), std::make_pair("..", 'i'),
            std::make_pair(".---", 'j'), std::make_pair("-.-", 'k'),  std::make_pair(".-..", 'l'),
            std::make_pair("--", 'm'),   std::make_pair("-.", 'n'),   std::make_pair("---", 'o'),
            std::make_pair(".--.", 'p'), std::make_pair("--.-", 'q'), std::make_pair(".-.", 'r'),
            std::make_pair("...", 's'),  std::make_pair("-", 't'),    std::make_pair("..-", 'u'),
            std::make_pair("...-", 'v'), std::make_pair(".--", 'w'),  std::make_pair("-..-", 'x'),
            std::make_pair("-.--", 'y'), std::make_pair("--..", 'z')};
        out << morseAlphabet[code.str().c_str()];
        code.str("");
        code.clear();
    };
    for (size_t i = 0; i < data.size(); ++i) {
        const auto len = data[i] / period;
        if (i % 2 == 0) {
            if (len == 1) {
                code << '.';
            } else if (len == 3) {
                code << '-';
            }
        } else {
            if (len == 3) {
                print();
            } else if (len == 7) {
                print();
                out << ' ';
            }
        }
    }
    print();
    return out.str();
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    Data data{};
    for (const auto& line : lines) {
        if (line.empty() || line[0] != ' ') {
            data.push_back(toMilliSec(line));
            continue;
        }
        std::cout << decode(data) << std::endl;
        data.clear();
    }

    return EXIT_SUCCESS;
}
