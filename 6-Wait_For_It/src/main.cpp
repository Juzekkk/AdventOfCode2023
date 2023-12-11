#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <limits>

struct Race
{
    long long Time;
    long long Distance;
};

std::vector<Race> parseFile(const std::string &filename)
{
    std::vector<Race> races;
    std::ifstream file(filename);
    std::string line;
    std::vector<long long> times, distances;

    std::getline(file, line);
    std::stringstream timeStream(line.substr(line.find(':') + 1));
    long long time;
    while (timeStream >> time)
    {
        times.push_back(time);
    }

    std::getline(file, line);
    std::stringstream distanceStream(line.substr(line.find(':') + 1));
    long long distance;
    while (distanceStream >> distance)
    {
        distances.push_back(distance);
    }

    for (size_t i = 0; i < times.size(); ++i)
    {
        races.push_back({times[i], distances[i]});
    }

    return races;
}

Race createConcatenatedRace(const std::vector<Race> &races)
{
    std::string concatenatedTime, concatenatedDistance;
    for (const auto &race : races)
    {
        concatenatedTime += std::to_string(race.Time);
        concatenatedDistance += std::to_string(race.Distance);
    }
    return {std::stoll(concatenatedTime), std::stoll(concatenatedDistance)};
}

long long calculateDistance(long long total_time, long long wait_time)
{
    long long difference = total_time - wait_time;
    return difference * wait_time;
}

int countWinningCombinations(const Race &race)
{
    int count = 0;
    for (long long wait_time = 0; wait_time <= race.Time; ++wait_time)
    {
        if (calculateDistance(race.Time, wait_time) > race.Distance)
        {
            ++count;
        }
    }
    return count;
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path to input file>\n";
        return 1;
    }

    std::vector<Race> races = parseFile(argv[1]);
    int finalResult = 1;

    for (const auto &race : races)
    {
        int winningCombinations = countWinningCombinations(race);
        std::cout << "Race (Time: " << race.Time << ", Distance: " << race.Distance
                  << ") has " << winningCombinations << " winning combinations.\n";
        finalResult *= winningCombinations;
    }
    std::cout << "Final result (excluding concatenated race): " << finalResult << std::endl;

    Race concatenatedRace = createConcatenatedRace(races);
    int winningCombinations = countWinningCombinations(concatenatedRace);
    std::cout << "Concatenated Race (Time: " << concatenatedRace.Time
              << ", Distance: " << concatenatedRace.Distance
              << ") has " << winningCombinations << " winning combinations.\n";

    return 0;
}
