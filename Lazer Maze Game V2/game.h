#ifndef GAME_H
#define GAME_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "token.h"

/**
 * @class Game
 * @brief Represents the entire game state, including the 7x7 grid and all tokens placed on it.
 *
 * Manages:
 * - Grid initialization
 * - Token placement
 * - Laser simulation and updates
 * - Checking and displaying the board state
 */
class Game {
private:
    char grid[7][7];         ///< The 7x7 game grid.
    std::vector<Token*> tokens; ///< Container holding pointers to all tokens on the board.

    int rows, cols;         ///< Grid dimensions (7x7).
    int laserRow, laserCol; ///< Laser's initial position.
    int targetRow, targetCol; ///< Target's position.

public:
    Game();
    ~Game();

    /**
     * @brief Initialize the grid with empty spaces.
     */
    void createGrid();

    /**
     * @brief Display the current state of the game grid.
     */
    void displayGrid() const;

    /**
     * @brief Check if a grid position is valid.
     * @param row Row index to check.
     * @param col Column index to check.
     * @return True if the position is within bounds, false otherwise.
     */
    bool isPositionValid(int row, int col) const;

    /**
     * @brief Add a token to the grid at the specified position.
     * @param token Pointer to the token to add.
     * @param row Row position for the token.
     * @param col Column position for the token.
     * @return True if the token was added successfully, false otherwise.
     */
    bool addToken(Token* token, int row, int col);

    /**
     * @brief Place the laser at the specified position.
     * @param row Row position of the laser.
     * @param col Column position of the laser.
     */
    void placeLaser(int row, int col);

    /**
     * @brief Place the target at the specified position.
     * @param row Row position of the target.
     * @param col Column position of the target.
     */
    void placeTarget(int row, int col);

    /**
     * @brief Get all tokens currently on the board.
     * @return A reference to the vector of token pointers.
     */
    const std::vector<Token*>& getTokens() const;

    /**
     * @brief Prompt the player to manually place a token on the board.
     * @param type The type of token to place (e.g., '/', '\\', '_', '|', '#').
     */
    void placeToken(char type);

    /**
     * @brief Simulate the main laser beam from its initial position and direction.
     */
    void simulateLaser();

    /**
     * @brief Simulate a split laser beam starting from a given position and direction.
     * @param startRow Row index where the split begins.
     * @param startCol Column index where the split begins.
     * @param direction Direction of the split beam.
     */
    void simulateLaserSplit(int startRow, int startCol, const std::string& direction);

    /**
     * @brief Save the player's current game state to a file.
     * @param name Player's name.
     * @param level Current level name.
     */
    void saveGame(const std::string& name, const std::string& level) const;

    /**
     * @brief Load a previously saved game state from a file.
     * @param name Player's name.
     * @param level Level name.
     * @param currLoad True if loading a currently active game, false otherwise.
     */
    void loadGame(const std::string& name, const std::string& level, bool currLoad);

    /**
     * @brief Check if the game is over based on remaining lives.
     * @param lives Player's remaining lives.
     * @return True if the game is over, false otherwise.
     */
    bool GameOver(int lives) const;
};

#endif
