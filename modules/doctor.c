#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/doctor.h"
#include "../headers/structures.h"
#include "../headers/helper.h"

int loadDoctorDepartment(char doctorId[], char department[]);
void viewSchedule(char doctorId[], char doctorName[], char doctorDepartment[]);
void viewAppointments(char doctorId[], char doctorName[], char doctorDepartment[]);
void searchAppointment();
void ehrMenu(char doctorId[], char doctorName[], char doctorDepartment[]);
void viewPatientEHR();
void addEHR(char doctorId[]);

// Doctor Menu
void doctorMenu(char doctorId[], char doctorName[])
{
    char doctorDepartment[50];
    int choice;

    if (loadDoctorDepartment(doctorId, doctorDepartment) == 0)
    {
        printf("Doctor ID not found.\n");
        return;
    }

    do
    {
        printf("\n===========================\n");
        printf("       DOCTOR MENU         \n");
        printf("===========================\n");
        printf("Doctor: Dr. %s\n", doctorName);
        printf("Department: %s\n", doctorDepartment);
        printf("1. View Schedule\n");
        printf("2. View My Appointments\n");
        printf("3. Search Appointment\n");
        printf("4. EHR Menu\n");
        printf("5. Logout\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Invalid input. Please enter a number.\n");
            choice = 0;
            continue;
        }

        switch (choice)
        {
        case 1:
            viewSchedule(doctorId, doctorName, doctorDepartment);
            break;
        case 2:
            viewAppointments(doctorId, doctorName, doctorDepartment);
            break;
        case 3:
            searchAppointment();
            break;
        case 4:
            ehrMenu(doctorId, doctorName, doctorDepartment);
            break;
        case 5:
            printf("Logout successful.\n");
            break;
        default:
            printf("Invalid choice. Please enter 1-5.\n");
        }
    } while (choice != 5);

    return;
}

// Loads the logged-in doctor's department from doctors.txt
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

// View Schedule for logged in doctor
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
}

// View Appointments fir the logged in doctor
void viewAppointments(char doctorId[], char doctorName[], char doctorDepartment[])
{
    FILE *fp;
    char appId[20], patientId[20], id[20], date[20], status[30];
    int found = 0;

    fp = fopen("data/appointments.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open appointments.txt\n");
        return;
    }

    printf("\n--- Appointments for Dr. %s (%s) ---\n", doctorName, doctorDepartment);
    printf("%-15s %-15s %-15s %-20s\n", "App ID", "Patient ID", "Date", "Status");
    printf("------------------------------------------------------------\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appId, patientId, id, date, status) == 5)
    {
        if (strcmp(id, doctorId) == 0)
        {
            printf("%-15s %-15s %-15s %-20s\n", appId, patientId, date, status);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No appointments found for Doctor ID: %s\n", doctorId);
    }

    fclose(fp);
}

// Search Appointments
void searchAppointment()
{
    FILE *fp;
    char appId[20], patientId[20], doctorId[20], date[20], status[30];
    char search[20];
    int found = 0;

    fp = fopen("data/appointments.txt", "r");
    if (fp == NULL)
    {
        printf("Cannot open appointments.txt\n");
        return;
    }

    printf("Enter Appointment ID or Patient ID to search: ");
    scanf("%19s", search);

    printf("\n--- Search Result ---\n");
    printf("%-15s %-15s %-15s %-15s %-20s\n", "App ID", "Patient ID", "Doctor ID", "Date", "Status");
    printf("------------------------------------------------------------------------\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appId, patientId, doctorId, date, status) == 5)
    {
        if (strcmp(appId, search) == 0 || strcmp(patientId, search) == 0)
        {
            printf("%-15s %-15s %-15s %-15s %-20s\n",
                   appId, patientId, doctorId, date, status);
            found = 1;
        }
    }

    if (found == 0)
    {
        printf("No record found for: %s\n", search);
    }

    fclose(fp);
}
// EHR Menu
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
        printf("3. Back to Main Menu\n");
        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n')
                ;
            printf("Invalid input. Please enter a number.\n");
            choice = 0;
            continue;
        }

        switch (choice)
        {
        case 1:
            viewPatientEHR();
            break;
        case 2:
            addEHR(doctorId);
            break;
        case 3:
            printf("Returning to main menu...\n");
            break;
        default:
            printf("Invalid choice. Please enter 1-3.\n");
        }
    } while (choice != 3);
}

void viewPatientEHR()
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
    scanf("%19s", searchId);

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
    scanf("%19s", patientId);
    printf("Enter Diagnosis: ");
    scanf(" %49[^\n]", diagnosis);
    printf("Enter Prescription: ");
    scanf(" %49[^\n]", prescription);
    printf("Enter Notes: ");
    scanf(" %99[^\n]", notes);

    fp = fopen("data/ehr.txt", "a");
    if (fp == NULL)
    {
        printf("Cannot open ehr.txt\n");
        return;
    }

    fprintf(fp, "%s|%s|%s|%s|%s|%s\n",
            ehrId, patientId, doctorId, diagnosis, prescription, notes);

    fclose(fp);
    printf("EHR added successfully.\n");
}
