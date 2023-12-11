#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <regex>
#include <cmath>
#include <iomanip>

struct Card
{
    int index;
    std::unordered_set<int> winningNumbers;
    std::unordered_set<int> myNumbers;
};

void printCardInfo(const Card &card)
{
    std::cout << std::left << std::setw(10) << "Card " + std::to_string(card.index);
    std::cout << std::setw(20) << "Winning Numbers: ";
    for (auto num : card.winningNumbers)
    {
        std::cout << num << " ";
    }
    std::cout << std::setw(20) << "\tNumbers: ";
    for (auto num : card.myNumbers)
    {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

std::vector<Card> parseCardData(const std::string &filePath)
{
    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cerr << "Error opening file: " << filePath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<Card> cards;
    std::string line;

    while (getline(file, line))
    {
        Card card;
        std::string winningNumbersStr, myNumbersStr;
        std::istringstream iss(line);

        iss >> std::ws;
        iss.ignore(5);
        iss >> card.index;
        iss.ignore(2);

        getline(iss, winningNumbersStr, '|');
        getline(iss, myNumbersStr);

        std::istringstream winningStream(winningNumbersStr);
        int number;
        while (winningStream >> number)
        {
            card.winningNumbers.insert(number);
        }

        std::istringstream myNumbersStream(myNumbersStr);
        while (myNumbersStream >> number)
        {
            card.myNumbers.insert(number);
        }

        cards.push_back(card);
    }

    return cards;
}

int countWinningNumbers(const Card &card)
{
    int count = 0;
    for (int num : card.myNumbers)
    {
        if (card.winningNumbers.find(num) != card.winningNumbers.end())
        {
            count++;
        }
    }
    return count;
}

int calculateTotalScratchcards(const std::vector<Card> &cards)
{
    std::vector<int> scratchcards(cards.size(), 1);

    for (int i = 0; i < cards.size(); ++i)
    {
        int wins = countWinningNumbers(cards[i]);

        for (int j = i + 1; j <= i + wins && j < cards.size(); ++j)
        {
            scratchcards[j] += scratchcards[i];
        }
    }

    int totalScratchcards = 0;
    for (int count : scratchcards)
    {
        totalScratchcards += count;
    }
    return totalScratchcards;
}

int calculateScore(int count)
{
    return count > 0 ? pow(2, count - 1) : 0;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>" << std::endl;
        return 1;
    }

    std::string filePath = argv[1];
    auto cards = parseCardData(filePath);
    int totalPoints = 0;

    for (const auto &card : cards)
    {
        printCardInfo(card);
        int wins = countWinningNumbers(card);
        int score = calculateScore(wins);

        totalPoints += score;
    }

    std::cout << std::string(70, '-') << std::endl; // Print a line for separation
    std::cout << "Total Points: " << totalPoints << std::endl;

    int totalScratchcards = calculateTotalScratchcards(cards);
    std::cout << "Total Scratchcards: " << totalScratchcards << std::endl;
    return 0;
}