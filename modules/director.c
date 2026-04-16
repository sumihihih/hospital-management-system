#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/director.h"
#include "../headers/admin.h"
#include "../headers/structures.h"

void manageAdministrators()
{
    int choice;
    while (1)
    {
        printf("\n--- Manage Administrators ---\n");
        printf("1. Add New Administrator\n");
        printf("2. Search Administrator\n");
        printf("3. View All Administrators\n");
        printf("0. Back\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        if (choice == 1)
            addAdministrator();
        else if (choice == 2)
            searchAdministrator();
        else if (choice == 3)
            viewAdministrators();
        else if (choice == 0)
            break;
    }
}

void addAdministrator()
{
    User newUser;
    Admin newAdmin;
    FILE *uFile = fopen("data/users.txt", "a");
    FILE *aFile = fopen("data/admins.txt", "a");

    if (!uFile || !aFile)
    {
        printf("Error opening data files!\n");
        return;
    }

    int nextNum = generateNextNumber("ADM");
    sprintf(newUser.id, "ADM%06d", nextNum);
    strcpy(newAdmin.admin_id, newUser.id);

    printf("Enter Admin Name: ");
    scanf(" %49[^\n]", newAdmin.name);
    strcpy(newUser.name, newAdmin.name);

    printf("Set Password: ");
    scanf(" %19[^\n]", newUser.password);
    strcpy(newUser.role, "Admin");

    fprintf(uFile, "\n%s|%s|%s|%s", newUser.id, newUser.name, newUser.password, newUser.role);
    // Save to admins.txt for record keeping
    fprintf(aFile, "\n%s|%s", newAdmin.admin_id, newAdmin.name);

    fclose(uFile);
    fclose(aFile);
    printf("Administrator %s added successfully!\n", newUser.id);
}

void searchAdministrator()
{
    char search[50];
    Admin temp;
    int found = 0;
    FILE *file = fopen("data/admins.txt", "r");

    if (!file)
        return;

    printf("Enter Name or ID to search: ");
    scanf(" %49[^\n]", search);

    char line[100];
    // Skip header
    fgets(line, sizeof(line), file);

    while (fscanf(file, " %14[^|]|%49[^\n]", temp.admin_id, temp.name) == 2)
    {
        if (strstr(temp.name, search) || strcmp(temp.admin_id, search) == 0)
        {
            printf("Found: ID: %s | Name: %s\n", temp.admin_id, temp.name);
            found = 1;
        }
    }

    if (!found)
        printf("No administrator found matching '%s'.\n", search);
    fclose(file);
}

void viewAdministrators()
{
    Admin temp;
    FILE *file = fopen("data/admins.txt", "r");
    char line[100];

    if (!file)
        return;

    printf("\n--- Administrator List ---\n");
    fgets(line, sizeof(line), file);

    while (fscanf(file, " %14[^|]|%49[^\n]", temp.admin_id, temp.name) == 2)
    {
        printf("ID: %s | Name: %s\n", temp.admin_id, temp.name);
    }
    fclose(file);
}