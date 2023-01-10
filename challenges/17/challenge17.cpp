// AquaQ Challenge Hub
// Challenge 17: The Beautiful Shame
// https://challenges.aquaq.co.uk/challenge/17

#include <chrono>
#include <fstream>
#include <iostream>
#include <map>
#include <numeric>
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

constexpr const char* fmt = " %Y%m%d";
using Day = std::chrono::time_point<
    std::chrono::system_clock,
    std::chrono::duration<int, std::ratio_multiply<std::ratio<24>, std::chrono::hours::period> > >;
using Game = std::pair<Day, int>;
using Games = std::vector<Game>;
using Teams = std::map<std::string, Games>;

Day toDay(const std::string& date)
{
    std::istringstream iss{date};
    Day dp;
    // Format: 1872-11-30
    iss >> date::parse("%F", dp);
    return dp;
}

void addGame(Teams& teams, std::string name, Game&& game)
{
    if (teams.find(name) != teams.end()) {
        teams[name].push_back(game);
    } else {
        teams[name] = Games{game};
    }
}

Teams toTeams(std::vector<std::string>&& lines)
{
    Teams teams{};
    for (const auto& line : lines) {
        const auto day = toDay(line.substr(0, 10));
        const auto pos1 = line.find(',', 11);
        const auto homeTeam = line.substr(11, pos1 - 11);
        const auto pos2 = line.find(',', pos1 + 1);
        const auto awayTeam = line.substr(pos1 + 1, pos2 - pos1 - 1);
        const auto pos3 = line.find(',', pos2 + 1);
        addGame(teams, homeTeam, {day, std::stoi(line.substr(pos2 + 1, pos3 - pos2 - 1))});
        const auto pos4 = line.find(',', pos3 + 1);
        addGame(teams, awayTeam, {day, std::stoi(line.substr(pos3 + 1, pos4 - pos3 - 1))});
    }
    lines.clear();
    return teams;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    lines.reserve(39083);
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    lines.erase(lines.begin());
    const auto teams = toTeams(std::move(lines));

    date::days maxNoScoreDelta{};
    std::string out;
    for (const auto& [team, games] : teams) {
        bool scored{true};
        Day noScoreDay;
        for (const auto& [day, score] : games) {
            if (scored && score == 0) {
                scored = false;
                noScoreDay = day;
                continue;
            }
            if (!scored && score != 0) {
                scored = true;
                if (const auto noScoreDelta = day - noScoreDay; noScoreDelta > maxNoScoreDelta) {
                    maxNoScoreDelta = noScoreDelta;
                    std::ostringstream oss{};
                    oss << team << date::format(fmt, noScoreDay) << date::format(fmt, day);
                    out = oss.str();
                }
            }
        }
    }
    std::cout << out << std::endl;

    return EXIT_SUCCESS;
}
