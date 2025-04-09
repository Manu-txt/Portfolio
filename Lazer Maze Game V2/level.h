#ifndef LEVEL_H
#define LEVEL_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "token.h"
#include "game.h"

/**
 * @brief The Level class keeps track of the layout, tokens, and difficulty of a particular level.
 * 
 * Responsible for:
 * - Loading a level layout from a file
 * - Displaying level info
 * - Checking if the level is completed
 */
class Level {
private:
    char layout[7][7];          // Layout of the game level
    std::vector<Token*> tokens; // Tokens defined by the level (if any)
    int difficulty;             // Difficulty of the level
    
public: 
    Level();
    ~Level();

    /**
     * @brief Load a level from a given file, reading the layout and difficulty.
     * @param filename The name of the file containing the level data.
     */
    void loadLevel(const std::string& filename, Game& game);

    /**
     * @brief Displays the information (layout and difficulty) of the level.
     */
    void displayLevelInfo();

    /**
     * @brief Checks if all targets have been hit, indicating level completion.
     *
     * @param grid A pointer to the Game object.
     * @return true if the level is completed, false otherwise.
     */
    bool isLevelCompleted(Game* grid);

    // Getter for difficulty
    int getDifficulty() const;
    
    
};

#endif
