#include <stdio.h>
#include <stdlib.h>
#include "headers/structures.h"

int main() {
    int choice;

    while(1) {
        printf("\n=== HOSPITAL MANAGEMENT SYSTEM ===\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 2) {
            printf("Exiting system. Goodbye!\n");
            break;
        }
        
        // We will add the login logic here next!
    }

    return 0;
}