// AquaQ Challenge Hub
// Challenge 8: Cron Flakes
// https://challenges.aquaq.co.uk/challenge/8

#include <array>
#include <fstream>
#include <iostream>
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

using StateMlk = std::array<int, 5>;
using StateCrl = std::array<int, 1>;

StateMlk mlk{0, 0, 0, 0, 0};
StateCrl crl{0};

void useMlkAndCrl()
{
    if (auto it = std::find_if(mlk.begin(), mlk.end(), [](const int amt) { return amt >= 100; });
        it != mlk.end() && crl[0] >= 100) {
        *it -= 100;
        crl[0] -= 100;
    }
    std::rotate(mlk.begin(), mlk.begin() + 1, mlk.end());
    mlk[4] = 0;
}

void addMlk(int amt)
{
    mlk[4] += amt;
}

void addCrl(int amt)
{
    crl[0] += amt;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    for (size_t i = 1; i < lines.size(); ++i) {
        const auto& line = lines[i];
        const auto pos = line.find(',', 11);
        addCrl(std::stoi(line.substr(pos + 1, line.size() - pos)));
        useMlkAndCrl();
        addMlk(std::stoi(line.substr(11, pos - 11)));
    }
    std::cout << std::accumulate(mlk.begin(), mlk.end(), 0) + std::accumulate(crl.begin(), crl.end(), 0) << std::endl;

    return EXIT_SUCCESS;
}
