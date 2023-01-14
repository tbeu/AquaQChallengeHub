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
        while (nodes.size() > 1) {
            auto first = nodes[0];
            auto second = nodes[1];
            nodes.erase(nodes.begin(), nodes.begin() + 2);
            insert(std::make_shared<Node>(std::make_pair(first, second)));
        }
    }

    std::string decode(const std::string& msg) const
    {
        if (nodes.empty()) {
            return "";
        }
        std::weak_ptr<Node> it = nodes[0];
        if (std::holds_alternative<char>(it.lock()->v)) {
            return std::string(msg.size(), std::get<char>(it.lock()->v));
        }
        std::ostringstream oss{};
        for (const auto c : msg) {
            const auto& [first, second] = std::get<NodeRefPair>(it.lock()->v);
            it = c == '0' ? first : second;
            if (std::holds_alternative<char>(it.lock()->v)) {
                oss << std::get<char>(it.lock()->v);
                it = nodes[0];
            }
        }
        return oss.str();
    }

private:
    void insert(NodeRef&& node)
    {
        auto cmp = [](const auto& a, const auto& b) { return a->w < b->w; };
        nodes.insert(std::upper_bound(nodes.begin(), nodes.end(), node, cmp), node);
    }

    std::vector<NodeRef> nodes{};
};

int main(int argc, char* argv[])
{
    std::vector<std::string> lines{};
    if (argc == 2) {
        if (!readFile(argv[1], lines)) {
            return EXIT_FAILURE;
        }
    }

    std::cout << HuffmanTree{lines[0]}.decode(lines[1]) << std::endl;

    return EXIT_SUCCESS;
}
