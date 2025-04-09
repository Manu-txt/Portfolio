#include "player.h"
#include <algorithm>

Player::Player(){
    username = "Guest";
    lives = 3;
    unusedTokens = {};
}

Player::Player(std::string username, int lives, Token* initialToken)
    : username(username), lives(lives) {
    if (initialToken) {
        unusedTokens.push_back(initialToken);
    }
}

Player::~Player() {
    // Clean up dynamically allocated tokens in unusedTokens
    for (Token* token : unusedTokens) {
        delete token;
    }
    unusedTokens.clear();
}

void Player::loseLives(bool lose) {
    if (lose && lives > 0) {
        --lives;
        std::cout << username << " lost a life! Lives remaining: " << lives << std::endl;
    }
}

void Player::addUnusedTokens(Token* token) {
    if (token) {
        unusedTokens.push_back(token);
        std::cout << "Added token to unused tokens for " << username << "." << std::endl;
    }
}

bool Player::useToken(Token* token, int row, int col, Game* grid) {
    if (!token || !grid) {
        std::cout << "Invalid token or grid pointer." << std::endl;
        return false;
    }

    // Check if the token is in the unusedTokens list
    auto it = std::find(unusedTokens.begin(), unusedTokens.end(), token);
    if (it != unusedTokens.end()) {
        // Place the token on the grid
        if (grid->addToken(token, row, col)) {
            unusedTokens.erase(it);
            std::cout << "Token used by " << username << " at position (" << row << ", " << col << ")." << std::endl;
            return true;
        } else {
            std::cout << "Failed to place token on the grid." << std::endl;
        }
    } else {
        std::cout << "Token not found in unused tokens." << std::endl;
    }
    return false;
}

Player Player::displayPlayerLogin(){
    Player player; // Player object
    std::string username, password;
    std::string playerChoice;

    std::cout << "Login Menu:\n";

    std::cout << "New Player? (yes/no): ";
    std::cin >> playerChoice;

    if (playerChoice == "yes" || playerChoice == "Yes") {
        // Get user data
        std::cout << "Enter a username: ";
        std::cin >> username;
        std::cout << "Enter a password: ";
        std::cin >> password;

        std::cout << std::endl;
        player = player.createPlayer(username, password);
        return player;
    }
    else if(playerChoice == "no" || playerChoice == "No") {
        std::cout << "Enter your username: ";
        std::cin >> username;
        std::cout << "Enter your password: ";
        std::cin >> password;

        std::cout << std::endl;
        return player = player.loginPlayer(username, password);
    }
    else{
        std::cout << "Invalid choice. Please enter 'yes' or 'no'." << std::endl;
        return displayPlayerLogin(); // Recursively call the function until valid input is entered
    }

    return Player();
}

Player Player::createPlayer(const std::string& username, const std::string& password) const{
    std::string filename = "Name&Password.txt";

    // Open the file for reading
    std::ifstream ReadData(filename);

    // Check to see if file can be opened
    if (!ReadData.is_open()) {
        std::cout << "Error opening file for reading...\n";
        return Player();;
    }

    // Opening in append mode so data doesn't get deleted
    std::ofstream EnterData(filename, std::ios::app); 
    if (EnterData.is_open()) {
        // Add the new user data to the file
        EnterData << username << " " << password << "\n"; 
        std::cout << "Created account!\n";
        return Player(username, 3, nullptr);
    } else {
        std::cout << "Error creating account...\n";
    }

    EnterData.close(); // Close the file after writing
    return Player();

}

Player Player::loginPlayer(std::string&  username, std::string&  password) const{
    // Variables to input data into
    std::string fileUsername;
    std::string filePassword;

    std::string filename = "Name&Password.txt";

    // Open the file for reading
    std::ifstream ReadData(filename);

    // Check to see if file can be opened
    if (!ReadData.is_open()) {
        std::cout << "Error opening file for reading...\n";
        return Player();
    }

    bool loginSuccess = false; // Flag to track login success

    // Read through the file to check for matching username/password
    while (ReadData >> fileUsername >> filePassword) {
        if (fileUsername == username && filePassword == password) {
            Player player(username, 3, nullptr);

            std::cout << "Logged in successfully!\n\n";

            // Greeting screen
            std::string welcomePlayer = "Welcome back, " + player.getUsername() + "!";
            int messageLength = welcomePlayer.length();
            int frame = messageLength + 2;

            // Top of the frame
            for (int i = 0; i < frame + 2; ++i) {
                std::cout << "*";
            }

            std::cout << std::endl;

            // Display the welcome message
            std::cout << "  " << welcomePlayer;
            for (int i = 0; i < frame - messageLength - 2; ++i) {
                std::cout << " ";
            }

            std::cout << std::endl;

            // Bottom frame
            for (int i = 0; i < frame + 2; ++i) {
                std::cout << "*";
            }

            std::cout << std::endl;

            loginSuccess = true;
            return Player(username, 3, nullptr);
        }
    }

    // If the login failed (no match found), display an error message
    if (!loginSuccess) {
        std::cout << "Login failed...Continuing as Guest\n";
    }

    ReadData.close(); // Close the file after reading
    return Player();;
}