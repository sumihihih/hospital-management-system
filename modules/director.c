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


void viewAppointmentTrends() {
    // Note: Ensure your file is actually named "appoinments.txt" in the data folder
    FILE *fp = fopen("data/appointments.txt", "r");
    if (fp == NULL) {
        printf("\n[!] Error: Cannot find 'data/appointments.txt'\n");
        return;
    }

    int targetMonth;
    int s_count = 0, r_count = 0, c_count = 0;
    char line[512];

    printf("\n====================================================\n");
    printf("           DIRECTOR: APPOINTMENT ANALYTICS          \n");
    printf("====================================================\n");
    printf("Enter Month to analyze (1-12): ");
    
    if (scanf("%d", &targetMonth) != 1) {
        printf("[!] Invalid input.\n");
        fclose(fp);
        return;
    }

    // Skip the header line (AppointmentID|PatientID...)
    fgets(line, sizeof(line), fp);

    while (fgets(line, sizeof(line), fp)) {
        // Use a simple pointer to find the pipes manually
        char *col1 = line;                   // AppointmentID
        char *col2 = strchr(col1, '|') + 1;  // PatientID
        char *col3 = strchr(col2, '|') + 1;  // DoctorID
        char *col4 = strchr(col3, '|') + 1;  // Date (YYYY-MM-DD)
        char *col5 = strchr(col4, '|') + 1;  // Time
        char *col6 = strchr(col5, '|') + 1;  // Status

        if (col4 && col6) {
            int y, m, d;
            // Parse the date from the 4th column
            if (sscanf(col4, "%d-%d-%d", &y, &m, &d) == 3) {
                if (m == targetMonth) {
                    // Check the status in the 6th column
                    if (strncmp(col6, "Scheduled", 9) == 0) s_count++;
                    else if (strncmp(col6, "Rescheduled", 11) == 0) r_count++;
                    else if (strncmp(col6, "Cancelled", 9) == 0) c_count++;
                }
            }
        }
    }

    // --- REPORT DISPLAY ---
    printf("\n----------------------------------------------------\n");
    printf(" REPORT FOR MONTH: %02d | STATUS SUMMARY             \n", targetMonth);
    printf("----------------------------------------------------\n");
    printf("  %-15s : %d\n", "Scheduled", s_count);
    printf("  %-15s : %d\n", "Rescheduled", r_count);
    printf("  %-15s : %d\n", "Cancelled", c_count);
    printf("----------------------------------------------------\n");
    printf("  TOTAL ACTIVITY  : %d\n", (s_count + r_count + c_count));
    printf("====================================================\n");

    if ((s_count + r_count + c_count) > 0) {
        printf("\nVISUAL TREND:\n");
        printf("Sch: "); for(int i=0; i<s_count; i++) printf("#"); printf(" (%d)\n", s_count);
        printf("Res: "); for(int i=0; i<r_count; i++) printf("#"); printf(" (%d)\n", r_count);
        printf("Can: "); for(int i=0; i<c_count; i++) printf("#"); printf(" (%d)\n", c_count);
    } else {
        printf("\n[i] No data found for Month %02d.\n", targetMonth);
    }
    printf("====================================================\n\n");

    fclose(fp);
}
