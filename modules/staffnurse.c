#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/nurse.h"
#include "../headers/structures.h"
#include "../headers/inventory.h"

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
void chooseInventoryItemName(int categoryChoice, char itemName[]);

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

void chooseInventoryItemName(int categoryChoice, char itemName[])
{
    int itemChoice;

    while (1)
    {
        printf("\nChoose Item Name:\n");

        if (categoryChoice == 1)
        {
            printf("1. Paracetamol\n");
            printf("2. Amoxicillin\n");
            printf("3. Ibuprofen\n");
            printf("4. Metformin\n");
        }
        else if (categoryChoice == 2)
        {
            printf("1. Syringes\n");
            printf("2. Gloves\n");
            printf("3. Gauze\n");
            printf("4. Masks\n");
        }
        else
        {
            printf("1. Wheelchair\n");
            printf("2. IV Stand\n");
            printf("3. Thermometer\n");
            printf("4. ECG Machine\n");
        }

        printf("Enter choice: ");

        if (scanf("%d", &itemChoice) != 1)
        {
            printf("Invalid input. Please enter a value from 1 to 4.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (categoryChoice == 1)
        {
            if (itemChoice == 1) strcpy(itemName, "Paracetamol");
            else if (itemChoice == 2) strcpy(itemName, "Amoxicillin");
            else if (itemChoice == 3) strcpy(itemName, "Ibuprofen");
            else if (itemChoice == 4) strcpy(itemName, "Metformin");
            else
            {
                printf("Invalid choice. Please try again.\n");
                continue;
            }
        }
        else if (categoryChoice == 2)
        {
            if (itemChoice == 1) strcpy(itemName, "Syringes");
            else if (itemChoice == 2) strcpy(itemName, "Gloves");
            else if (itemChoice == 3) strcpy(itemName, "Gauze");
            else if (itemChoice == 4) strcpy(itemName, "Masks");
            else
            {
                printf("Invalid choice. Please try again.\n");
                continue;
            }
        }
        else
        {
            if (itemChoice == 1) strcpy(itemName, "Wheelchair");
            else if (itemChoice == 2) strcpy(itemName, "IV Stand");
            else if (itemChoice == 3) strcpy(itemName, "Thermometer");
            else if (itemChoice == 4) strcpy(itemName, "ECG Machine");
            else
            {
                printf("Invalid choice. Please try again.\n");
                continue;
            }
        }

        return;
    }
}

/* Add Inventory */
void addInventory(void)
{
    FILE *file;
    Inventory inv;
    int categoryChoice;

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
    while (getchar() != '\n');

    categoryChoice = chooseInventoryCategory(inv.category);
    chooseInventoryItemName(categoryChoice, inv.item_name);

    printf("Enter Stock Level: ");
    if (scanf("%d", &inv.stock_level) != 1)
    {
        printf("Invalid stock level.\n");
        while (getchar() != '\n');
        fclose(file);
        return;
    }

    while (getchar() != '\n');

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
