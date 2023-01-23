// AquaQ Challenge Hub
// Challenge 34: Train in Vain
// https://challenges.aquaq.co.uk/challenge/34

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

#include <Tbl.hpp>

struct Train
{
    uint16_t id;                // train id
    uint16_t origin;            // station id
    uint16_t aTime;             // arrival time at (next) station
    uint16_t startTime{aTime};  // start time of route
    uint16_t qTime{0};          // begin of queueing at (next) station
};

constexpr uint16_t nullStation{0};
using Q = std::vector<Train>;

uint16_t toMin(const std::string& time)
{
    return std::stoi(time.substr(0, 2)) * 60 + std::stoi(time.substr(3, 2));
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        return EXIT_SUCCESS;
    }

    std::ifstream file(argv[1]);
    std::stringstream buf;
    buf << file.rdbuf();
    Tbl::Table timeTable(buf.str());
    if (!timeTable) {
        return EXIT_FAILURE;
    }

    const auto nRoutes = timeTable.GetNumColumns();
    const auto nStations = timeTable.GetNumRows();
    std::vector<Q> queues(nStations, Q{});
    std::vector<bool> routeStarted(nRoutes, false);

    uint16_t maxTime{};
    for (uint16_t r = 0; r < nStations; ++r) {
        auto& q = queues[r];

        // Add the trains arriving at this station (and originating from the null station) to queue
        for (uint16_t c = 1; c < nRoutes; ++c) {
            if (routeStarted[c]) {
                continue;
            }
            if (const auto& data = timeTable.GetData(r, c); std::holds_alternative<std::string>(data)) {
                routeStarted[c] = true;
                q.push_back(Train{c, nullStation, toMin(std::get<std::string>(data))});
            }
        }

        uint16_t dTime{};  // departure time of train from station = earliest arrival time of next train
        while (!q.empty()) {
            auto it = std::min_element(q.begin(), q.end(), [](const auto& a, const auto& b) {
                return std::tuple{a.aTime, a.origin, a.qTime, a.id} < std::tuple{b.aTime, b.origin, b.qTime, b.id};
            });
            auto train = *it;
            q.erase(it);
            train.aTime = std::max<uint16_t>(train.aTime, dTime) + 5;
            dTime = train.aTime;
            // Update arrival time of all other queueing trains
            std::for_each(q.begin(), q.end(), [&](auto& t) { t.aTime = std::max<uint16_t>(t.aTime, dTime); });
            // Add train to next station queue (if not is terminal station)
            bool isTerminal{true};
            for (uint16_t i = r + 1; i < nStations; ++i) {
                if (const auto& data = timeTable.GetData(i, train.id); std::holds_alternative<std::string>(data)) {
                    train.aTime += toMin(std::get<std::string>(data)) - toMin(timeTable.Get<std::string>(r, train.id));
                    train.qTime = train.aTime;
                    train.origin = r + 1;
                    queues[i].push_back(train);
                    isTerminal = false;
                    break;
                }
            }
            if (isTerminal) {
                maxTime = std::max<uint16_t>(maxTime, train.aTime - train.startTime);
            }
        }
    }
    std::cout << maxTime << std::endl;

    return EXIT_SUCCESS;
}
