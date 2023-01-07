// AquaQ Challenge Hub
// Challenge 9: Big Data?
// https://challenges.aquaq.co.uk/challenge/9

#include <fstream>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

#include <gsl/util>
#include <InfInt.h>

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

    auto prod = std::accumulate(lines.cbegin(), lines.cend(), InfInt{1}, std::multiplies<InfInt>());
    std::cout << prod << std::endl;

    return EXIT_SUCCESS;
}
