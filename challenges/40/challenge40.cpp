// AquaQ Challenge Hub
// Challenge 40: Prominence promenade
// https://challenges.aquaq.co.uk/challenge/40

#include <array>
#include <fstream>
#include <iostream>
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

    std::vector<uint16_t> values{0};
    {
        std::istringstream iss{lines[0]};
        uint16_t num;
        while (iss >> num) {
            values.push_back(num);
        }
        values.push_back(0);
    }

    std::vector<size_t> peaks;
    uint16_t max{0};
    for (size_t i = 1; i < values.size() - 1; ++i) {
        if (values[i - 1] < values[i] && values[i + 1] < values[i]) {
            peaks.push_back(i);
            max = std::max(max, values[i]);
        }
    }

    uint16_t sumPromincence{0};
    for (size_t i = 0; i < peaks.size(); ++i) {
        const auto val = values[peaks[i]];
        if (val == max) {
            sumPromincence += max;
            continue;
        }
        uint16_t minL{max};
        for (size_t j = peaks[i] - 1; j > 0; --j) {
            if (values[j] >= val) {
                break;
            }
            minL = std::min(minL, values[j]);
        }
        uint16_t minR{max};
        for (size_t j = peaks[i] + 1; j < values.size() - 1; ++j) {
            if (values[j] >= val) {
                break;
            }
            minR = std::min(minR, values[j]);
        }
        sumPromincence += std::min(val - minL, val - minR);
    }

    std::cout << sumPromincence << std::endl;

    return EXIT_SUCCESS;
}
