#include "level.h"

Level::Level() : difficulty(0) {
    // Initialize the layout with empty spaces
    std::cout << "Creating empty spaces\n";
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            layout[i][j] = '.';
        }
    }
}

Level::~Level() {
    // Clean up dynamically allocated tokens
    std::cout << "Level Destructor\n";
    for (Token* token : tokens) {
        delete token;
    }
    tokens.clear();
}

void Level::loadLevel(const std::string& filename, Game& game) {
    std::cout << "Loading Level...\n";
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return;
    }

    // Example loading: read a 7x7 layout and parse tokens
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            file >> layout[i][j];
            char symbol = layout[i][j];
            if (symbol == 'o') {
                game.placeTarget(i, j);
            } else if (symbol == 'b') {
                game.placeLaser(i, j);
            } else if (symbol == '/' || symbol == '\\') {
                game.addToken(new Mirror(i, j, symbol), i, j);
            } else if (symbol == '_' || symbol == '|') {
                game.addToken(new BeamSplitter(i, j, symbol), i, j);
            } else if (symbol == '#') {
                game.addToken(new Blocker(i, j), i, j);
            }
        }
    }

    file >> difficulty;
    file.close();
}


void Level::displayLevelInfo() {
    std::cout << "Level Layout:" << std::endl;
    for (int i = 0; i < 7; ++i) {
        for (int j = 0; j < 7; ++j) {
            std::cout << layout[i][j] << ' ';
        }
        std::cout << std::endl;
    }

    std::cout << "Difficulty: " << difficulty << std::endl;
}

bool Level::isLevelCompleted(Game* grid) {
    if (!grid) {
        std::cerr << "Invalid Game pointer." << std::endl;
        return false;
    }

    // Check if all targets on the grid have been hit
   for (Token* token : grid->getTokens()) {
    Target* target = dynamic_cast<Target*>(token);
    if (target && !target->getIsHit()) {
        std::cout << "Target is not hit yet.\n";
        return false;
    }
}


    //std::cout << "All targets hit! Level completed.\n";
    return true;
}

int Level::getDifficulty() const { return difficulty; }

