// AquaQ Challenge Hub
// Challenge 3: Short walks
// https://challenges.aquaq.co.uk/challenge/3

#include <fstream>
#include <iostream>
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

    int16_t y{0}, x{2};
    int16_t sum{};
    for (const auto c : lines[0]) {
        int16_t yNew{y}, xNew{x};
        if (c == 'U') {
            yNew--;
        } else if (c == 'D') {
            yNew++;
        } else if (c == 'L') {
            xNew--;
        } else if (c == 'R') {
            xNew++;
        }
        if (xNew + yNew > 1 && xNew + yNew < 9 && std::abs(xNew - yNew) < 4) {
            y = yNew;
            x = xNew;
        }
        sum += y;
        sum += x;
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
