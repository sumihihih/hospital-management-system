#include <stdio.h>
#include <stdlib.h>
#include "headers/structures.h"

int main() {
    int choice;

    while (1) {
        printf("\n======================================");
        printf("\n      HOSPITAL MANAGEMENT SYSTEM      ");
        printf("\n======================================\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            printf("Login selected...\n");
            // we will add login function here next
        }
        else if (choice == 2) {
            printf("Exiting system. Goodbye! \n");
            break;
        }
        else {
            printf("Invalid choice. Try again.\n");
        }
    }

    return 0;
}