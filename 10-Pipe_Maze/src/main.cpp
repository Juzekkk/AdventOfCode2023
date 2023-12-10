#include "PipeGrid.h"
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " <input_file_path>" << std::endl;
        return 1;
    }

    std::ifstream inputFile(argv[1]);
    if (!inputFile.is_open())
    {
        std::cerr << "Error: Could not open the input file." << std::endl;
        return 1;
    }

    std::vector<std::string> inputGrid;
    std::string line;
    while (getline(inputFile, line))
    {
        inputGrid.push_back(line);
    }
    inputFile.close();

    PipeGrid pipeGrid(inputGrid);
    int visitedPipes = pipeGrid.countVisitedPipes();
    std::cout << "Distance to furthers pipe : " << visitedPipes / 2 << std::endl
              << std::endl;

    pipeGrid.cleanUpGrid();
    pipeGrid.outputGridToConsole();

    return 0;
}
