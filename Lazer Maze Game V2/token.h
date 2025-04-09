#ifndef TOKEN_H
#define TOKEN_H

#include <string>
#include <vector>

/**
 * @class Token
 * @brief Abstract base class representing a token on the 7x7 grid.
 */
class Token {
protected:
    char symbol; ///< The character symbol representing the token on the grid.
    int row;     ///< The token's row position on the grid.
    int col;     ///< The token's column position on the grid.

public:
    Token();
    Token(char symbol, int row = 0, int col = 0);
    virtual ~Token();

    /**
     * @brief Get the character symbol representing the token.
     * @return The token's symbol.
     */
    char getSymbol() const;

    /**
     * @brief Get the token's row position.
     * @return The row position.
     */
    int getRow() const;

    /**
     * @brief Get the token's column position.
     * @return The column position.
     */
    int getCol() const;

    /**
     * @brief Set the token's row position.
     * @param row The new row position.
     */
    void setRow(int row);

    /**
     * @brief Set the token's column position.
     * @param col The new column position.
     */
    void setCol(int col);

    /**
     * @brief Process interaction with the laser beam.
     * @param direction Current direction of the laser beam (e.g., "up", "down", "left", "right", "stop").
     * @return True if the laser is split into multiple beams, false otherwise.
     */
    virtual bool interactWithLaser(std::string& direction) = 0;
};

/**
 * @class Target
 * @brief Represents a target token ('o') that the laser aims to hit.
 */
class Target : public Token {
private:
    bool isHit; ///< Tracks if this target has already been hit by the laser.

public:
    Target(int row, int col);
    ~Target() override = default;

    /**
     * @brief Get the hit status of the target.
     * @return True if the target has been hit, false otherwise.
     */
    bool getIsHit() const;

    /**
     * @brief Handle laser interaction with the target.
     * @param direction Current laser direction.
     * @return Always false as the target stops the laser.
     */
    bool interactWithLaser(std::string& direction) override;
};

/**
 * @class Mirror
 * @brief Represents a mirror token ('/' or '\\') that reflects the laser beam at a 90-degree angle.
 */
class Mirror : public Token {
public:
    Mirror(int row, int col, char symbol);
    ~Mirror() override = default;

    /**
     * @brief Handle laser interaction with the mirror.
     * @param direction Current laser direction.
     * @return Always false as mirrors do not split the laser.
     */
    bool interactWithLaser(std::string& direction) override;
};

/**
 * @class BeamSplitter
 * @brief Represents a beam splitter token ('_' or '|').
 */
class BeamSplitter : public Token {
public:
    BeamSplitter(int row, int col, char symbol);
    ~BeamSplitter() override = default;

    /**
     * @brief Handle laser interaction with the beam splitter.
     * @param direction Current laser direction.
     * @return True if the laser splits into multiple beams, false otherwise.
     */
    bool interactWithLaser(std::string& direction) override;
};

/**
 * @class Blocker
 * @brief Represents a blocker token ('#') that stops the laser beam immediately.
 */
class Blocker : public Token {
public:
    Blocker(int row, int col);
    ~Blocker() override = default;

    /**
     * @brief Handle laser interaction with the blocker.
     * @param direction Current laser direction.
     * @return Always false as blockers stop the laser.
     */
    bool interactWithLaser(std::string& direction) override;
};

#endif
