// AquaQ Challenge Hub
// Challenge 11: Boxed In
// https://challenges.aquaq.co.uk/challenge/11

#include <array>
#include <fstream>
#include <iostream>
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

using Pos = uint16_t;
using Box = std::vector<Pos>;
using Boxes = std::vector<Box>;

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    lines.erase(lines.begin());
    Boxes boxes{};
    for (const auto& line : lines) {
        std::istringstream iss{line};
        uint16_t lx, ly, ux, uy;
        iss >> lx;
        iss.ignore(1);
        iss >> ly;
        iss.ignore(1);
        iss >> ux;
        iss.ignore(1);
        iss >> uy;
        Box box{};
        for (uint16_t x = lx; x < ux; ++x) {
            for (uint16_t y = ly; y < uy; ++y) {
                // box is sorted vector such that std::set_union can be applied
                box.emplace_back(y | x << 8);
            }
        }
        boxes.emplace_back(box);
    }

    std::set<Pos> unique{};
    for (size_t i = 0; i < boxes.size() - 1; ++i) {
        const auto boxi = boxes[i];
        for (size_t j = i + 1; j < boxes.size(); ++j) {
            const auto boxj = boxes[j];
            if (std::find_first_of(boxi.begin(), boxi.end(), boxj.begin(), boxj.end()) != boxi.end()) {
                std::set_union(boxi.begin(), boxi.end(), boxj.begin(), boxj.end(),
                               std::inserter(unique, unique.begin()));
            }
        }
    }
    std::cout << unique.size() << std::endl;

    return EXIT_SUCCESS;
}
