#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/director.h"
#include "../headers/structures.h"
#include "../headers/helper.h"

void writeNewLineIfNeeded(FILE *file)
{
    long fileSize;

    fseek(file, 0, SEEK_END);
    fileSize = ftell(file);

    if (fileSize > 0)
    {
        fseek(file, -1, SEEK_END);

        if (fgetc(file) != '\n')
        {
            fseek(file, 0, SEEK_END);
            fputc('\n', file);
        }
    }
}

void printStaffTableHeader(void)
{
    printf("\n---------------------------------------------------------------\n");
    printf("%-10s %-20s %-15s %-15s\n", "ID", "Name", "Role", "Department");
    printf("---------------------------------------------------------------\n");
}

void printStaffRow(char id[], char name[], char role[], char department[])
{
    printf("%-10s %-20s %-15s %-15s\n", id, name, role, department);
}

int isMatch(char text[], char search[])
{
    char textLower[100];
    char searchLower[100];

    strcpy(textLower, text);
    strcpy(searchLower, search);
    toLowerCase(textLower);
    toLowerCase(searchLower);

    return strstr(textLower, searchLower) != NULL;
}

void viewAdministrators(void)
{
    Admin temp;
    FILE *file = fopen("data/admins.txt", "r");

    if (!file)
    {
        printf("No admins file found.\n");
        return;
    }

    printf("\n--- Administrator List ---\n");
    fgets((char[100]){0}, 100, file);

    while (fscanf(file, " %14[^|]|%49[^\n]", temp.admin_id, temp.name) == 2)
    {
        printf("ID: %s | Name: %s\n", temp.admin_id, temp.name);
    }

    fclose(file);
}

void addAdministrator(void)
{
    User newUser;
    Admin newAdmin;
    int nextNum;
    FILE *uFile = fopen("data/users.txt", "a+");
    FILE *aFile = fopen("data/admins.txt", "a+");

    if (!uFile || !aFile)
    {
        printf("Error opening data files!\n");
        if (uFile) fclose(uFile);
        if (aFile) fclose(aFile);
        return;
    }

    nextNum = generateNextNumber("data/users.txt", "ADM");
    sprintf(newUser.id, "ADM%03d", nextNum);
    strcpy(newAdmin.admin_id, newUser.id);

    printf("Enter Admin Name: ");
    scanf(" %49[^\n]", newAdmin.name);
    strcpy(newUser.name, newAdmin.name);

    printf("Set Password: ");
    scanf(" %19[^\n]", newUser.password);
    strcpy(newUser.role, "Admin");
    strcpy(newUser.status, "Active");

    writeNewLineIfNeeded(uFile);
    writeNewLineIfNeeded(aFile);

    fprintf(uFile, "%s|%s|%s|%s|%s",
            newUser.id, newUser.name, newUser.password, newUser.role, newUser.status);
    fprintf(aFile, "%s|%s", newAdmin.admin_id, newAdmin.name);

    fclose(uFile);
    fclose(aFile);
    printf("Administrator %s added successfully!\n", newUser.id);
}

void searchAdministrator(void)
{
    char search[50];
    Admin temp;
    int found = 0;
    FILE *file = fopen("data/admins.txt", "r");
    char line[100];

    if (!file)
    {
        printf("No admins file found.\n");
        return;
    }

    printf("Enter Name or ID to search: ");
    scanf(" %49[^\n]", search);

    fgets(line, sizeof(line), file);

    while (fscanf(file, " %14[^|]|%49[^\n]", temp.admin_id, temp.name) == 2)
    {
        if (isMatch(temp.name, search) || strcmp(temp.admin_id, search) == 0)
        {
            printf("Found: ID: %s | Name: %s\n", temp.admin_id, temp.name);
            found = 1;
        }
    }

    if (!found)
    {
        printf("No administrator found matching '%s'.\n", search);
    }

    fclose(file);
}

void manageAdministrators(void)
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

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter 0-3.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1)
            addAdministrator();
        else if (choice == 2)
            searchAdministrator();
        else if (choice == 3)
            viewAdministrators();
        else if (choice == 0)
            return;
        else
            printf("Invalid choice. Please enter 0-3.\n");
    }
}

void printAllAdminsForStaff(void)
{
    FILE *file = fopen("data/admins.txt", "r");
    char id[15], name[50];

    if (!file)
    {
        return;
    }

    fgets((char[100]){0}, 100, file);

    while (fscanf(file, " %14[^|]|%49[^\n]", id, name) == 2)
    {
        printStaffRow(id, name, "Administrator", "-");
    }

    fclose(file);
}

void printAllDoctorsForStaff(void)
{
    FILE *file = fopen("data/doctors.txt", "r");
    char id[15], name[50], department[50], specialization[50];

    if (!file)
    {
        return;
    }

    fgets((char[200]){0}, 200, file);

    while (fscanf(file, " %14[^|]|%49[^|]|%49[^|]|%49[^\n]", id, name, department, specialization) == 4)
    {
        printStaffRow(id, name, "Doctor", department);
    }

    fclose(file);
}

void printAllNursesForStaff(void)
{
    FILE *file = fopen("data/nurses.txt", "r");
    char id[15], name[50], department[50];

    if (!file)
    {
        return;
    }

    fgets((char[200]){0}, 200, file);

    while (fscanf(file, " %14[^|]|%49[^|]|%49[^\n]", id, name, department) == 3)
    {
        printStaffRow(id, name, "Nurse", department);
    }

    fclose(file);
}

void viewAllStaff(void)
{
    printf("\n--- All Staff Report ---\n");
    printStaffTableHeader();
    printAllAdminsForStaff();
    printAllDoctorsForStaff();
    printAllNursesForStaff();
}

void filterAdmins(int filterChoice, char search[], int *found)
{
    FILE *file = fopen("data/admins.txt", "r");
    char id[15], name[50];

    if (!file)
    {
        return;
    }

    fgets((char[100]){0}, 100, file);

    while (fscanf(file, " %14[^|]|%49[^\n]", id, name) == 2)
    {
        if ((filterChoice == 1 && isMatch("Administrator", search)) ||
            (filterChoice == 2 && isMatch(name, search)))
        {
            printStaffRow(id, name, "Administrator", "-");
            *found = 1;
        }
    }

    fclose(file);
}

void filterDoctors(int filterChoice, char search[], int *found)
{
    FILE *file = fopen("data/doctors.txt", "r");
    char id[15], name[50], department[50], specialization[50];

    if (!file)
    {
        return;
    }

    fgets((char[200]){0}, 200, file);

    while (fscanf(file, " %14[^|]|%49[^|]|%49[^|]|%49[^\n]", id, name, department, specialization) == 4)
    {
        if ((filterChoice == 1 && isMatch("Doctor", search)) ||
            (filterChoice == 2 && isMatch(name, search)) ||
            (filterChoice == 3 && isMatch(department, search)))
        {
            printStaffRow(id, name, "Doctor", department);
            *found = 1;
        }
    }

    fclose(file);
}

void filterNurses(int filterChoice, char search[], int *found)
{
    FILE *file = fopen("data/nurses.txt", "r");
    char id[15], name[50], department[50];

    if (!file)
    {
        return;
    }

    fgets((char[200]){0}, 200, file);

    while (fscanf(file, " %14[^|]|%49[^|]|%49[^\n]", id, name, department) == 3)
    {
        if ((filterChoice == 1 && isMatch("Nurse", search)) ||
            (filterChoice == 2 && isMatch(name, search)) ||
            (filterChoice == 3 && isMatch(department, search)))
        {
            printStaffRow(id, name, "Nurse", department);
            *found = 1;
        }
    }

    fclose(file);
}

void searchStaffReport(void)
{
    int filterChoice;
    int roleChoice;
    int found = 0;
    char search[50];

    printf("\nSearch Staff By:\n");
    printf("1. Role\n");
    printf("2. Name\n");
    printf("3. Department\n");
    printf("Enter choice: ");

    if (scanf("%d", &filterChoice) != 1)
    {
        printf("Invalid input.\n");
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n');

    if (filterChoice == 1)
    {
        printf("\nChoose Role:\n");
        printf("1. Administrator\n");
        printf("2. Doctor\n");
        printf("3. Nurse\n");
        printf("Enter choice: ");

        if (scanf("%d", &roleChoice) != 1)
        {
            printf("Invalid input.\n");
            while (getchar() != '\n');
            return;
        }

        while (getchar() != '\n');

        if (roleChoice == 1) strcpy(search, "Administrator");
        else if (roleChoice == 2) strcpy(search, "Doctor");
        else if (roleChoice == 3) strcpy(search, "Nurse");
        else
        {
            printf("Invalid role choice.\n");
            return;
        }
    }
    else if (filterChoice == 2 || filterChoice == 3)
    {
        printf("Enter search term: ");
        scanf(" %49[^\n]", search);
    }
    else
    {
        printf("Invalid choice.\n");
        return;
    }

    printf("\n--- Staff Search Result ---\n");
    printStaffTableHeader();

    filterAdmins(filterChoice, search, &found);
    filterDoctors(filterChoice, search, &found);
    filterNurses(filterChoice, search, &found);

    if (!found)
    {
        printf("No matching staff found.\n");
    }
}

void staffReport(void)
{
    int choice;

    while (1)
    {
        printf("\n--- Staff Report ---\n");
        printf("1. View All Staff\n");
        printf("2. Search / Filter Staff\n");
        printf("0. Back\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter 0-2.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1)
            viewAllStaff();
        else if (choice == 2)
            searchStaffReport();
        else if (choice == 0)
            return;
        else
            printf("Invalid choice. Please enter 0-2.\n");
    }
}

void viewAppointmentTrends(void)
{
    FILE *fp = fopen("data/appointments.txt", "r");
    char appId[20], patientId[20], doctorId[20], date[20], time[20], status[30];
    if (fp == NULL)
    {
        printf("\n[!] Error: Cannot find 'data/appointments.txt'\n");
        return;
    }

    int targetMonth;
    int scheduledCount = 0, rescheduleCount = 0, cancelledCount = 0;

    printf("\n====================================================\n");
    printf("           DIRECTOR: APPOINTMENT ANALYTICS          \n");
    printf("====================================================\n");
    printf("Enter Month to analyze (1-12): ");

    if (scanf("%d", &targetMonth) != 1)
    {
        printf("[!] Invalid input.\n");
        fclose(fp);
        while (getchar() != '\n');
        return;
    }

    while (getchar() != '\n');
    fscanf(fp, "%*[^\n]\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appId, patientId, doctorId, date, time, status) == 6)
    {
        int y, m, d;

        if (sscanf(date, "%d-%d-%d", &y, &m, &d) == 3)
        {
            if (m == targetMonth)
            {
                if (strcmp(status, "Scheduled") == 0) scheduledCount++;
                else if (strcmp(status, "Reschedule") == 0) rescheduleCount++;
                else if (strcmp(status, "Cancelled") == 0) cancelledCount++;
            }
        }
    }

    printf("\n----------------------------------------------------\n");
    printf(" REPORT FOR MONTH: %02d | STATUS SUMMARY             \n", targetMonth);
    printf("----------------------------------------------------\n");
    printf("  %-15s : %d\n", "Scheduled", scheduledCount);
    printf("  %-15s : %d\n", "Reschedule", rescheduleCount);
    printf("  %-15s : %d\n", "Cancelled", cancelledCount);
    printf("----------------------------------------------------\n");
    printf("  TOTAL ACTIVITY  : %d\n", (scheduledCount + rescheduleCount + cancelledCount));
    printf("====================================================\n");

    if ((scheduledCount + rescheduleCount + cancelledCount) > 0)
    {
        printf("\nVISUAL TREND:\n");
        printf("Sch: "); for (int i = 0; i < scheduledCount; i++) printf("#"); printf(" (%d)\n", scheduledCount);
        printf("Res: "); for (int i = 0; i < rescheduleCount; i++) printf("#"); printf(" (%d)\n", rescheduleCount);
        printf("Can: "); for (int i = 0; i < cancelledCount; i++) printf("#"); printf(" (%d)\n", cancelledCount);
    }
    else
    {
        printf("\n[i] No data found for Month %02d.\n", targetMonth);
    }
    printf("====================================================\n\n");

    fclose(fp);
}

void directorMenu(void)
{
    int choice;

    while (1)
    {
        printf("\n=====================================\n");
        printf("            DIRECTOR MENU            \n");
        printf("=====================================\n");
        printf("1. Manage Administrators\n");
        printf("2. Staff Report\n");
        printf("3. Appointment Trend Report\n");
        printf("0. Logout\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter 0-3.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1)
            manageAdministrators();
        else if (choice == 2)
            staffReport();
        else if (choice == 3)
            viewAppointmentTrends();
        else if (choice == 0)
        {
            printf("Logout successful.\n");
            return;
        }
        else
            printf("Invalid choice. Please enter 0-3.\n");
    }
}
