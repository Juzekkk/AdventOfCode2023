#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

std::vector<int> parseLine(const std::string &line)
{
    std::vector<int> numbers;
    std::istringstream iss(line);
    int number;
    while (iss >> number)
    {
        numbers.push_back(number);
    }
    return numbers;
}

std::vector<int> getDifferences(const std::vector<int> &numbers)
{
    std::vector<int> diffs;
    for (size_t i = 1; i < numbers.size(); ++i)
    {
        diffs.push_back(numbers[i] - numbers[i - 1]);
    }
    return diffs;
}

bool allZeroes(const std::vector<int> &numbers)
{
    for (int num : numbers)
    {
        if (num != 0)
            return false;
    }
    return true;
}

std::vector<std::vector<int>> generateSequences(const std::vector<int> &numbers)
{
    std::vector<std::vector<int>> sequences;
    sequences.push_back(numbers);

    while (true)
    {
        std::vector<int> nextSequence = getDifferences(sequences.back());
        if (allZeroes(nextSequence))
        {
            sequences.push_back(nextSequence);
            break;
        }
        sequences.push_back(nextSequence);
    }

    return sequences;
}

int extrapolateNextValue(const std::vector<std::vector<int>> &sequences)
{
    std::vector<std::vector<int>> sequencesCopy = sequences;
    for (int i = sequencesCopy.size() - 2; i >= 0; --i)
    {
        int nextValue = sequencesCopy[i].back() + sequencesCopy[i + 1].back();
        sequencesCopy[i].push_back(nextValue);
    }
    return sequencesCopy[0].back();
}

int extrapolatePreviousValue(const std::vector<std::vector<int>> &sequences)
{
    std::vector<std::vector<int>> sequencesCopy = sequences;
    sequencesCopy.back().insert(sequencesCopy.back().begin(), 0);
    for (int i = sequencesCopy.size() - 2; i >= 0; --i)
    {
        int newFirstValue = sequencesCopy[i].front() - sequencesCopy[i + 1].front();
        sequencesCopy[i].insert(sequencesCopy[i].begin(), newFirstValue);
    }
    return sequencesCopy[0].front();
}

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file path>" << std::endl;
        return 1;
    }

    std::ifstream file(argv[1]);
    if (!file)
    {
        std::cerr << "Error opening file." << std::endl;
        return 1;
    }

    std::string line;
    int sum_next = 0, sum_previous = 0;
    while (std::getline(file, line))
    {
        std::vector<int> numbers = parseLine(line);
        auto sequences = generateSequences(numbers);

        int extrapolatedNextValue = extrapolateNextValue(sequences);
        int extrapolatedPreviousValue = extrapolatePreviousValue(sequences);

        sum_next += extrapolatedNextValue;
        sum_previous += extrapolatedPreviousValue;
    }

    std::cout << "Sum of extrapolated next values: " << sum_next << std::endl;
    std::cout << "Sum of extrapolated previous values: " << sum_previous << std::endl;

    return 0;
}
