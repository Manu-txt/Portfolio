/**
 *This program serves as the central hub of the Laser Maze game, integrating 
 *and managing the flow of gameplay by arranging the various game 
 *components. It acts as the communicator between the different classes and 
 *.h files, allowing smooth interactions and ensuring that the game runs 
 *as seamlessly as possible. The game involves core features such as 
 *player login, level progression, saving/loading game states, and dynamically 
 *placing tokens on the game grid.
 * 
 * CECS 275 MW @5:30pm -- Fall 2024
 * @author Hector Polanco
 * @author Emmanuel Velazquez
 * @version 3.0.0
 */

#include <iostream>
#include <string>
#include "menu.h"
#include "level.h"
#include "game.h"
#include "player.h"
#include "token.h"

int getIntInput(const std::string& prompt, int min, int max) {
    int choice;
    bool valid = false;
    do {
        std::cout << prompt;
        std::cin >> choice;
        if (std::cin.fail() || choice < min || choice > max) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Invalid input. Please try again.\n";
        } else {
            valid = true;
        }
    } while (!valid);
    return choice;
}

int main() {
    std::cout << "=== Laser Maze Game ===\n\n";

    Menu menu;

    // Warn user about auto-save
    std::cout << "Warning: Game saves automatically\n\n";

    // Player login
    Player player = player.displayPlayerLogin();

    bool startLazer = false; 
    bool exit = false; 

    while(!exit) {
        std::cout << "\n=== Main Menu ===\n";
        menu.dispalyMainMenu();
        int choice = getIntInput("Enter your choice: ", 1, 4); // Adjusted max to 3 since we have only 4 options
        std::string lazerChoice; 

        switch(choice) {
            case 1: {
                menu.displayLevelSelectionMenu();
                int lvlChoice = getIntInput("\nEnter level number (1-6): ", 1, 6);

                std::string levelFile;
                if (lvlChoice == 1) levelFile = "level1.txt";
                else if (lvlChoice == 2) levelFile = "level2.txt";
                else if (lvlChoice == 3) levelFile = "level3.txt";
                else if (lvlChoice == 4) levelFile = "level4.txt";
                else if (lvlChoice == 5) levelFile = "level5.txt";
                else if (lvlChoice == 6) levelFile = "level6.txt";

                Game game;
                Level currentLevel;
                currentLevel.loadLevel(levelFile, game);
                currentLevel.displayLevelInfo();
                game.displayGrid();
            
                startLazer = false;
                while(!startLazer){
                    std::cout << "You can place a token now. Enter type of token: \n"
                              << "Available tokens: '/' (Mirror), '\\' (Mirror), '_' (BeamSplitter), '|' (BeamSplitter), '#' (Blocker)\n"
                              << "Or enter 'Q' if want to to exit the game.\n";
                    char tokenType;
                    std::cin >> tokenType;

                    if (tokenType != 'Q' && tokenType != 'q') {
                        game.placeToken(tokenType);
                    }
                    else{
                        break;
                    }

                    std::cout << "Finished placing tokens? (yes/no): ";
                    std::cin >> lazerChoice;
                    std::cout << std::endl;

                    if(lazerChoice == "Yes" || lazerChoice == "yes"){
                        startLazer = true;
                        // Laser simulation is already done during token placement and after load
                        if (currentLevel.isLevelCompleted(&game)) {
                            std::cout << "You completed the level!\n";
                        } 
                        else {
                            std::cout << "Level not completed yet.\n";
                            player.loseLives(true);
                        }
                    }
                    else if(lazerChoice == "No" || lazerChoice == "no"){
                        startLazer = false;
                    }
                    else{
                        std::cout << "Invalid choice...\n";
                    }

                    // Save the game state after each round of placements/decisions
                    game.saveGame(player.getUsername(), levelFile);
                }
                break;
            }
            case 2: {
                menu.displayLevelSelectionMenu();
                int lvlChoice = getIntInput("\nEnter level number (1-6): ", 1, 6);

                std::string levelFile;
                if (lvlChoice == 1) levelFile = "level1.txt";
                else if (lvlChoice == 2) levelFile = "level2.txt";
                else if (lvlChoice == 3) levelFile = "level3.txt";
                else if (lvlChoice == 4) levelFile = "level4.txt";
                else if (lvlChoice == 5) levelFile = "level5.txt";
                else if (lvlChoice == 6) levelFile = "level6.txt";

                Game game;
                Level currentLevel;

                game.loadGame(player.getUsername(), levelFile, true);
                startLazer = false;
                while(!startLazer){
                    std::cout << "You can place a token now. Enter type of token: \n"
                              << "Available tokens: '/' (Mirror), '\\' (Mirror), '_' (BeamSplitter), '|' (BeamSplitter), '#' (Blocker)\n"
                              << "Or enter 'Q' if want to to exit the game.\n";
                    char tokenType;
                    std::cin >> tokenType;

                    if (tokenType != 'Q' && tokenType != 'q') {
                        game.placeToken(tokenType);
                    }
                    else{
                        break;
                    }

                    std::cout << "Finished placing tokens? (yes/no): ";
                    std::cin >> lazerChoice;
                    std::cout << std::endl;

                    if(lazerChoice == "Yes" || lazerChoice == "yes"){
                        startLazer = true;
                        // Laser simulation is already done during token placement and after load
                        if (currentLevel.isLevelCompleted(&game)) {
                            std::cout << "You completed the level!\n";
                        } 
                        else {
                            std::cout << "Level not completed yet.\n";
                            player.loseLives(true);
                        }
                    }
                    else if(lazerChoice == "No" || lazerChoice == "no"){
                        startLazer = false;
                    }
                    else{
                        std::cout << "Invalid choice...\n";
                    }

                    // Save the game state after each round of placements/decisions
                    game.saveGame(player.getUsername(), levelFile);
                }
                break;
            }
            case 3: {
                // Re-login
                player = player.displayPlayerLogin();
                break;
            }
            case 4: {
                // Exiting game
                exit = true;
                std::cout << "Exiting the game.\n";
                break;
            }
            default:
                std::cout << "Invalid choice.\n";
                break;
        }
    }

    return 0;
}
