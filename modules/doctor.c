#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/doctor.h"
#include "../headers/structures.h"
#include "../headers/helper.h"

void writeDoctorNewLineIfNeeded(FILE *file);
int loadDoctorDepartment(char doctorId[], char department[]);
void waitForDoctorEnter(void);
void manageScheduleMenu(char doctorId[], char doctorName[], char doctorDepartment[]);
void viewSchedule(char doctorId[], char doctorName[], char doctorDepartment[]);
void addScheduleSlot(char doctorId[]);
void updateScheduleStatus(char doctorId[]);
int chooseScheduleStatus(char status[]);
void viewAppointments(char doctorId[], char doctorName[], char doctorDepartment[]);
void searchAppointment(char doctorId[]);
void ehrMenu(char doctorId[], char doctorName[], char doctorDepartment[]);
void viewPatientEHR(void);
void addEHR(char doctorId[]);
void reportMenu(char doctorId[], char doctorName[]);
void viewMyPatientReport(char doctorId[], char doctorName[]);
void searchReportByPatient(char doctorId[], char doctorName[]);
void filterReportByDateRange(char doctorId[], char doctorName[]);
void searchReportByDiagnosis(char doctorId[], char doctorName[]);

void writeDoctorNewLineIfNeeded(FILE *file)
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

void doctorMenu(char doctorId[], char doctorName[])
{
    char doctorDepartment[50];
    int choice;

    if (loadDoctorDepartment(doctorId, doctorDepartment) == 0)
    {
        printf("Doctor ID not found.\n");
        return;
    }

    while (1)
    {
        printf("\n===========================\n");
        printf("       DOCTOR MENU         \n");
        printf("===========================\n");
        printf("Doctor: Dr. %s\n", doctorName);
        printf("Department: %s\n", doctorDepartment);
        printf("1. Manage Schedule\n");
        printf("2. View My Appointments\n");
        printf("3. Search Appointment\n");
        printf("4. EHR Menu\n");
        printf("5. Reporting and Analytics\n");
        printf("0. Logout\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid input. Please enter 0-5.\n");
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1)
        {
            manageScheduleMenu(doctorId, doctorName, doctorDepartment);
        }
        else if (choice == 2)
        {
            viewAppointments(doctorId, doctorName, doctorDepartment);
        }
        else if (choice == 3)
        {
            searchAppointment(doctorId);
        }
        else if (choice == 4)
        {
            ehrMenu(doctorId, doctorName, doctorDepartment);
        }
        else if (choice == 5)
        {
            reportMenu(doctorId, doctorName);
        }
        else if (choice == 0)
        {
            printf("Logout successful.\n");
            return;
        }
        else
        {
            printf("Invalid choice. Please enter 0-5.\n");
        }
    }
}

int loadDoctorDepartment(char doctorId[], char doctorDepartment[])
{
    FILE *fp;
    char id[20], name[50], dept[50], spec[50];

    fp = fopen("data/doctors.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open doctors.txt\n");
        return 0;
    }

    fscanf(fp, "%*[^\n]\n");

    while (fscanf(fp, " %19[^|]|%49[^|]|%49[^|]|%49[^\n]", id, name, dept, spec) == 4)
    {
        if (strcmp(doctorId, id) == 0)
        {
            strcpy(doctorDepartment, dept);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

void waitForDoctorEnter(void)
{
    printf("\nPress Enter to go back...");
    getchar();
}

void manageScheduleMenu(char doctorId[], char doctorName[], char doctorDepartment[])
{
    int choice;

    while (1)
    {
        printf("\n===========================\n");
        printf("     MANAGE SCHEDULE       \n");
        printf("===========================\n");
        printf("Doctor: Dr. %s (%s)\n", doctorName, doctorDepartment);
        printf("1. View Schedule\n");
        printf("2. Add Schedule Slot\n");
        printf("3. Update Availability\n");
        printf("0. Go Back\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid input. Please enter 0-3.\n");
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1)
        {
            viewSchedule(doctorId, doctorName, doctorDepartment);
        }
        else if (choice == 2)
        {
            addScheduleSlot(doctorId);
        }
        else if (choice == 3)
        {
            updateScheduleStatus(doctorId);
        }
        else if (choice == 0)
        {
            return;
        }
        else
        {
            printf("Invalid choice. Please enter 0-3.\n");
        }
    }
}

void viewSchedule(char doctorId[], char doctorName[], char doctorDepartment[])
{
    FILE *fp;
    char id[20], date[20], time[20], status[30];
    int found = 0;

    fp = fopen("data/schedule.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open schedule.txt\n");
        return;
    }

    printf("\n--- Schedule for Dr. %s (%s) ---\n", doctorName, doctorDepartment);
    printf("%-15s %-15s %-20s\n", "Date", "Time", "Status");
    printf("--------------------------------------------\n");

    fscanf(fp, "%*[^\n]\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%29[^\n]", id, date, time, status) == 4)
    {
        if (strcmp(id, doctorId) == 0)
        {
            printf("%-15s %-15s %-20s\n", date, time, status);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No schedule found for Doctor ID: %s\n", doctorId);
    }

    fclose(fp);
    waitForDoctorEnter();
}

int chooseScheduleStatus(char status[])
{
    int choice;

    while (1)
    {
        printf("1. Available\n");
        printf("2. Booked\n");
        printf("3. Unavailable\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid input. Please enter 1-3.\n");
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1)
        {
            strcpy(status, "Available");
            return 1;
        }
        else if (choice == 2)
        {
            strcpy(status, "Booked");
            return 1;
        }
        else if (choice == 3)
        {
            strcpy(status, "Unavailable");
            return 1;
        }

        printf("Invalid choice. Please enter 1-3.\n");
    }
}

void addScheduleSlot(char doctorId[])
{
    FILE *fp;
    FILE *checkFile;
    char checkDoctorId[20], checkDate[20], checkTime[20], checkStatus[30];
    char date[20], time[20], status[30];

    fp = fopen("data/schedule.txt", "a+");
    if (fp == NULL)
    {
        printf("Cannot open schedule.txt\n");
        return;
    }

    printf("Enter Date (YYYY-MM-DD): ");
    scanf(" %19s", date);
    printf("Enter Time (HH:MM): ");
    scanf(" %19s", time);
    while (getchar() != '\n');

    checkFile = fopen("data/schedule.txt", "r");
    if (checkFile != NULL)
    {
        fscanf(checkFile, "%*[^\n]\n");

        while (fscanf(checkFile, " %19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                      checkDoctorId, checkDate, checkTime, checkStatus) == 4)
        {
            if (strcmp(checkDoctorId, doctorId) == 0 &&
                strcmp(checkDate, date) == 0 &&
                strcmp(checkTime, time) == 0)
            {
                fclose(checkFile);
                fclose(fp);
                printf("This schedule slot already exists.\n");
                return;
            }
        }

        fclose(checkFile);
    }

    printf("Choose Availability Status:\n");
    chooseScheduleStatus(status);

    writeDoctorNewLineIfNeeded(fp);
    fprintf(fp, "%s|%s|%s|%s", doctorId, date, time, status);
    fclose(fp);

    printf("Schedule slot added successfully.\n");
}

void updateScheduleStatus(char doctorId[])
{
    FILE *fp;
    FILE *temp;
    char id[20], date[20], time[20], status[30];
    char targetDate[20], targetTime[20], newStatus[30];
    char header[100];
    int found = 0;

    fp = fopen("data/schedule.txt", "r");
    temp = fopen("data/temp_schedule.txt", "w");

    if (fp == NULL || temp == NULL)
    {
        printf("Cannot open schedule.txt\n");
        if (fp != NULL) fclose(fp);
        if (temp != NULL) fclose(temp);
        return;
    }

    if (fgets(header, sizeof(header), fp) != NULL)
    {
        fputs(header, temp);
    }

    printf("Enter Date to update (YYYY-MM-DD): ");
    scanf(" %19s", targetDate);
    printf("Enter Time to update (HH:MM): ");
    scanf(" %19s", targetTime);
    while (getchar() != '\n');

    printf("Choose New Availability Status:\n");
    chooseScheduleStatus(newStatus);

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%29[^\n]", id, date, time, status) == 4)
    {
        if (strcmp(id, doctorId) == 0 && strcmp(date, targetDate) == 0 && strcmp(time, targetTime) == 0)
        {
            fprintf(temp, "%s|%s|%s|%s\n", id, date, time, newStatus);
            found = 1;
        }
        else
        {
            fprintf(temp, "%s|%s|%s|%s\n", id, date, time, status);
        }
    }

    fclose(fp);
    fclose(temp);

    remove("data/schedule.txt");
    rename("data/temp_schedule.txt", "data/schedule.txt");

    if (found)
    {
        printf("Schedule updated successfully.\n");
    }
    else
    {
        printf("Schedule slot not found.\n");
    }
}

void viewAppointments(char doctorId[], char doctorName[], char doctorDepartment[])
{
    FILE *fp;
    char appId[20], patientId[20], id[20], date[20], time[20], status[30];
    int found = 0;

    fp = fopen("data/appointments.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open appointments.txt\n");
        return;
    }

    printf("\n--- Appointments for Dr. %s (%s) ---\n", doctorName, doctorDepartment);
    printf("%-10s %-12s %-12s %-8s %-15s\n", "App ID", "Patient ID", "Date", "Time", "Status");
    printf("---------------------------------------------------------------\n");

    fscanf(fp, "%*[^\n]\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appId, patientId, id, date, time, status) == 6)
    {
        if (strcmp(id, doctorId) == 0)
        {
            printf("%-10s %-12s %-12s %-8s %-15s\n", appId, patientId, date, time, status);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No appointments found for Doctor ID: %s\n", doctorId);
    }

    fclose(fp);
    waitForDoctorEnter();
}

void searchAppointment(char doctorId[])
{
    FILE *fp;
    char appId[20], patientId[20], fileDoctorId[20], date[20], time[20], status[30];
    char search[20];
    int found = 0;

    fp = fopen("data/appointments.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open appointments.txt\n");
        return;
    }

    printf("Enter Appointment ID or Patient ID to search: ");
    scanf(" %19s", search);
    while (getchar() != '\n');

    printf("\n--- Search Result ---\n");
    printf("%-10s %-12s %-12s %-8s %-15s\n", "App ID", "Patient ID", "Date", "Time", "Status");
    printf("---------------------------------------------------------------\n");

    fscanf(fp, "%*[^\n]\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appId, patientId, fileDoctorId, date, time, status) == 6)
    {
        if (strcmp(fileDoctorId, doctorId) == 0 &&
            (strcmp(appId, search) == 0 || strcmp(patientId, search) == 0))
        {
            printf("%-10s %-12s %-12s %-8s %-15s\n", appId, patientId, date, time, status);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No record found.\n");
    }

    fclose(fp);
    waitForDoctorEnter();
}

void ehrMenu(char doctorId[], char doctorName[], char doctorDepartment[])
{
    int choice;

    do
    {
        printf("\n===========================\n");
        printf("         EHR MENU          \n");
        printf("===========================\n");
        printf("Doctor: Dr. %s\n", doctorName);
        printf("1. View Patient EHR\n");
        printf("2. Add EHR\n");
        printf("0. Go Back\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid input. Please enter 0-2.\n");
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1)
        {
            viewPatientEHR();
        }
        else if (choice == 2)
        {
            addEHR(doctorId);
        }
        else if (choice == 0)
        {
            return;
        }
        else
        {
            printf("Invalid choice. Please enter 0-2.\n");
        }
    } while (1);
}

void viewPatientEHR(void)
{
    FILE *fp;
    char ehrId[20], patientId[20], doctorId[20];
    char diagnosis[50], prescription[50], notes[100];
    char searchId[20];
    int found = 0;

    fp = fopen("data/ehr.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open ehr.txt\n");
        return;
    }

    printf("Enter Patient ID: ");
    scanf(" %19s", searchId);
    while (getchar() != '\n');

    fscanf(fp, "%*[^\n]\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%49[^|]|%49[^|]|%99[^\n]",
                  ehrId, patientId, doctorId, diagnosis, prescription, notes) == 6)
    {
        if (strcmp(patientId, searchId) == 0)
        {
            printf("\n--- Patient EHR ---\n");
            printf("EHR ID       : %s\n", ehrId);
            printf("Patient ID   : %s\n", patientId);
            printf("Doctor ID    : %s\n", doctorId);
            printf("Diagnosis    : %s\n", diagnosis);
            printf("Prescription : %s\n", prescription);
            printf("Notes        : %s\n", notes);
            printf("-------------------\n");
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No EHR found for Patient ID: %s\n", searchId);
    }

    fclose(fp);
    waitForDoctorEnter();
}

void addEHR(char doctorId[])
{
    FILE *fp;
    char ehrId[20], patientId[20], diagnosis[50], prescription[50], notes[100];
    int nextNum;

    nextNum = generateNextNumber("data/ehr.txt", "EHR");
    sprintf(ehrId, "EHR%03d", nextNum);
    printf("Generated EHR ID: %s\n", ehrId);

    printf("Enter Patient ID: ");
    scanf(" %19s", patientId);
    printf("Enter Diagnosis: ");
    scanf(" %49[^\n]", diagnosis);
    printf("Enter Prescription: ");
    scanf(" %49[^\n]", prescription);
    printf("Enter Notes: ");
    scanf(" %99[^\n]", notes);
    while (getchar() != '\n');

    fp = fopen("data/ehr.txt", "a+");
    if (fp == NULL)
    {
        printf("Cannot open ehr.txt\n");
        return;
    }

    writeDoctorNewLineIfNeeded(fp);
    fprintf(fp, "%s|%s|%s|%s|%s|%s", ehrId, patientId, doctorId, diagnosis, prescription, notes);

    fclose(fp);
    printf("EHR added successfully.\n");
}

void reportMenu(char doctorId[], char doctorName[])
{
    int choice;

    while (1)
    {
        printf("\n===========================\n");
        printf("   REPORTING & ANALYTICS   \n");
        printf("===========================\n");
        printf("Doctor: Dr. %s\n", doctorName);
        printf("1. View My Patient Report\n");
        printf("2. Search Report By Patient ID\n");
        printf("3. Filter Report By Date Range\n");
        printf("4. Search Report By Diagnosis\n");
        printf("0. Go Back\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            printf("Invalid input. Please enter 0-4.\n");
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1)
        {
            viewMyPatientReport(doctorId, doctorName);
        }
        else if (choice == 2)
        {
            searchReportByPatient(doctorId, doctorName);
        }
        else if (choice == 3)
        {
            filterReportByDateRange(doctorId, doctorName);
        }
        else if (choice == 4)
        {
            searchReportByDiagnosis(doctorId, doctorName);
        }
        else if (choice == 0)
        {
            return;
        }
        else
        {
            printf("Invalid choice. Please enter 0-4.\n");
        }
    }
}

void viewMyPatientReport(char doctorId[], char doctorName[])
{
    FILE *fp;
    char appId[20], patientId[20], fileDoctorId[20], date[20], time[20], status[30];
    int found = 0;

    fp = fopen("data/appointments.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open appointments.txt\n");
        return;
    }

    printf("\n--- My Patient Report: Dr. %s ---\n", doctorName);
    printf("%-10s %-12s %-12s %-8s %-15s\n", "App ID", "Patient ID", "Date", "Time", "Status");
    printf("---------------------------------------------------------------\n");

    fscanf(fp, "%*[^\n]\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appId, patientId, fileDoctorId, date, time, status) == 6)
    {
        if (strcmp(fileDoctorId, doctorId) == 0)
        {
            printf("%-10s %-12s %-12s %-8s %-15s\n", appId, patientId, date, time, status);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No report data found.\n");
    }

    fclose(fp);
    waitForDoctorEnter();
}

void searchReportByPatient(char doctorId[], char doctorName[])
{
    FILE *fp;
    char appId[20], patientId[20], fileDoctorId[20], date[20], time[20], status[30];
    char searchPatientId[20];
    int found = 0;

    fp = fopen("data/appointments.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open appointments.txt\n");
        return;
    }

    printf("Enter Patient ID: ");
    scanf(" %19s", searchPatientId);
    while (getchar() != '\n');

    printf("\n--- Report By Patient ID: %s (Dr. %s) ---\n", searchPatientId, doctorName);
    printf("%-10s %-12s %-12s %-8s %-15s\n", "App ID", "Patient ID", "Date", "Time", "Status");
    printf("---------------------------------------------------------------\n");

    fscanf(fp, "%*[^\n]\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appId, patientId, fileDoctorId, date, time, status) == 6)
    {
        if (strcmp(fileDoctorId, doctorId) == 0 && strcmp(patientId, searchPatientId) == 0)
        {
            printf("%-10s %-12s %-12s %-8s %-15s\n", appId, patientId, date, time, status);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No report data found for this patient.\n");
    }

    fclose(fp);
    waitForDoctorEnter();
}

void filterReportByDateRange(char doctorId[], char doctorName[])
{
    FILE *fp;
    char appId[20], patientId[20], fileDoctorId[20], date[20], time[20], status[30];
    char startDate[20], endDate[20];
    int found = 0;

    fp = fopen("data/appointments.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open appointments.txt\n");
        return;
    }

    printf("Enter Start Date (YYYY-MM-DD): ");
    scanf(" %19s", startDate);
    printf("Enter End Date (YYYY-MM-DD): ");
    scanf(" %19s", endDate);
    while (getchar() != '\n');

    printf("\n--- Report By Date Range for Dr. %s ---\n", doctorName);
    printf("%-10s %-12s %-12s %-8s %-15s\n", "App ID", "Patient ID", "Date", "Time", "Status");
    printf("---------------------------------------------------------------\n");

    fscanf(fp, "%*[^\n]\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appId, patientId, fileDoctorId, date, time, status) == 6)
    {
        if (strcmp(fileDoctorId, doctorId) == 0 &&
            strcmp(date, startDate) >= 0 && strcmp(date, endDate) <= 0)
        {
            printf("%-10s %-12s %-12s %-8s %-15s\n", appId, patientId, date, time, status);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No report data found in this date range.\n");
    }

    fclose(fp);
    waitForDoctorEnter();
}

void searchReportByDiagnosis(char doctorId[], char doctorName[])
{
    FILE *fp;
    char ehrId[20], patientId[20], fileDoctorId[20];
    char diagnosis[50], prescription[50], notes[100];
    char searchDiagnosis[50], diagnosisLower[50];
    int found = 0;

    fp = fopen("data/ehr.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open ehr.txt\n");
        return;
    }

    printf("Enter Diagnosis: ");
    scanf(" %49[^\n]", searchDiagnosis);
    while (getchar() != '\n');
    toLowerCase(searchDiagnosis);

    printf("\n--- Report By Diagnosis for Dr. %s ---\n", doctorName);
    printf("%-10s %-12s %-20s %-20s\n", "EHR ID", "Patient ID", "Diagnosis", "Prescription");
    printf("----------------------------------------------------------------\n");

    fscanf(fp, "%*[^\n]\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%49[^|]|%49[^|]|%99[^\n]",
                  ehrId, patientId, fileDoctorId, diagnosis, prescription, notes) == 6)
    {
        strcpy(diagnosisLower, diagnosis);
        toLowerCase(diagnosisLower);

        if (strcmp(fileDoctorId, doctorId) == 0 && strcmp(diagnosisLower, searchDiagnosis) == 0)
        {
            printf("%-10s %-12s %-20s %-20s\n", ehrId, patientId, diagnosis, prescription);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No report data found for this diagnosis.\n");
    }

    fclose(fp);
    waitForDoctorEnter();
}
