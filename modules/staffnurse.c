#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/nurse.h"

#define MAX_NAME 50

/* Structures */
struct Inventory
{
    char ItemID[10];
    char ItemName[40];
    char Category[30];
    int quantity;
};

/* Function Prototypes */
void nurseMenu(void);
void viewDoctorSchedule(void);
void addInventory(void);
void viewInventory(void);
void searchInventory(void);
void searchDoctor(void);
void editInventory(void);
void deleteInventory(void);

int main()
{
    int choice;

    do
    {
        nurseMenu();
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Enter a number.\n");
            while (getchar() != '\n')
                ;
            continue;
        }

        switch (choice)
        {
        case 1:
            viewDoctorSchedule();
            break;
        case 2:
            addInventory();
            break;
        case 3:
            viewInventory();
            break;
        case 4:
            searchInventory();
            break;
        case 5:
            editInventory();
            break;
        case 6:
            deleteInventory();
            break;
        case 7:
            printf("Logging out...\n");
            break;
        default:
            printf("Invalid option.\n");
        }

    } while (choice != 7);

    return 0;
}

/* Menu */
void nurseMenu()
{
    printf("\n============================\n");
    printf("     STAFF NURSE MENU       \n");
    printf("============================\n");
    printf("1. View Doctor Schedule\n");
    printf("2. Add Inventory\n");
    printf("3. View Inventory\n");
    printf("4. Search Inventory\n");
    printf("5. Edit Inventory\n");
    printf("6. Delete Inventory\n");
    printf("7. Exit\n");
    printf("============================\n");
}

/* View Doctor Schedule */
void viewDoctorSchedule()
{
    FILE *file = fopen("scheduler.txt", "r");

    if (file == NULL)
    {
        printf("No schedule file found.\n");
        return;
    }

    char line[200];
    printf("---------Doctor Schedule---------\n");

    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }

    fclose(file);
}

/* Add Inventory */
void addInventory()
{
    FILE *fptr;
    struct Inventory inv;

    fptr = fopen("../inventory.txt", "a");

    if (fptr == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter item ID: ");
    scanf("%s", inv.ItemID);

    printf("Enter item Name: ");
    scanf("%s", &inv.ItemName);

    printf("Enter Category: ");
    scanf("%s", &inv.Category);

    printf("Enter Stock Level: ");
    scanf("%d", &inv.quantity);

    if (inv.quantity < 0)
    {
        printf("Invalid quantity.\n");
    }
    else
    {
        fprintf(fptr, "%s %d\n", inv.ItemName, inv.quantity);
        printf("Item added successfully.\n");
    }

    fclose(fptr);
}

/* View Inventory */
void viewInventory()
{
    FILE *fptr;
    struct Inventory inv;

    fptr = fopen("inventory.txt", "r");

    if (fptr == NULL)
    {
        printf("No inventory found.\n");
        return;
    }

    printf("\n%-20s %-10s\n", "Item Name", "Quantity");
    printf("------------------------------\n");

    while (fscanf(fptr, "%s %d", inv.ItemName, &inv.quantity) != EOF)
    {
        printf("%-20s %-10d\n", inv.ItemName, inv.quantity);
    }

    fclose(fptr);
}

/* Search Inventory */
void searchInventory()
{
    FILE *fptr;
    struct Inventory inv;
    char name[MAX_NAME];
    int found = 0;

    fptr = fopen("inventory.txt", "r");

    if (fptr == NULL)
    {
        printf("No inventory found.\n");
        return;
    }

    printf("Enter item name: ");
    scanf("%s", name);

    while (fscanf(fptr, "%s %d", inv.ItemName, &inv.quantity) != EOF)
    {
        if (strcmp(inv.ItemName, name) == 0)
        {
            printf("Found: %s - %d\n", inv.ItemName, inv.quantity);
            found = 1;
        }
    }

    if (!found)
        printf("Item not found.\n");

    fclose(fptr);
}

/* Edit Inventory */
void editInventory()
{
    FILE *fptr, *temp;
    struct Inventory inv;
    char name[MAX_NAME];
    int newQty, found = 0;

    fptr = fopen("inventory.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fptr == NULL)
    {
        printf("No inventory file.\n");
        return;
    }

    printf("Enter item to edit: ");
    scanf("%s", name);

    while (fscanf(fptr, "%s %d", inv.ItemName, &inv.quantity) != EOF)
    {
        if (strcmp(inv.ItemName, name) == 0)
        {
            printf("Enter new quantity: ");
            scanf("%d", &newQty);
            fprintf(temp, "%s %d\n", inv.ItemName, newQty);
            found = 1;
        }
        else
        {
            fprintf(temp, "%s %d\n", inv.ItemName, inv.quantity);
        }
    }

    fclose(fptr);
    fclose(temp);

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    if (found)
        printf("Inventory updated.\n");
    else
        printf("Item not found.\n");
}

/* Delete Inventory */
void deleteInventory()
{
    FILE *fptr, *temp;
    struct Inventory inv;
    char name[MAX_NAME];
    int found = 0;

    fptr = fopen("inventory.txt", "r");
    temp = fopen("temp.txt", "w");

    if (fptr == NULL)
    {
        printf("No inventory file.\n");
        return;
    }

    printf("Enter item to delete: ");
    scanf("%s", name);

    while (fscanf(fptr, "%s %d", inv.ItemName, &inv.quantity) != EOF)
    {
        if (strcmp(inv.ItemName, name) != 0)
        {
            fprintf(temp, "%s %d\n", inv.ItemName, inv.quantity);
        }
        else
        {
            found = 1;
        }
    }

    fclose(fptr);
    fclose(temp);

    remove("inventory.txt");
    rename("temp.txt", "inventory.txt");

    if (found)
        printf("Item deleted successfully.\n");
    else
        printf("Item not found.\n");
}