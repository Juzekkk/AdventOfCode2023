#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <climits>

// Define types for Map and Range for clarity
using Map = std::vector<std::tuple<long long, long long, long long>>;
using Range = std::pair<long long, long long>;

// Function to parse seeds from a line of text
std::vector<long long> parseSeeds(const std::string &line)
{
    std::vector<long long> seeds;
    std::istringstream iss(line.substr(7)); // Skip "seeds: "
    long long seed;
    while (iss >> seed)
    {
        seeds.push_back(seed);
    }
    return seeds;
}

// Function to create ranges from a list of seed values
std::vector<Range> createRanges(const std::vector<long long> &input)
{
    std::vector<Range> result;
    for (size_t i = 0; i < input.size() - 1; i += 2)
    {
        result.emplace_back(input[i], input[i] + input[i + 1]);
    }
    return result;
}

// Function to minimize overlapping ranges
std::vector<Range> minimizeRanges(const std::vector<Range> &ranges)
{
    if (ranges.empty())
        return {};

    std::vector<Range> sortedRanges = ranges;
    std::sort(sortedRanges.begin(), sortedRanges.end());

    std::vector<Range> result;
    Range current = sortedRanges[0];

    for (size_t i = 1; i < sortedRanges.size(); ++i)
    {
        if (sortedRanges[i].first <= current.second)
        {
            current.second = std::max(current.second, sortedRanges[i].second);
        }
        else
        {
            result.push_back(current);
            current = sortedRanges[i];
        }
    }
    result.push_back(current);
    return result;
}

// Function to parse mappings from a file stream
std::vector<Map> parseMappings(std::ifstream &file)
{
    std::vector<Map> mappings;
    std::string line;
    while (getline(file, line))
    {
        if (line.empty() || line.find("map:") != std::string::npos)
        {
            mappings.emplace_back();
            continue;
        }

        std::istringstream iss(line);
        long long a, b, c;
        if (iss >> a >> b >> c)
        {
            mappings.back().emplace_back(a, b, c);
        }
    }
    return mappings;
}

// Function to apply a single seed through a mapping
long long applyMap(long long seed, const Map &map)
{
    for (const auto &[destStart, srcStart, length] : map)
    {
        if (seed >= srcStart && seed < srcStart + length)
        {
            return destStart + (seed - srcStart);
        }
    }
    return seed;
}

// Function to apply a range through a mapping
std::vector<Range> applyMapToRange(const Range &inputRange, const Map &map)
{
    std::vector<Range> result;
    for (const auto &[destStart, srcStart, length] : map)
    {
        long long srcEnd = srcStart + length;
        if (inputRange.first < srcEnd && inputRange.second > srcStart)
        {
            long long mappedStart = std::max(inputRange.first, srcStart);
            long long mappedEnd = std::min(inputRange.second, srcEnd);
            result.emplace_back(destStart + (mappedStart - srcStart), destStart + (mappedEnd - srcStart));
        }
    }
    return result;
}

// Function to process a range through all maps
std::vector<Range> processRangeThroughAllMaps(const Range &inputRange, const std::vector<Map> &allMaps)
{
    std::vector<Range> currentRanges = {inputRange};
    for (const auto &map : allMaps)
    {
        std::vector<Range> newRanges;
        for (const auto &range : currentRanges)
        {
            auto mappedRanges = applyMapToRange(range, map);
            newRanges.insert(newRanges.end(), mappedRanges.begin(), mappedRanges.end());
        }
        currentRanges = std::move(newRanges);
    }
    return currentRanges;
}

// Function to process a single seed through all maps
long long processSingleSeedThroughAllMaps(long long seed, const std::vector<Map> &allMaps)
{
    for (const auto &map : allMaps)
    {
        seed = applyMap(seed, map);
    }
    return seed;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <path_to_file>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file.is_open())
    {
        std::cerr << "Could not open file: " << argv[1] << std::endl;
        return 1;
    }

    std::string line;
    if (!getline(file, line))
    {
        std::cerr << "Error reading first line of the file" << std::endl;
        return 1;
    }

    auto seedsPartOne = parseSeeds(line);
    auto mappings = parseMappings(file);
    file.close();

    // Processing Part One
    std::cout << "\nPart One - Processing Seeds\n";
    std::vector<long long> finalValuesPartOne;
    for (const auto &seed : seedsPartOne)
    {
        std::cout << "Processing seed " << seed << " through all maps:\n";
        long long finalValue = processSingleSeedThroughAllMaps(seed, mappings);
        finalValuesPartOne.push_back(finalValue);
        std::cout << "Final value: " << finalValue << "\n\n";
    }
    auto lowestValuePartOne = *std::min_element(finalValuesPartOne.begin(), finalValuesPartOne.end());
    std::cout << "Lowest location number (Part One): " << lowestValuePartOne << "\n";

    // Processing Part Two
    std::cout << "\nPart Two - Processing Ranges\n";
    auto partTwoRanges = createRanges(seedsPartOne);
    auto minimizedRanges = minimizeRanges(partTwoRanges);
    std::vector<Range> finalRangesPartTwo;
    for (const auto &range : minimizedRanges)
    {
        std::cout << "Processing range [" << range.first << ", " << range.second << "]\n";
        auto finalRanges = processRangeThroughAllMaps(range, mappings);
        finalRangesPartTwo.insert(finalRangesPartTwo.end(), finalRanges.begin(), finalRanges.end());
        for (const auto &fr : finalRanges)
        {
            std::cout << " -> Mapped to [" << fr.first << ", " << fr.second << "]\n";
        }
        std::cout << "\n";
    }

    // Displaying the lowest range start from Part Two
    long long lowestRangeStart = LLONG_MAX;
    for (const auto &range : finalRangesPartTwo)
    {
        if (range.first < lowestRangeStart)
        {
            lowestRangeStart = range.first;
        }
    }

    if (lowestRangeStart != LLONG_MAX)
    {
        std::cout << "Lowest range start (Part Two): " << lowestRangeStart << "\n";
    }
    else
    {
        std::cout << "No ranges found in Part Two.\n";
    }

    return 0;
}