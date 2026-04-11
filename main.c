#include <stdio.h>
#include <stdlib.h>
#include "headers/structures.h"
#include "headers/auth.h"
#include <string.h>

//helper function for IDs in registeruser
int generateNextNumber(char prefix[]) {
    FILE *file;
    char line[200];
    char id[20];
    int max = 0, num;

    file = fopen("data/users.txt", "r");

    if (file == NULL) {
        return 1;
    }

    fgets(line, sizeof(line), file); // skip header

    while (fgets(line, sizeof(line), file)) {
        sscanf(line, "%[^|]", id);

        if (strncmp(id, prefix, 3) == 0) {
            num = atoi(id + 3);
            if (num > max) {
                max = num;
            }
        }
    }

    fclose(file);
    return max + 1;
}



void registerUser() {
    FILE *file;
    User user;
    int roleChoice;
    char prefix[4];
    int nextNum;

    file = fopen("data/users.txt", "a");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("\n===== REGISTER NEW USER =====\n");

    printf("Choose user type:\n");
    printf("1. Patient\n");
    printf("2. Doctor\n");
    printf("3. Nurse\n");
    printf("Enter choice: ");
    scanf("%d", &roleChoice);

    if (roleChoice < 1 || roleChoice > 3) {
        printf("Invalid choice.\n");
        fclose(file);
        return;
    }

    // assign role + prefix
    if (roleChoice == 1) {
        strcpy(user.role, "Patient");
        strcpy(prefix, "PAT");
    }
    else if (roleChoice == 2) {
        strcpy(user.role, "Doctor");
        strcpy(prefix, "DOC");
    }
    else {
        strcpy(user.role, "Nurse");
        strcpy(prefix, "NUR");
    }

    // generate ID (6 digits)
    nextNum = generateNextNumber(prefix);
    sprintf(user.id, "%s%06d", prefix, nextNum);

    printf("Generated ID: %s\n", user.id);

    printf("Enter Name: ");
    scanf("%s", user.name);

    printf("Enter Password: ");
    scanf("%s", user.password);

    // save to file
    fprintf(file, "\n%s|%s|%s|%s",
            user.id, user.name, user.password, user.role);

    printf("User registered successfully!\n");

    fclose(file);
}



void adminMenu() {
    int choice;

    while (1) {
        printf("\n=====================================");
        printf("\n              ADMIN MENU             ");
        printf("\n=====================================\n");
        printf("1. Register new user\n");
        printf("2. View patients\n");
        printf("3. View inventory\n");
        printf("4. Logout\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter 1-4.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1) {
            registerUser();
        }
        else if (choice == 2) {
            printf("View patients feature coming soon...\n");
        }
        else if (choice == 3) {
            printf("View inventory feature coming soon...\n");
        }
        else if (choice == 4) {
            printf("Logging out...\n");
            break;
        }
        else {
            printf("Invalid choice. Please enter 1-4.\n");
        }
    }
}



void login() {
    FILE *file;
    User user;
    char inputID[15];
    char inputPassword[20];
    int found = 0;

    file = fopen("data/users.txt", "r");

    if (file == NULL) {
        printf("Error: could not open users.txt file.\n");
        return;
    }

    printf("\n=====================================");
    printf("\n                LOGIN                ");
    printf("\n=====================================\n");
    printf("Enter ID: ");
    scanf("%14s", inputID);

    printf("Enter Password: ");
    scanf("%19s", inputPassword);

    // skip header line
    fscanf(file, "%*[^\n]\n");

    while (fscanf(file, "%14[^|]|%49[^|]|%19[^|]|%19[^\n]\n",
                  user.id,
                  user.name,
                  user.password,
                  user.role) == 4) {

        if (strcmp(inputID, user.id) == 0 && strcmp(inputPassword, user.password) == 0) {
            found = 1;
            printf("\nLogin successful!\n");
            printf("Welcome, %s\n", user.name);
            printf("Role: %s\n", user.role);

            if (strcmp(user.role, "Admin") == 0) {
                printf("Opening Admin Menu...\n");
                adminMenu();
            }
            else if (strcmp(user.role, "Doctor") == 0) {
                printf("Opening Doctor Menu...\n");
            }
            else if (strcmp(user.role, "Patient") == 0) {
                printf("Opening Patient Menu...\n");
            }
            else if (strcmp(user.role, "Nurse") == 0) {
                printf("Opening Nurse Menu...\n");
            }
            else if (strcmp(user.role, "Director") == 0) {
                printf("Opening Director Menu...\n");
            }

        }
    }

    if (!found) {
        printf("\nInvalid User ID or Password.\n");
    }

    fclose(file);
}


int main() {
    int choice;

    while (1) {
        printf("\n=====================================\n");
        printf("        HOSPITAL MANAGEMENT SYSTEM\n");
        printf("=====================================\n");
        printf("1. Login\n");
        printf("2. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter 1 or 2.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');  // clear leftover newline

        if (choice == 1) {
            printf("Calling login function...\n");
            login();
        }
        else if (choice == 2) {
            printf("Exiting system...\n");
            break;
        }
        else {
            printf("Invalid input. Please enter 1 or 2.\n");
        }
    }

    return 0;
}