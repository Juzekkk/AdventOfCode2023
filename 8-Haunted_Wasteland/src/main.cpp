#include <iostream>
#include <fstream>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>

using Network = std::unordered_map<std::string, std::pair<std::string, std::string>>;

std::string trim(const std::string &str)
{
    size_t first = str.find_first_not_of(' ');
    if (std::string::npos == first)
    {
        return str;
    }
    size_t last = str.find_last_not_of(' ');
    return str.substr(first, (last - first + 1));
}

std::string readInstructions(std::ifstream &file)
{
    std::string instructions;
    getline(file, instructions);
    return instructions;
}

Network parseNodes(std::ifstream &file)
{
    Network network;
    std::string line, node, left, right;

    while (getline(file, line))
    {
        std::stringstream ss(line);
        ss >> node;
        ss.ignore(4); // Ignore " = ("
        getline(ss, left, ',');
        getline(ss, right, ')');
        network[trim(node)] = {trim(left), trim(right)};
    }

    return network;
}

long long lcm(long long a, long long b)
{
    long long maxNum = std::max(a, b);
    long long minNum = std::min(a, b);
    for (long long i = maxNum;; i += maxNum)
    {
        if (i % minNum == 0)
            return i;
    }
}

int navigateToNode(const std::string &startNode, const std::string &endNode, const std::string &instructions, const Network &network, bool endWithZ = false)
{
    std::string currentNode = startNode;
    int steps = 0;
    do
    {
        char direction = instructions[steps % (instructions.size() - 1)];
        currentNode = (direction == 'L') ? network.at(currentNode).first : network.at(currentNode).second;
        steps++;
    } while (!endWithZ ? currentNode != endNode : currentNode.back() != 'Z');
    return steps;
}

long long syncAllPaths(const std::string &instructions, const Network &network)
{
    std::vector<long long> loopDistances;
    for (const auto &node : network)
    {
        if (node.first.back() == 'A')
        {
            loopDistances.push_back(navigateToNode(node.first, "", instructions, network, true));
        }
    }

    long long syncPoint = loopDistances[0];
    for (size_t i = 1; i < loopDistances.size(); ++i)
    {
        syncPoint = lcm(syncPoint, loopDistances[i]);
    }
    return syncPoint;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file_path>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }

    std::string instructions = readInstructions(file);
    Network network = parseNodes(file);
    file.close();

    std::cout << "Reached ZZZ in " << navigateToNode("AAA", "ZZZ", instructions, network) << " steps." << std::endl;
    std::cout << "All paths reached nodes ending with Z in " << syncAllPaths(instructions, network) << " steps." << std::endl;

    return 0;
}
