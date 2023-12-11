#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

std::unordered_map<char, int> cardStrength = {
    {'A', 14},
    {'K', 13},
    {'Q', 12},
    {'J', 11},
    {'T', 10},
    {'9', 9},
    {'8', 8},
    {'7', 7},
    {'6', 6},
    {'5', 5},
    {'4', 4},
    {'3', 3},
    {'2', 2}};

std::unordered_map<char, int> cardStrengthWithJokers = {
    {'A', 14},
    {'K', 13},
    {'Q', 12},
    {'T', 10},
    {'9', 9},
    {'8', 8},
    {'7', 7},
    {'6', 6},
    {'5', 5},
    {'4', 4},
    {'3', 3},
    {'2', 2},
    {'J', 1}};

enum HandType
{
    HighCard = 1,
    OnePair,
    TwoPair,
    ThreeOfAKind,
    FullHouse,
    FourOfAKind,
    FiveOfAKind
};

struct Hand
{
    std::string cards;
    int bid;
};

HandType determineHandType(const std::string &hand, bool considerJokers)
{
    std::unordered_map<char, int> count;
    int pairs = 0, threes = 0, fours = 0, jokers = 0;

    for (char card : hand)
    {
        if (considerJokers && card == 'J')
            jokers++;
        else
            count[card]++;
    }

    if (considerJokers)
    {
        int maxCount = 0;
        char maxCard = 0;
        for (const auto &p : count)
        {
            if (p.second > maxCount)
            {
                maxCount = p.second;
                maxCard = p.first;
            }
        }
        count[maxCard] += jokers;
    }

    for (const auto &p : count)
    {
        if (p.second == 5)
            return FiveOfAKind;
        if (p.second == 4)
            fours++;
        if (p.second == 3)
            threes++;
        if (p.second == 2)
            pairs++;
    }

    if (fours == 1)
        return FourOfAKind;
    if (threes == 1 && pairs == 1)
        return FullHouse;
    if (threes == 1)
        return ThreeOfAKind;
    if (pairs == 2)
        return TwoPair;
    if (pairs == 1)
        return OnePair;

    return HighCard;
}

bool compareHands(const Hand &a, const Hand &b, const std::unordered_map<char, int> &strength, bool considerJokers)
{
    HandType typeA = determineHandType(a.cards, considerJokers);
    HandType typeB = determineHandType(b.cards, considerJokers);

    if (typeA != typeB)
        return typeA > typeB;

    for (size_t i = 0; i < a.cards.size(); ++i)
    {
        if (strength.at(a.cards[i]) != strength.at(b.cards[i]))
        {
            return strength.at(a.cards[i]) > strength.at(b.cards[i]);
        }
    }
    return false;
}

std::vector<Hand> readHandsFromFile(const std::string &filePath)
{
    std::vector<Hand> hands;
    std::ifstream file(filePath);
    std::string cards;
    int bid;

    while (file >> cards >> bid)
    {
        hands.push_back({cards, bid});
    }

    return hands;
}

int calculateTotalWinnings(const std::vector<Hand> &hands)
{
    int totalWinnings = 0;
    int rank = hands.size();
    for (const auto &hand : hands)
    {
        totalWinnings += hand.bid * rank--;
    }
    return totalWinnings;
}

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <file_path>\n";
        return 1;
    }

    std::string filePath = argv[1];
    std::vector<Hand> hands = readHandsFromFile(filePath);

    std::sort(hands.begin(), hands.end(), [&](const Hand &a, const Hand &b)
              { return compareHands(a, b, cardStrength, false); });
    int totalWinningsWithoutJokers = calculateTotalWinnings(hands);
    std::cout << "Total Winnings without Jokers: " << totalWinningsWithoutJokers << std::endl;

    std::sort(hands.begin(), hands.end(), [&](const Hand &a, const Hand &b)
              { return compareHands(a, b, cardStrengthWithJokers, true); });
    int totalWinningsWithJokers = calculateTotalWinnings(hands);
    std::cout << "Total Winnings with Jokers: " << totalWinningsWithJokers << std::endl;

    return 0;
}
