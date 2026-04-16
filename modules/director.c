#include <stdio.h>
#include "../headers/director.h"

void directorMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n=====================================\n");
        printf("            DIRECTOR MENU            \n");
        printf("=====================================\n");
        printf("1. Manage Administrators\n");
        printf("2. Staff Report\n");
        printf("3. Appointment Trend Report\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter 0-3.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1)
        {
            printf("Manage Administrators feature coming soon...\n");
        }
        else if (choice == 2)
        {
            printf("Staff Report feature coming soon...\n");
        }
        else if (choice == 3)
        {
            printf("Appointment Trend Report feature coming soon...\n");
        }
        else if (choice == 0)
        {
            printf("Logging out...\n");
            break;
        }
        else
        {
            printf("Invalid choice. Please enter 0-3.\n");
        }
    }
}
