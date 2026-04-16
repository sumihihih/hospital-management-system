#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/structures.h"
#include "headers/auth.h"
#include "headers/admin.h"
#include "headers/director.h"
#include "headers/nurse.h"
#include "headers/helper.h" 
#include "headers/doctor.h"

int chooseRole(char selectedRole[]) {
    int roleChoice;

    while (1) {
        printf("\nChoose role:\n");
        printf("1. Director\n");
        printf("2. Admin\n");
        printf("3. Doctor\n");
        printf("4. Nurse\n");
        printf("5. Patient\n");
        printf("0. Go Back\n");
        printf("Enter choice: ");

        if (scanf("%d", &roleChoice) != 1) {
            printf("Invalid input please enter a value from 0 to 5.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (roleChoice == 0) {
            return 0;
        }
        else if (roleChoice == 1) {
            strcpy(selectedRole, "Director");
            return 1;
        }
        else if (roleChoice == 2) {
            strcpy(selectedRole, "Admin");
            return 1;
        }
        else if (roleChoice == 3) {
            strcpy(selectedRole, "Doctor");
            return 1;
        }
        else if (roleChoice == 4) {
            strcpy(selectedRole, "Nurse");
            return 1;
        }
        else if (roleChoice == 5) {
            strcpy(selectedRole, "Patient");
            return 1;
        }

        printf("Invalid input please enter a value from 0 to 5.\n");
    }
}

void openRoleMenu(char role[], char userId[], char userName[]) {
    if (strcmp(role, "Admin") == 0) {
        adminMenu(userName);
    }
    else if (strcmp(role, "Doctor") == 0) {
        doctorMenu(userId, userName);
    }
    else if (strcmp(role, "Patient") == 0) {
        printf("Patient menu coming soon...\n");
    }
    else if (strcmp(role, "Nurse") == 0) {
        nurseMenu();
    }
    else if (strcmp(role, "Director") == 0) {
        directorMenu();
    }
}

void login() {
    FILE *file;
    User user;
    char selectedRole[20];
    char inputID[15], inputPassword[20];
    int found;

    while (1) {
        printf("\n=====================================");
        printf("\n                LOGIN                ");
        printf("\n=====================================\n");

        if (chooseRole(selectedRole) == 0) {
            return;
        }

        while (1) {
            found = 0;

            file = fopen("data/users.txt", "r");

            if (file == NULL) {
                printf("Error opening file.\n");
                return;
            }

            printf("\nRole: %s\n", selectedRole);

            while (1) {
                printf("Enter ID: ");

                if (fgets(inputID, sizeof(inputID), stdin) == NULL) {
                    printf("Invalid input. ID cannot be blank.\n");
                    continue;
                }

                inputID[strcspn(inputID, "\n")] = '\0';

                if (inputID[0] == '\0') {
                    printf("Invalid input. ID cannot be blank.\n");
                    continue;
                }

                break;
            }

            while (1) {
                printf("Enter Password: ");

                if (fgets(inputPassword, sizeof(inputPassword), stdin) == NULL) {
                    printf("Invalid input. Password cannot be blank.\n");
                    continue;
                }

                inputPassword[strcspn(inputPassword, "\n")] = '\0';

                if (inputPassword[0] == '\0') {
                    printf("Invalid input. Password cannot be blank.\n");
                    continue;
                }

                break;
            }

            fscanf(file, "%*[^\n]\n");

            while (fscanf(file, "%14[^|]|%49[^|]|%19[^|]|%19[^|]|%19[^\n]\n",
                          user.id, user.name, user.password, user.role, user.status) == 5) {

                if (strcmp(inputID, user.id) == 0 &&
                    strcmp(inputPassword, user.password) == 0 &&
                    strcmp(selectedRole, user.role) == 0) {

                    if (strcmp(user.status, "Active") != 0) {
                        break;
                    }

                    found = 1;
                    printf("\nLogin successful!\n");
                    printf("Welcome %s (%s)\n", user.name, user.role);

                    fclose(file);
                    openRoleMenu(user.role, user.id, user.name);
                    break;
                }
            }

            if (found == 1) {
                break;
            }

            fclose(file);

            printf("\nInvalid login, role mismatch, or account deactivated.\n");

            if (askTryAgainOrGoBack() == 0) {
                break;
            }
        }
    }
}


int main() {
    int choice;

    while (1) {
        printf("\n=====================================");
        printf("\n      HOSPITAL MANAGEMENT SYSTEM     ");
        printf("\n=====================================\n");
        printf("1. Login\n");
        printf("2. Logout\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter 1 or 2.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');  // clear leftover newline

        if (choice == 1) {
            printf("taking you to login screen..\n");
            login();
        }
        else if (choice == 2) {
            printf("Exiting system. See you later :)\n");
            break;
        }
        else {
            printf("Invalid input. Please enter 1 or 2.\n");
        }
    }

    return 0;
}
