#ifndef MENU_H
#define MENU_H

#include <iostream>
#include <string>
#include <fstream>


/**
 * @brief Manages the main menu, the login menu and level selection
 */
class Menu{
    public:
        // Default contstuct
        Menu();

        // Destructor
        ~Menu();
        
        /**
         * @brief Displays the Main menu
         * 
         * @param nothing
         * 
         * @return nothing
         */
        void dispalyMainMenu();

        /**
         * @brief Displays a selection screen that allows the user to choose their level
         * 
         * @param nothing
         * 
         * @return nothing
         */
        void displayLevelSelectionMenu();

};

#endif