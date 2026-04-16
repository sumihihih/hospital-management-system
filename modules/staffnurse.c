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
    struct Inventory inv;

    // FIX: removed duplicate FILE *file declaration
    FILE *file = fopen("../inventory.txt", "a");

    if (file == NULL)
    {
        printf("Error opening file.\n");
        return;
    }

    printf("Enter item ID: ");
    scanf("%s", inv.ItemID);

    printf("Enter item Name: ");
    scanf("%s", inv.ItemName); // FIX: removed incorrect & (array already decays to pointer)

    printf("Enter Category: ");
    scanf("%s", inv.Category); // FIX: removed incorrect &

    printf("Enter Stock Level: ");
    scanf("%d", &inv.quantity);

    if (inv.quantity < 0)
    {
        printf("Invalid quantity.\n");
        fclose(file); // FIX: close file before returning
        return;
    }

    // FIX: write in pipe-delimited format to match inventory.txt
    fprintf(file, "%s|%s|%s|%d\n", inv.ItemID, inv.ItemName, inv.Category, inv.quantity);
    fclose(file);
    printf("Item added successfully!\n");
} // FIX: added missing closing brace

/* View Inventory */
void viewInventory()
{
    struct Inventory inv;

    // FIX: variable renamed consistently to 'file'; corrected path to ../inventory.txt
    FILE *file = fopen("../inventory.txt", "r");

    if (file == NULL)
    {
        printf("No inventory found.\n");
        return;
    }

    // Skip header line
    char header[200];
    fgets(header, sizeof(header), file);

    printf("\n%-10s %-20s %-15s %-10s\n", "ItemID", "ItemName", "Category", "StockLevel");
    printf("----------------------------------------------------------\n");

    // FIX: fscanf now uses pipe-delimited format matching inventory.txt
    while (fscanf(file, " %9[^|]|%39[^|]|%29[^|]|%d",
                  inv.ItemID, inv.ItemName, inv.Category, &inv.quantity) == 4)
    {
        printf("%-10s %-20s %-15s %-10d\n",
               inv.ItemID, inv.ItemName, inv.Category, inv.quantity);
    }

    fclose(file);
}

/* Search Inventory */
void searchInventory()
{
    struct Inventory inv;
    char name[MAX_NAME];
    int found = 0;

    // FIX: consistent variable name 'file'; corrected path
    FILE *file = fopen("../inventory.txt", "r");

    if (file == NULL)
    {
        printf("No inventory found.\n");
        return;
    }

    // Skip header line
    char header[200];
    fgets(header, sizeof(header), file);

    printf("Enter item name to search: ");
    scanf("%s", name);

    // FIX: fscanf uses pipe-delimited format
    while (fscanf(file, " %9[^|]|%39[^|]|%29[^|]|%d",
                  inv.ItemID, inv.ItemName, inv.Category, &inv.quantity) == 4)
    {
        if (strcmp(inv.ItemName, name) == 0)
        {
            printf("\nItem Found:\n");
            printf("  ID       : %s\n", inv.ItemID);
            printf("  Name     : %s\n", inv.ItemName);
            printf("  Category : %s\n", inv.Category);
            printf("  Stock    : %d\n", inv.quantity);
            found = 1;
        }
    }

    if (!found)
        printf("Item not found.\n");

    // FIX: was fclose(fptr) — variable is 'file'
    fclose(file);
}

/* Edit Inventory */
void editInventory()
{
    struct Inventory inv;
    char name[MAX_NAME];
    int newQty, found = 0;

    // FIX: consistent variable name 'file'; corrected path
    FILE *file = fopen("../inventory.txt", "r");
    FILE *temp = fopen("../temp.txt", "w");

    if (file == NULL)
    {
        printf("No inventory file.\n");
        if (temp != NULL)
            fclose(temp);
        return;
    }

    // Copy header line to temp file
    char header[200];
    if (fgets(header, sizeof(header), file))
        fputs(header, temp);

    printf("Enter item name to edit: ");
    scanf("%s", name);

    // FIX: fscanf and fprintf use pipe-delimited format
    while (fscanf(file, " %9[^|]|%39[^|]|%29[^|]|%d",
                  inv.ItemID, inv.ItemName, inv.Category, &inv.quantity) == 4)
    {
        if (strcmp(inv.ItemName, name) == 0)
        {
            printf("Current stock: %d\n", inv.quantity);
            printf("Enter new quantity: ");
            scanf("%d", &newQty);
            fprintf(temp, "%s|%s|%s|%d\n", inv.ItemID, inv.ItemName, inv.Category, newQty);
            found = 1;
        }
        else
        {
            fprintf(temp, "%s|%s|%s|%d\n", inv.ItemID, inv.ItemName, inv.Category, inv.quantity);
        }
    }

    // FIX: was fclose(fptr) — variable is 'file'
    fclose(file);
    fclose(temp);

    remove("../inventory.txt");
    rename("../temp.txt", "../inventory.txt");

    if (found)
        printf("Inventory updated.\n");
    else
        printf("Item not found.\n");
}

/* Delete Inventory */
void deleteInventory()
{
    struct Inventory inv;
    char name[MAX_NAME];
    int found = 0;

    FILE *fptr = fopen("../inventory.txt", "r");
    FILE *temp = fopen("../temp.txt", "w");

    if (fptr == NULL)
    {
        printf("No inventory file.\n");
        if (temp != NULL)
            fclose(temp);
        return;
    }

    // Copy header line to temp file
    char header[200];
    if (fgets(header, sizeof(header), fptr))
        fputs(header, temp);

    printf("Enter item name to delete: ");
    scanf("%s", name);

    // FIX: fscanf and fprintf use pipe-delimited format; reads all 4 fields
    while (fscanf(fptr, " %9[^|]|%39[^|]|%29[^|]|%d",
                  inv.ItemID, inv.ItemName, inv.Category, &inv.quantity) == 4)
    {
        if (strcmp(inv.ItemName, name) != 0)
        {
            fprintf(temp, "%s|%s|%s|%d\n", inv.ItemID, inv.ItemName, inv.Category, inv.quantity);
        }
        else
        {
            found = 1;
        }
    }

    fclose(fptr);
    fclose(temp);

    remove("../inventory.txt");
    rename("../temp.txt", "../inventory.txt");

    if (found)
        printf("Item deleted successfully.\n");
    else
        printf("Item not found.\n");
}