// AquaQ Challenge Hub
// Challenge 12: A Day In The Lift
// https://challenges.aquaq.co.uk/challenge/12

#include <fstream>
#include <iostream>
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

using Floor = std::pair<bool, int>;

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    std::vector<Floor> lift{};
    for (const auto& line : lines) {
        std::istringstream iss{line};
        bool dir;
        int inc;
        iss >> dir >> inc;
        lift.push_back({dir, inc});
    }

    size_t count{1};
    size_t floor{0};
    bool up{true};
    do {
        const auto [dir, inc] = lift[floor];
        if (!dir) {
            up = !up;
        }
        floor += up ? inc : -inc;
        count++;
    } while (floor < lift.size());
    std::cout << count << std::endl;

    return EXIT_SUCCESS;
}
