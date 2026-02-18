#include "via.h"
#include "pia.h"
#include <iostream>

int main() {
    std::cout << "Run which?\n1) Value Iteration\n2) Policy Iteration\n3) Both\nChoice: ";
    int choice;
    std::cin >> choice;

    if (choice == 1) {
        runValueIteration();
    } else if (choice == 2) {
        runPolicyIteration();
    } else if (choice == 3) {
        runValueIteration();
        runPolicyIteration();
    } else {
        std::cout << "Invalid choice.\n";
    }
    return 0;
}
