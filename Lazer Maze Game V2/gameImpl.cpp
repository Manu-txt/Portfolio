 #include "game.h"
#include "level.h"
/***************************************
 * GAME
 ***************************************/
Game::Game()
    : rows(7), cols(7), laserRow(-1), laserCol(-1), targetRow(-1), targetCol(-1) {
    createGrid();
}

Game::~Game() {
    for (Token* token : tokens) {
        delete token;
    }
    tokens.clear();
}

void Game::createGrid() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            grid[i][j] = '.';
        }
    }
}

void Game::displayGrid() const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            std::cout << grid[i][j] << ' ';
        }
        std::cout << '\n';
    }
}

bool Game::isPositionValid(int row, int col) const {
    return (row >= 0 && row < rows && col >= 0 && col < cols);
}

bool Game::addToken(Token* token, int row, int col) {
    if (!isPositionValid(row, col)) {
        std::cout << "Error: Invalid position (" << row + 1 << ", " << col + 1 << "). Try again.\n";
        return false;
    }

    // Check for real obstacles (tokens), ignore laser paths
    char cell = grid[row][col];
    if (cell != '.' && cell != '^' && cell != 'v' && cell != '<' && cell != '>') {
        std::cout << "Error: Position (" << row + 1 << ", " << col + 1 << ") is already occupied by a token.\n";
        return false;
    }

    // Clear the laser path before placing the token
    if (cell == '^' || cell == 'v' || cell == '<' || cell == '>') {
        grid[row][col] = '.';  // Reset the cell
    }

    // Place the token
    grid[row][col] = token->getSymbol();
    tokens.push_back(token);
    std::cout << "Token placed successfully at (" << row + 1 << ", " << col + 1 << ").\n";
    return true;
}




void Game::placeLaser(int row, int col) {
    if (isPositionValid(row, col)) {
        laserRow = row;
        laserCol = col;
        grid[row][col] = 'b';
    }
}

void Game::placeTarget(int row, int col) {
    if (isPositionValid(row, col)) {
        targetRow = row;
        targetCol = col;
        Token* targetToken = new Target(row, col);
        tokens.push_back(targetToken);
        grid[row][col] = 'o';
    }
}

const std::vector<Token*>& Game::getTokens() const {
    return tokens;
}


void Game::placeToken(char type) {
    int inputRow, inputCol;

    while (true) {
        std::cout << "Enter row and column to place token: ";

        if (!(std::cin >> inputRow >> inputCol)) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
            std::cout << "Error: Invalid input. Please enter valid row and column.\n";
            continue;
        }

        // Adjust for zero-based indexing
        inputRow = 7 - inputRow;
        inputCol -= 1;

        // Validate position
        if (!isPositionValid(inputRow, inputCol)) {
            std::cout << "Error: Invalid position. Please try again.\n";
            continue;
        }

        // Allow placement over laser paths
        if (grid[inputRow][inputCol] != '.' && grid[inputRow][inputCol] != '^' &&
            grid[inputRow][inputCol] != 'v' && grid[inputRow][inputCol] != '<' &&
            grid[inputRow][inputCol] != '>') {
            std::cout << "Error: Position is already occupied by a token. Please try again.\n";
            continue;
        }

        // Clear laser paths if present
        if (grid[inputRow][inputCol] == '^' || grid[inputRow][inputCol] == 'v' ||
            grid[inputRow][inputCol] == '<' || grid[inputRow][inputCol] == '>') {
            grid[inputRow][inputCol] = '.'; // Reset laser path
        }

        // Create the token
        Token* token = nullptr;
        if (type == '/' || type == '\\') {
            token = new Mirror(inputRow, inputCol, type);
        } else if (type == '_' || type == '|') {
            token = new BeamSplitter(inputRow, inputCol, type);
        } else if (type == '#') {
            token = new Blocker(inputRow, inputCol);
        } else {
            std::cerr << "Invalid token type.\n";
            return;
        }

        // Place the token
        if (addToken(token, inputRow, inputCol)) {
            std::cout << "Token placed successfully.\n";
            simulateLaser(); // Recalculate the laser after placing a token
            return;
        } else {
            delete token; // Cleanup memory
            std::cerr << "Error: Failed to place token. Please try again.\n";
        }
    }
}






void Game::simulateLaser() {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == '^' || grid[i][j] == 'v' ||
                grid[i][j] == '<' || grid[i][j] == '>') {
                grid[i][j] = '.';
            }
        }
    }

    std::string direction = "right";
    int currentRow = laserRow;
    int currentCol = laserCol;

    while (isPositionValid(currentRow, currentCol)) {
        Token* currentToken = nullptr;

        for (Token* token : tokens) {
            if (token->getRow() == currentRow && token->getCol() == currentCol) {
                currentToken = token;
                break;
            }
        }

        if (currentToken) {
            std::cout << "Laser interacts with token ('" << currentToken->getSymbol() << "').\n";

            bool split = currentToken->interactWithLaser(direction);

            if (split) {
                if (currentToken->getSymbol() == '|') {
                    simulateLaserSplit(currentRow, currentCol, "up");
                    simulateLaserSplit(currentRow, currentCol, "down");
                } else if (currentToken->getSymbol() == '_') {
                    simulateLaserSplit(currentRow, currentCol, "left");
                    simulateLaserSplit(currentRow, currentCol, "right");
                }
                break;
            }

            if (direction == "stop") {
                break;
            }

            if (direction == "up")    currentRow--;
            else if (direction == "down")  currentRow++;
            else if (direction == "left")  currentCol--;
            else if (direction == "right") currentCol++;
        } else {
            if (grid[currentRow][currentCol] == '.') {
                if (direction == "up") {
                    grid[currentRow][currentCol] = '^';
                    currentRow--;
                } else if (direction == "down") {
                    grid[currentRow][currentCol] = 'v';
                    currentRow++;
                } else if (direction == "left") {
                    grid[currentRow][currentCol] = '<';
                    currentCol--;
                } else if (direction == "right") {
                    grid[currentRow][currentCol] = '>';
                    currentCol++;
                } else {
                    break;
                }
            } else {
                if (direction == "up")    currentRow--;
                else if (direction == "down")  currentRow++;
                else if (direction == "left")  currentCol--;
                else if (direction == "right") currentCol++;
                else break;
            }

            if (direction == "stop") {
                break;
            }
        }
    }

    for (Token* token : tokens) {
        grid[token->getRow()][token->getCol()] = token->getSymbol();
    }

    if (isPositionValid(laserRow, laserCol)) {
        grid[laserRow][laserCol] = 'b'; 
    }
    if (isPositionValid(targetRow, targetCol)) {
        grid[targetRow][targetCol] = 'o'; 
    }

    displayGrid();
    std::cout << "Laser simulation complete.\n";
}

void Game::simulateLaserSplit(int startRow, int startCol, const std::string& direction) {
    int currentRow = startRow;
    int currentCol = startCol;
    std::string splitDirection = direction;

    while (isPositionValid(currentRow, currentCol)) {
        if (splitDirection == "up") {
            grid[currentRow][currentCol] = '^';
            currentRow--;
        } else if (splitDirection == "down") {
            grid[currentRow][currentCol] = 'v';
            currentRow++;
        } else if (splitDirection == "left") {
            grid[currentRow][currentCol] = '<';
            currentCol--;
        } else if (splitDirection == "right") {
            grid[currentRow][currentCol] = '>';
            currentCol++;
        } else {
            break;
        }

        for (Token* token : tokens) {
            if (token->getRow() == currentRow && token->getCol() == currentCol) {
                std::cout << "Split laser interacts with token '" << token->getSymbol()
                          << "' at (" << currentRow << ", " << currentCol << ").\n";

                bool splitAgain = token->interactWithLaser(splitDirection);

                if (splitAgain) {
                    if (token->getSymbol() == '|') {
                        simulateLaserSplit(currentRow, currentCol, "up");
                        simulateLaserSplit(currentRow, currentCol, "down");
                    } else if (token->getSymbol() == '_') {
                        simulateLaserSplit(currentRow, currentCol, "left");
                        simulateLaserSplit(currentRow, currentCol, "right");
                    }
                    return;
                }

                if (splitDirection == "stop") {
                    return;
                }

                break;
            }
        }
    }
}

void Game::saveGame(const std::string& name, const std::string& level) const {
    std::string filename = name + "_" + level + ".txt";
    std::ofstream save_game(filename);

    if (!save_game.is_open()) {
        std::cout << "Unable to save game...\n";
        return;
    }

    // Save only tokens, not laser paths
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (grid[i][j] == '^' || grid[i][j] == 'v' || grid[i][j] == '<' || grid[i][j] == '>') {
                save_game << ". ";  // Save as empty space
            } else {
                save_game << grid[i][j] << " ";
            }
        }
        save_game << "\n";
    }

    save_game.close();
    std::cout << "Game saved successfully.\n";
}


void Game::loadGame(const std::string& name, const std::string& level, bool currLoad) {
    std::string filename = name + "_" + level + ".txt";
    std::ifstream load_game(filename);

    if (!load_game.is_open()) {
        std::cout << "Unable to load game...\n";
        return;
    }

    // Clear the current grid
    createGrid();
    tokens.clear();

    char symbol;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            load_game >> symbol;

            if (symbol == 'b') placeLaser(i, j);
            else if (symbol == 'o') placeTarget(i, j);
            else if (symbol == '/' || symbol == '\\') addToken(new Mirror(i, j, symbol), i, j);
            else if (symbol == '_' || symbol == '|') addToken(new BeamSplitter(i, j, symbol), i, j);
            else if (symbol == '#') addToken(new Blocker(i, j), i, j);
        }
    }

    load_game.close();
    std::cout << "Game loaded successfully.\n";

    // Re-simulate laser after loading
    simulateLaser();
}

bool Game::GameOver(int lives) const {
    if (lives == 0) {
        std::cout << "===============\n";
        std::cout << "|  GAME OVER  |\n";
        std::cout << "===============\n";
        return true;
    } else {
        return false;
    }
}