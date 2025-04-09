#include "token.h"
#include <iostream>

/***************************************
 * Token (Base Class)
 ***************************************/
Token::Token() : symbol('.'), row(0), col(0) {}

Token::Token(char symbol, int row, int col) : symbol(symbol), row(row), col(col) {}

Token::~Token() {}

char Token::getSymbol() const { return symbol; }
int Token::getRow() const { return row; }
int Token::getCol() const { return col; }

void Token::setRow(int row) { this->row = row; }
void Token::setCol(int col) { this->col = col; }

/***************************************
 * Target
 ***************************************/
Target::Target(int row, int col) : Token('o', row, col), isHit(false) {}

bool Target::getIsHit() const { return isHit; }

bool Target::interactWithLaser(std::string& direction) {
    if (!isHit) {
        isHit = true;
        std::cout << "Laser hit the target!\n";
    } else {
        std::cout << "Target is already hit!\n";
    }

    direction = "stop"; // Stop the laser
    return false;
}

/***************************************
 * Mirror
 ***************************************/
Mirror::Mirror(int row, int col, char symbol) : Token(symbol, row, col) {}

bool Mirror::interactWithLaser(std::string& direction) {
    if (symbol == '/') {
        if (direction == "up") direction = "right";
        else if (direction == "right") direction = "up";
        else if (direction == "down") direction = "left";
        else if (direction == "left") direction = "down";
    } else if (symbol == '\\') {
        if (direction == "up") direction = "left";
        else if (direction == "left") direction = "up";
        else if (direction == "down") direction = "right";
        else if (direction == "right") direction = "down";
    }

    std::cout << "Laser reflected by mirror to direction: " << direction << "\n";
    return false;
}

/***************************************
 * BeamSplitter
 ***************************************/
BeamSplitter::BeamSplitter(int row, int col, char symbol) : Token(symbol, row, col) {}

bool BeamSplitter::interactWithLaser(std::string& direction) {
    std::cout << "Laser hit the beam splitter.\n";

    if (symbol == '|') {
        if (direction == "right" || direction == "left") {
            std::cout << "BeamSplitter (|): Splitting laser into up and down directions.\n";
            return true;
        }
    } else if (symbol == '_') {
        if (direction == "up" || direction == "down") {
            std::cout << "BeamSplitter (_): Splitting laser into left and right directions.\n";
            return true;
        }
    }

    direction = "stop"; // Laser stops when invalid split occurs
    return false;
}

/***************************************
 * Blocker
 ***************************************/
Blocker::Blocker(int row, int col) : Token('#', row, col) {}

bool Blocker::interactWithLaser(std::string& direction) {
    std::cout << "Laser blocked by blocker.\n";
    direction = "stop"; // Laser stops
    return false;
}
