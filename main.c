#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/structures.h"
#include "headers/auth.h"
#include "headers/admin.h"

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





void chooseRole(char selectedRole[]) {
    int roleChoice;

    printf("\nChoose role:\n");
    printf("1. Director\n");
    printf("2. Admin\n");
    printf("3. Doctor\n");
    printf("4. Nurse\n");
    printf("5. Patient\n");
    printf("Enter choice: ");
    scanf("%d", &roleChoice);

    if (roleChoice == 1) {
        strcpy(selectedRole, "Director");
    }
    else if (roleChoice == 2) {
        strcpy(selectedRole, "Admin");
    }
    else if (roleChoice == 3) {
        strcpy(selectedRole, "Doctor");
    }
    else if (roleChoice == 4) {
        strcpy(selectedRole, "Nurse");
    }
    else if (roleChoice == 5) {
        strcpy(selectedRole, "Patient");
    }
    else {
        strcpy(selectedRole, "");
    }
}

void openRoleMenu(char role[]) {
    if (strcmp(role, "Admin") == 0) {
        adminMenu();
    }
    else if (strcmp(role, "Doctor") == 0) {
        printf("Doctor menu coming soon...\n");
    }
    else if (strcmp(role, "Patient") == 0) {
        printf("Patient menu coming soon...\n");
    }
    else if (strcmp(role, "Nurse") == 0) {
        printf("Nurse menu coming soon...\n");
    }
    else if (strcmp(role, "Director") == 0) {
        printf("Director menu coming soon...\n");
    }
}

void login() {
    FILE *file;
    User user;
    char selectedRole[20];
    char inputID[15], inputPassword[20];
    int found = 0;

    file = fopen("data/users.txt", "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return;
    }

    printf("\n=====================================");
    printf("\n                LOGIN                ");
    printf("\n=====================================");

    chooseRole(selectedRole);

    if (strcmp(selectedRole, "") == 0) {
        printf("Invalid role choice.\n");
        fclose(file);
        return;
    }

    printf("Enter ID: ");
    scanf("%s", inputID);

    printf("Enter Password: ");
    scanf("%s", inputPassword);

    fscanf(file, "%*[^\n]\n");

    while (fscanf(file, "%[^|]|%[^|]|%[^|]|%[^\n]\n",
                  user.id, user.name, user.password, user.role) != EOF) {

        if (strcmp(inputID, user.id) == 0 &&
            strcmp(inputPassword, user.password) == 0 &&
            strcmp(selectedRole, user.role) == 0) {

            found = 1;
            printf("\nLogin successful!\n");
            printf("Welcome %s (%s)\n", user.name, user.role);
            openRoleMenu(user.role);
            break;
        }
    }

    if (!found) {
        printf("\nInvalid login or role mismatch.\n");
    }

    fclose(file);
}



int main() {
    int choice;

    while (1) {
        printf("\n=====================================");
        printf("\n      HOSPITAL MANAGEMENT SYSTEM     ");
        printf("\n=====================================\n");
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