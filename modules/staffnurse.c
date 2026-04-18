#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/nurse.h"
#include "../headers/structures.h"
#include "../headers/inventory.h"
#include "../headers/helper.h"

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
void viewPatientReports(void);
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
        printf("7. Reporting and Analytics\n");
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
        else if (choice == 7)
        {
            viewPatientReports();
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
    const char *itemNames[3][4] = {
        {"Paracetamol", "Amoxicillin", "Ibuprofen", "Metformin"},
        {"Syringes", "Gloves", "Gauze", "Masks"},
        {"Wheelchair", "IV Stand", "Thermometer", "ECG Machine"}
    };
    int itemChoice;
    int i;

    while (1)
    {
        printf("\nChoose Item Name:\n");

        for (i = 0; i < 4; i++)
        {
            printf("%d. %s\n", i + 1, itemNames[categoryChoice - 1][i]);
        }

        printf("Enter choice: ");

        if (scanf("%d", &itemChoice) != 1)
        {
            printf("Invalid input. Please enter a value from 1 to 4.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (itemChoice < 1 || itemChoice > 4)
        {
            printf("Invalid choice. Please try again.\n");
            continue;
        }

        strcpy(itemName, itemNames[categoryChoice - 1][itemChoice - 1]);
        return;
    }
}

/* Add Inventory */
void addInventory(void)
{
    FILE *file;
    FILE *temp;
    Inventory inv;
    Inventory existing;
    int categoryChoice;
    int found = 0;
    int nextNum;
    char header[200];

    printf("\n=====================================\n");
    printf("            ADD INVENTORY            \n");
    printf("=====================================\n");

    categoryChoice = chooseInventoryCategory(inv.category);
    chooseInventoryItemName(categoryChoice, inv.item_name);

    printf("Enter Stock Level: ");
    if (scanf("%d", &inv.stock_level) != 1)
    {
        printf("Invalid stock level.\n");
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n');

    if (inv.stock_level < 0)
    {
        printf("Invalid stock level.\n");
        return;
    }

    file = fopen("data/inventory.txt", "r");
    temp = fopen("data/temp.txt", "w");

    if (file == NULL || temp == NULL)
    {
        printf("Error opening inventory file.\n");
        if (file != NULL) fclose(file);
        if (temp != NULL) fclose(temp);
        return;
    }

    if (fgets(header, sizeof(header), file) != NULL)
    {
        fputs(header, temp);
    }

    while (fscanf(file, " %9[^|]|%49[^|]|%29[^|]|%d",
                  existing.item_id, existing.item_name,
                  existing.category, &existing.stock_level) == 4)
    {
        if (!found && strcmp(existing.item_name, inv.item_name) == 0)
        {
            existing.stock_level += inv.stock_level;
            found = 1;
        }

        fprintf(temp, "%s|%s|%s|%d\n",
                existing.item_id, existing.item_name,
                existing.category, existing.stock_level);
    }

    if (!found)
    {
        nextNum = generateNextNumber("data/inventory.txt", "I");
        sprintf(inv.item_id, "I%03d", nextNum);
        printf("Generated Item ID: %s\n", inv.item_id);

        fprintf(temp, "%s|%s|%s|%d\n",
                inv.item_id, inv.item_name, inv.category, inv.stock_level);
    }

    fclose(file);
    fclose(temp);

    remove("data/inventory.txt");
    rename("data/temp.txt", "data/inventory.txt");

    if (found)
    {
        printf("Item already exists. Stock updated successfully!\n");
    }
    else
    {
        printf("Inventory added successfully!\n");
    }
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

/* Reporting and Analytics (view and search) */
void viewPatientReports(void)
{
    FILE *file;
    char searchCriteria[50];
    char recordID[15], patientID[15], doctorID[15], diagnosis[50], prescription[50], notes[100];
    int found = 0;
    char header[255];

    file = fopen("data/ehr.txt", "r");

    if (file == NULL)
    {
        printf("Error: EHR database (ehr.txt) not found.\n");
        return;
    }

    printf("\n=====================================\n");
    printf("    PATIENT REPORTING & ANALYTICS    \n");
    printf("=====================================\n");
    printf("Enter Patient ID or Diagnosis to search: ");
    scanf(" %49[^\n]", searchCriteria);

    printf("\n%-10s %-12s %-15s %-15s %-20s\n", "RecID", "PatID", "Diagnosis", "Prescription", "Notes");
    printf("--------------------------------------------------------------------------------\n");

    if (fgets(header, sizeof(header), file) == NULL)
    {
        fclose(file);
        return;
    }

    while (fscanf(file, " %14[^|]|%14[^|]|%14[^|]|%49[^|]|%49[^|]|%99[^\n]",
                  recordID, patientID, doctorID, diagnosis, prescription, notes) == 6)
    {
        if (strstr(patientID, searchCriteria) != NULL || strstr(diagnosis, searchCriteria) != NULL)
        {
            printf("%-10s %-12s %-15s %-15s %-20s\n",
                   recordID, patientID, diagnosis, prescription, notes);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No records found matching: %s\n", searchCriteria);
    }

    fclose(file);
}
