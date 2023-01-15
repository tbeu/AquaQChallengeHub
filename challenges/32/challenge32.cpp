// AquaQ Challenge Hub
// Challenge 32: In Parenthesis
// https://challenges.aquaq.co.uk/challenge/32

#include <algorithm>
#include <fstream>
#include <iostream>
#include <stack>
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

    size_t count{};
    for (const auto& line : lines) {
        std::stack<char> stack{};
        bool isValid{true};
        for (const auto c : line) {
            if (c == '(' || c == '[' || c == '{') {
                stack.push(c);
                continue;
            }
            if (c != ')' && c != ']' && c != '}') {
                continue;
            }
            if (stack.empty()) {
                isValid = false;
            } else {
                const auto b = stack.top();
                stack.pop();
                isValid = b == '(' && c == ')' || b == '[' && c == ']' || b == '{' && c == '}';
            }
            if (!isValid) {
                break;
            }
        }
        count += isValid && stack.empty() ? 1 : 0;
    }
    std::cout << count << std::endl;

    return EXIT_SUCCESS;
}
