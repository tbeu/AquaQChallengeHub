// AquaQ Challenge Hub
// Challenge 28: Hall of Mirrors
// https://challenges.aquaq.co.uk/challenge/28

#include <fstream>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <string_view>
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

using Dir = uint8_t;
constexpr Dir R{0};
constexpr Dir D{1};
constexpr Dir L{2};
constexpr Dir U{3};
constexpr char forward{'/'};
constexpr char backward{'\\'};

std::string encode(const std::string_view& text, std::vector<std::string>& lines)
{
    std::ostringstream oss{};
    for (const auto t : text) {
        size_t r, c{0};
        for (r = 1; r < lines.size() - 1; ++r) {
            if (t == lines[r][c]) {
                break;
            }
        }
        c++;
        Dir dir{R};
        while (r > 0 && r < lines.size() - 1 && c > 0 && c < lines[0].size() - 1) {
            if (forward == lines[r][c]) {
                dir = 3 - dir;
                lines[r][c] = backward;
            } else if (backward == lines[r][c]) {
                dir = (5 - dir) % 4;
                lines[r][c] = forward;
            }
            switch (dir) {
                case R:
                    c++;
                    break;
                case D:
                    r++;
                    break;
                case L:
                    c--;
                    break;
                case U:
                    r--;
                    break;
            }
        }
        oss << lines[r][c];
    }
    return oss.str();
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    constexpr std::string_view text{"FISSION_MAILED"};
    std::cout << encode(text, lines) << std::endl;

    return EXIT_SUCCESS;
}
