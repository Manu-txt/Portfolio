#include "menu.h"
#include "player.h"

// Prototypes


// Class functions
Menu::Menu(){}

Menu::~Menu(){}

void Menu::dispalyMainMenu(){
    std::cout << "1) Choose Level\n";
    std::cout << "2) Load previous level\n";
    std::cout << "3) Login\n";
    std::cout << "4) Exit\n";
}

void Menu::displayLevelSelectionMenu(){
    std::cout << "What level do you want to play?\n";
    // Display Easy levels
    std::cout << "  Easy Levels\n";
    std::cout << "      1) Level 1\n";
    std::cout << "      2) Level 2\n\n";
    // Display Medium levels
    std::cout << "  Medium Levels\n";
    std::cout << "      3) Level 3\n";
    std::cout << "      4) Level 4\n\n";
    // Display Hard levels
    std::cout << "  Hard Levels\n";
    std::cout << "      5) Level 5\n";
    std::cout << "      6) Level 6\n";
}