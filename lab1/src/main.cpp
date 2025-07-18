#define START_PROGRAM 0;

#include <iostream>

#include "programs.h"

int main(int argc, char* argv[]) {
    int prog_count = 11;
    int choice = START_PROGRAM;
    int (*programs[])(int argc, char* argv[]) = {nullptr,        program_6_main, program_7_main,
                                                 program_8_main, program_9_main, program_10_main,
                                                 program_11_main};

    while (true) {
        if (!(choice >= 1 && choice <= prog_count)) {
            std::cout << "Enter program number [6 - " << prog_count << "] (exit - 0): ";
            std::cin >> choice;
        }

        if (choice >= 1 && choice <= prog_count && choice != 5) {
            std::cout << "Running program " << choice << "...\n" << std::endl;
            if (choice > 5) choice -= 5;
            programs[choice](argc, argv);
            return 0;
        } else if (choice == 0) {
            return 0;
        } else {
            std::cout << "Invalid number. Try again =) \n" << std::endl;
            choice = -1;
        }
    }

    return 0;
}