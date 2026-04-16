#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/nurse.h"
#include "../headers/structures.h"

#define MAX_NAME 50

// structures already defined in structures.h so removed from here

/* Function Prototypes */
void nurseMenu(void);
void viewDoctorSchedule(void);
void addInventory(void);
void viewInventory(void);
void searchInventory(void);
void editInventory(void);
void deleteInventory(void);

void nurseMenu(void)
{
    int choice;

    do
    {
        printf("\n=====================================\n");
        printf("          STAFF NURSE MENU           \n");
        printf("=====================================\n");
        printf("1. View Doctor Schedule\n");
        printf("2. Add Inventory\n");
        printf("3. View Inventory\n");
        printf("4. Search Inventory\n");
        printf("5. Edit Inventory\n");
        printf("6. Delete Inventory\n");
        printf("0. Logout\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1)
        {
            viewDoctorSchedule();
        }
        else if (choice == 2)
        {
            addInventory();
        }
        else if (choice == 3)
        {
            viewInventory();
        }
        else if (choice == 4)
        {
            searchInventory();
        }
        else if (choice == 5)
        {
            editInventory();
        }
        else if (choice == 6)
        {
            deleteInventory();
        }
        else if (choice == 0)
        {
            printf("Logging out...\n");
        }
        else
        {
            printf("Invalid choice. Please try again.\n");
        }

    } while (choice != 0);
}

/* View Doctor Schedule */
void viewDoctorSchedule()
{
    FILE *file = fopen("data/schedule.txt", "r");

    if (file == NULL)
    {
        printf("No schedule file found.\n");
        return;
    }

    char line[200];
    printf("\n=====================================\n");
    printf("          DOCTOR SCHEDULE            \n");
    printf("=====================================\n");

    while (fgets(line, sizeof(line), file))
    {
        printf("%s", line);
    }

    fclose(file);
}

/* Add Inventory */
void addInventory(void)
{
    FILE *file;
    Inventory inv;

    file = fopen("data/inventory.txt", "a");

    if (file == NULL)
    {
        printf("Error opening inventory file.\n");
        return;
    }

    printf("\n=====================================\n");
    printf("            ADD INVENTORY            \n");
    printf("=====================================\n");

    printf("Enter Item ID: ");
    scanf("%9s", inv.item_id);

    printf("Enter Item Name: ");
    scanf(" %49[^\n]", inv.item_name);

    printf("Enter Category: ");
    scanf(" %29[^\n]", inv.category);

    printf("Enter Stock Level: ");
    scanf("%d", &inv.stock_level);

    if (inv.stock_level < 0)
    {
        printf("Invalid stock level.\n");
        fclose(file);
        return;
    }

    fprintf(file, "\n%s|%s|%s|%d",
            inv.item_id, inv.item_name, inv.category, inv.stock_level);

    fclose(file);

    printf("Inventory added successfully!\n");
}

/* View Inventory */
void viewInventory(void)
{
    FILE *file;
    Inventory inv;

    file = fopen("data/inventory.txt", "r");

    if (file == NULL)
    {
        printf("Error opening inventory file.\n");
        return;
    }

    printf("\n=====================================\n");
    printf("            VIEW INVENTORY           \n");
    printf("=====================================\n");
    printf("%-10s %-20s %-15s %-10s\n",
           "ItemID", "ItemName", "Category", "Stock");
    printf("------------------------------------------------------------\n");

    /* skip header line */
    fscanf(file, "%*[^\n]\n");

    while (fscanf(file, " %9[^|]|%49[^|]|%29[^|]|%d",
                  inv.item_id, inv.item_name, inv.category, &inv.stock_level) == 4)
    {
        printf("%-10s %-20s %-15s %-10d\n",
               inv.item_id, inv.item_name, inv.category, inv.stock_level);
    }

    fclose(file);
}


/* Search Inventory */
void searchInventory(void)
{
    FILE *file;
    Inventory inv;
    char search[50];
    int found = 0;

    file = fopen("data/inventory.txt", "r");

    if (file == NULL)
    {
        printf("Error opening inventory file.\n");
        return;
    }

    printf("\n=====================================\n");
    printf("           SEARCH INVENTORY          \n");
    printf("=====================================\n");
    printf("Enter Item ID or Item Name: ");
    scanf(" %49[^\n]", search);

    /* skip header line */
    fscanf(file, "%*[^\n]\n");

    while (fscanf(file, " %9[^|]|%49[^|]|%29[^|]|%d",
                  inv.item_id, inv.item_name, inv.category, &inv.stock_level) == 4)
    {
        if (strcmp(search, inv.item_id) == 0 || strcmp(search, inv.item_name) == 0)
        {
            printf("\nItem Found:\n");
            printf("Item ID     : %s\n", inv.item_id);
            printf("Item Name   : %s\n", inv.item_name);
            printf("Category    : %s\n", inv.category);
            printf("Stock Level : %d\n", inv.stock_level);
            found = 1;
        }
    }

    if (!found)
    {
        printf("Item not found.\n");
    }

    fclose(file);
}


/* Edit Inventory */
void editInventory(void)
{
    FILE *file, *temp;
    Inventory inv;
    char searchName[50];
    int newQty, found = 0;
    char header[200];

    file = fopen("data/inventory.txt", "r");
    temp = fopen("data/temp.txt", "w");

    if (file == NULL || temp == NULL)
    {
        printf("Error opening inventory file.\n");
        if (file != NULL) fclose(file);
        if (temp != NULL) fclose(temp);
        return;
    }

    /* copy header */
    if (fgets(header, sizeof(header), file) != NULL)
    {
        fputs(header, temp);
    }

    printf("Enter item name to edit: ");
    scanf(" %49[^\n]", searchName);

    while (fscanf(file, " %9[^|]|%49[^|]|%29[^|]|%d",
                  inv.item_id, inv.item_name, inv.category, &inv.stock_level) == 4)
    {
        if (strcmp(inv.item_name, searchName) == 0)
        {
            printf("Current stock: %d\n", inv.stock_level);
            printf("Enter new stock level: ");
            scanf("%d", &newQty);

            fprintf(temp, "%s|%s|%s|%d\n",
                    inv.item_id, inv.item_name, inv.category, newQty);
            found = 1;
        }
        else
        {
            fprintf(temp, "%s|%s|%s|%d\n",
                    inv.item_id, inv.item_name, inv.category, inv.stock_level);
        }
    }

    fclose(file);
    fclose(temp);

    remove("data/inventory.txt");
    rename("data/temp.txt", "data/inventory.txt");

    if (found)
        printf("Inventory updated successfully!\n");
    else
        printf("Item not found.\n");
}


// delete inventory

void deleteInventory(void)
{
    FILE *file, *temp;
    Inventory inv;
    char searchName[50];
    int found = 0;
    char header[200];

    file = fopen("data/inventory.txt", "r");
    temp = fopen("data/temp.txt", "w");

    if (file == NULL || temp == NULL)
    {
        printf("Error opening inventory file.\n");
        if (file != NULL) fclose(file);
        if (temp != NULL) fclose(temp);
        return;
    }

    /* copy header */
    if (fgets(header, sizeof(header), file) != NULL)
    {
        fputs(header, temp);
    }

    printf("Enter item name to delete: ");
    scanf(" %49[^\n]", searchName);

    while (fscanf(file, " %9[^|]|%49[^|]|%29[^|]|%d",
                  inv.item_id, inv.item_name, inv.category, &inv.stock_level) == 4)
    {
        if (strcmp(inv.item_name, searchName) != 0)
        {
            fprintf(temp, "%s|%s|%s|%d\n",
                    inv.item_id, inv.item_name, inv.category, inv.stock_level);
        }
        else
        {
            found = 1;
        }
    }

    fclose(file);
    fclose(temp);

    remove("data/inventory.txt");
    rename("data/temp.txt", "data/inventory.txt");

    if (found)
        printf("Item deleted successfully!\n");
    else
        printf("Item not found.\n");
}