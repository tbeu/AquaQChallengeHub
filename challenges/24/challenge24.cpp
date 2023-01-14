// AquaQ Challenge Hub
// Challenge 24: Huff and Puff
// https://challenges.aquaq.co.uk/challenge/24

#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <variant>
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

struct Node;
using NodeRef = std::shared_ptr<Node>;
using NodeRefPair = std::pair<NodeRef, NodeRef>;
using Table = std::map<std::string, char>;

struct Node
{
    Node(char c, size_t w) : v{c}, w{w}
    {
    }
    Node(NodeRefPair p) : v{p}, w{p.first->w + p.second->w}
    {
    }
    std::variant<NodeRefPair, char> v;
    size_t w{};
};

struct HuffmanTree
{
    HuffmanTree(const std::string& secret)
    {
        std::map<char, size_t> charCounts{};
        for (const auto c : secret) {
            if (charCounts.find(c) == charCounts.end()) {
                charCounts[c] = std::count(secret.begin(), secret.end(), c);
            }
        }
        for (const auto [c, w] : charCounts) {
            insert(std::make_shared<Node>(c, w));
        }

        while (root.size() > 1) {
            auto first = root[0];
            auto second = root[1];
            root.erase(root.begin(), root.begin() + 2);
            insert(std::make_shared<Node>(std::make_pair(first, second)));
        }
        fillTable(root[0]);
    }

    Table table{};

private:
    void insert(NodeRef&& node)
    {
        auto cmp = [](const auto& a, const auto& b) { return a->w < b->w; };
        root.insert(std::upper_bound(root.begin(), root.end(), node, cmp), node);
    }

    void fillTable(std::weak_ptr<Node> node, std::string key = "")
    {
        if (std::holds_alternative<char>(node.lock()->v)) {
            table[key] = std::get<char>(node.lock()->v);
            return;
        }
        const auto& [first, second] = std::get<NodeRefPair>(node.lock()->v);
        fillTable(first, key + "0");
        fillTable(second, key + "1");
    }

    std::vector<NodeRef> root{};
};

std::string decode(const Table& table, const std::string& msg)
{
    std::ostringstream oss{};
    size_t pos{0}, amt{1};
    while (pos + amt <= msg.size()) {
        if (const auto it = table.find(msg.substr(pos, amt)); it != table.end()) {
            oss << it->second;
            pos += amt;
            amt = 1;
        } else {
            amt++;
        }
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

    std::cout << decode(HuffmanTree{lines[0]}.table, lines[1]) << std::endl;

    return EXIT_SUCCESS;
}
