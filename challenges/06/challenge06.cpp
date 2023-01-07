// AquaQ Challenge Hub
// Challenge 6: Let me count the ways
// https://challenges.aquaq.co.uk/challenge/6

#include <fstream>
#include <iostream>
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

using Partition = std::vector<uint32_t>;
using PartitionDist = std::pair<Partition, uint32_t>;
using Partitions = std::vector<PartitionDist>;

uint32_t fact(uint32_t n)
{
    return (n < 2) ? 1 : n * fact(n - 1);
}

void partition(Partitions& partitions, uint32_t num, uint32_t count, PartitionDist&& vPartDist, uint32_t sumPart)
{
    auto& [vPart, dist] = vPartDist;
    if (vPart.size() == count - 1) {
        if (const auto lastPart = num - sumPart; lastPart >= vPart.back()) {
            const auto newDist = (lastPart == vPart.back()) ? dist / 2 : dist;
            vPart.push_back(lastPart);
            partitions.push_back({vPart, newDist});
        }
        return;
    }
    for (auto i = vPart.back(); i <= num; ++i) {
        if (const auto newSumPart = sumPart + i; newSumPart <= num) {
            const auto newDist = (i == vPart.back()) ? dist / 2 : dist;
            Partition newVPart{vPart};
            newVPart.push_back(i);
            partition(partitions, num, count, {newVPart, newDist}, newSumPart);
        }
    }
}

Partitions getPartitions(uint32_t num, uint32_t count)
{
    Partitions partitions{};
    const auto dist{fact(count)};
    for (uint32_t i = 0; i <= num / count; ++i) {
        partition(partitions, num, count, {Partition{i}, dist}, i);
    }
    return partitions;
}

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    std::istringstream iss{lines[0]};
    uint32_t count, num;
    iss >> count;
    iss.ignore(22);
    iss >> num;

    const auto partitions = getPartitions(num, count);

    size_t sum{};
    for (const auto& [vPart, dist] : partitions) {
        std::stringstream ss{};
        std::copy(vPart.cbegin(), vPart.cend(), std::ostream_iterator<uint32_t>(ss));
        const auto s{ss.str()};
        sum += std::count(s.cbegin(), s.cend(), '1') * dist;
    }
    std::cout << sum << std::endl;

    return EXIT_SUCCESS;
}
