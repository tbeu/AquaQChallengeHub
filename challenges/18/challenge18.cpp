// AquaQ Challenge Hub
// Challenge 18: Emit time
// https://challenges.aquaq.co.uk/challenge/18

#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include <date/date.h>
#include <gsl/util>

static bool readFile(const std::string& fileName, std::vector<std::string>& lines)
{
    std::ifstream iss{fileName};
    if (!iss) {
        std::cerr << "Cannot open file " << fileName << std::endl;
        return false;
    }
    auto closeStream = gsl::finally([&iss] { iss.close(); });
    std::string str;
    while (std::getline(iss, str)) {
        lines.push_back(str);
    }
    return true;
}

using namespace std::chrono_literals;

inline bool isPalindromic(const std::chrono::seconds& sec)
{
    const auto hms = date::hh_mm_ss<std::chrono::seconds>(sec);
    return ((hms.hours() % 24) / 10) / 1h == (hms.seconds() % 10) / 1s &&
           ((hms.hours() % 24) % 10) / 1h == (hms.seconds() / 10) / 1s &&
           (hms.minutes() / 10) / 1min == (hms.minutes() % 10) / 1min;
}

std::chrono::seconds toSec(const std::string& time)
{
    std::istringstream iss{time};
    std::chrono::seconds sec;
    // Format: 13:41:00
    iss >> date::parse("%T", sec);
    return sec;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    size_t sum{};
    for (const auto& line : lines) {
        auto secUp = toSec(line);
        auto secDown{secUp};
        size_t delta{};
        while (!isPalindromic(secUp) && !isPalindromic(secDown)) {
            secUp += 1s;
            secDown -= 1s;
            delta++;
        }
        sum += delta;
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
