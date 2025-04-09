#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include "token.h"
#include "game.h"

/**
 * @brief Player class holds player data such as username, lives, and unused tokens.
 */
class Player {
private: 
    std::string username;          // Player's username
    int lives;                     // Player's lives
    std::vector<Token*> unusedTokens; // Player's unused tokens

public:
    Player();
    // If initialToken is given, add it to unusedTokens
    Player(std::string username, int lives = 3, Token* initialToken = nullptr);
    ~Player(); // Destructor

    /**
     * @brief Decrease player's life by one if lose == true
     */
    void loseLives(bool lose);

    /**
     * @brief Add a token to the player's unused tokens inventory.
     */
    void addUnusedTokens(Token* token);

    /**
     * @brief Use a token from the player's inventory and place it on the game grid.
     *
     * @param token - a Token pointer from player's unused inventory
     * @param row - row position to place token
     * @param col - column position to place token
     * @param grid - pointer to Game object
     * @return true on success, false on failure
     */
    bool useToken(Token* token, int row, int col, Game* grid);

    /**
     * @brief Displays a player login screen that ask if the user is new or not. 
     *        If login fails, will continue as guest.
     * 
     * @param nothing
     * 
     * @return a player object that will be used to initialize player in main
     */
    Player displayPlayerLogin();

    /**
     * @brief Creates a new player and inputs it into a text file that holds other
     *        usernames and passwords
     * 
     * @param username - a string that holds the username of the player
     * @param password - a string that holds the password of the player
     * 
     * @return a player object that is used in the displayPlayerLogin() function 
     *         to initialize the player in main. If creating account fails, 
     *         returns default Player.
     */
    Player createPlayer(const std::string& username, const std::string& password) const;

    /**
     * @brief If the user has an account, will log the user into their account using their
     *        username and password.
     * 
     * @param username - a string that holds the username of the player
     * @param password - a string that holds the password of the player
     * 
     * @return a player object that is used inthe displayPlayerLogin() function
     *         to initialize the player in main. If login fails, will default to 
     *         a guest account.
     */
    Player loginPlayer(std::string&  username, std::string&  password) const;

    // Getters if needed
    std::string getUsername() const { return username; }
    int getLives() const { return lives; }
};

#endif