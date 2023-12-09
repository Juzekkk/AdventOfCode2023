#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <unordered_set>
#include <vector>
#include <regex>
#include <cmath>
#include <iomanip>

using namespace std;

struct Card
{
    int index;
    unordered_set<int> winningNumbers;
    unordered_set<int> myNumbers;
};

void printCardInfo(const Card &card)
{
    cout << left << setw(10) << "Card " + to_string(card.index);
    cout << setw(20) << "Winning Numbers: ";
    for (auto num : card.winningNumbers)
    {
        cout << num << " ";
    }
    cout << setw(20) << "\tNumbers: ";
    for (auto num : card.myNumbers)
    {
        cout << num << " ";
    }
    cout << endl;
}

vector<Card> parseCardData(const string &filePath)
{
    ifstream file(filePath);
    if (!file.is_open())
    {
        cerr << "Error opening file: " << filePath << endl;
        exit(EXIT_FAILURE);
    }

    vector<Card> cards;
    string line;

    while (getline(file, line))
    {
        Card card;
        string winningNumbersStr, myNumbersStr;
        istringstream iss(line);

        // Extracting the card index
        iss >> ws;     // eat up any leading whitespace
        iss.ignore(5); // ignore "Card "
        iss >> card.index;
        iss.ignore(2); // ignore ": "

        // Extracting the whole winning numbers string and my numbers string
        getline(iss, winningNumbersStr, '|');
        getline(iss, myNumbersStr);

        // Parsing winning numbers
        istringstream winningStream(winningNumbersStr);
        int number;
        while (winningStream >> number)
        {
            card.winningNumbers.insert(number);
        }

        // Parsing my numbers
        istringstream myNumbersStream(myNumbersStr);
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

int calculateTotalScratchcards(const vector<Card> &cards)
{
    vector<int> scratchcards(cards.size(), 1); // Start with 1 copy of each card

    for (int i = 0; i < cards.size(); ++i)
    {
        int wins = countWinningNumbers(cards[i]);

        // Distribute wins to subsequent cards
        for (int j = i + 1; j <= i + wins && j < cards.size(); ++j)
        {
            scratchcards[j] += scratchcards[i];
        }
    }

    // Sum up all the scratchcards
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
        cerr << "Usage: " << argv[0] << " <file_path>" << endl;
        return 1;
    }

    string filePath = argv[1];
    auto cards = parseCardData(filePath);
    int totalPoints = 0;

    for (const auto &card : cards)
    {
        printCardInfo(card);
        int wins = countWinningNumbers(card);
        int score = calculateScore(wins);

        totalPoints += score;
    }

    cout << string(70, '-') << endl; // Print a line for separation
    cout << "Total Points: " << totalPoints << endl;

    int totalScratchcards = calculateTotalScratchcards(cards);
    cout << "Total Scratchcards: " << totalScratchcards << endl;
    return 0;
}