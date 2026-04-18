#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../headers/structures.h"
#include "../headers/patient.h"
#include "../headers/helper.h"

// --- HELPER FUNCTIONS ---

void trim(char *s)
{
    char *p;

    if (s == NULL)
    {
        return;
    }

    p = s + strlen(s);

    while (p > s && isspace((unsigned char)*(p - 1)))
    {
        p--;
        *p = '\0';
    }
}

void getTodayDate(char today[])
{
    time_t t = time(NULL);
    struct tm currentTime = *localtime(&t);

    sprintf(today, "%04d-%02d-%02d",
            currentTime.tm_year + 1900,
            currentTime.tm_mon + 1,
            currentTime.tm_mday);
}

int isAppointmentToday(char apptDate[])
{
    char today[12];
    char tempAppt[20];

    getTodayDate(today);
    strcpy(tempAppt, apptDate);
    trim(tempAppt);

    return strcmp(tempAppt, today) == 0;
}

int isPastAppointment(char apptDate[])
{
    char today[12];
    char tempAppt[20];

    getTodayDate(today);
    strcpy(tempAppt, apptDate);
    trim(tempAppt);

    return strcmp(tempAppt, today) < 0;
}

void writePatientNewLineIfNeeded(FILE *fp)
{
    long fileSize;

    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);

    if (fileSize > 0)
    {
        fseek(fp, -1, SEEK_END);
        if (fgetc(fp) != '\n')
        {
            fputc('\n', fp);
        }
    }

    fseek(fp, 0, SEEK_END);
}

int updateScheduleSlotStatus(char doctorId[], char date[], char timeStr[], char newStatus[])
{
    FILE *fp;
    FILE *ft;
    char line[250];
    char id[20], fileDate[20], fileTime[20], status[30];
    int updated = 0;

    fp = fopen("data/schedule.txt", "r");
    ft = fopen("data/temp_schedule.txt", "w");

    if (!fp || !ft)
    {
        if (fp) fclose(fp);
        if (ft) fclose(ft);
        return 0;
    }

    if (fgets(line, sizeof(line), fp) != NULL)
    {
        fputs(line, ft);
    }

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%29[^\n]", id, fileDate, fileTime, status) == 4)
    {
        trim(id);
        trim(fileDate);
        trim(fileTime);
        trim(status);

        if (strcmp(id, doctorId) == 0 &&
            strcmp(fileDate, date) == 0 &&
            strcmp(fileTime, timeStr) == 0)
        {
            fprintf(ft, "%s|%s|%s|%s\n", id, fileDate, fileTime, newStatus);
            updated = 1;
        }
        else
        {
            fprintf(ft, "%s|%s|%s|%s\n", id, fileDate, fileTime, status);
        }
    }

    fclose(fp);
    fclose(ft);

    remove("data/schedule.txt");
    rename("data/temp_schedule.txt", "data/schedule.txt");

    return updated;
}

int chooseAvailableScheduleSlot(char doctorId[], char date[], char timeStr[])
{
    FILE *fp;
    char fileDoctorId[100][20];
    char fileDate[100][20];
    char fileTime[100][20];
    char id[20], slotDate[20], slotTime[20], status[30];
    int count = 0;
    int choice;

    fp = fopen("data/schedule.txt", "r");
    if (!fp)
    {
        printf("\nFile error.\n");
        return 0;
    }

    printf("\n=====================================\n");
    printf("     AVAILABLE DOCTOR SCHEDULE       \n");
    printf("=====================================\n");
    printf("%-5s %-10s %-12s %-8s\n", "No.", "DoctorID", "Date", "Time");
    printf("----------------------------------------\n");

    fscanf(fp, "%*[^\n]\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%29[^\n]", id, slotDate, slotTime, status) == 4)
    {
        trim(status);

        if (strcmp(status, "Available") == 0 && count < 100)
        {
            strcpy(fileDoctorId[count], id);
            strcpy(fileDate[count], slotDate);
            strcpy(fileTime[count], slotTime);
            printf("%-5d %-10s %-12s %-8s\n", count + 1, id, slotDate, slotTime);
            count++;
        }
    }

    fclose(fp);

    if (count == 0)
    {
        printf("No available schedule slot found.\n");
        return 0;
    }

    while (1)
    {
        printf("Enter slot number: ");
        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter a number.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (choice >= 1 && choice <= count)
        {
            strcpy(doctorId, fileDoctorId[choice - 1]);
            strcpy(date, fileDate[choice - 1]);
            strcpy(timeStr, fileTime[choice - 1]);
            return 1;
        }

        printf("Invalid choice. Please try again.\n");
    }
}

int findAppointmentById(char currentPatientID[], char targetID[], char doctorId[], char date[], char timeStr[])
{
    FILE *fp;
    char line[250];
    char appointmentId[20], patientId[20], fileDoctorId[20], fileDate[20], fileTime[20], status[30];

    fp = fopen("data/appointments.txt", "r");
    if (!fp)
    {
        return 0;
    }

    fgets(line, sizeof(line), fp);

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appointmentId, patientId, fileDoctorId, fileDate, fileTime, status) == 6)
    {
        trim(appointmentId);
        trim(patientId);

        if (strcmp(appointmentId, targetID) == 0 && strcmp(patientId, currentPatientID) == 0)
        {
            strcpy(doctorId, fileDoctorId);
            strcpy(date, fileDate);
            strcpy(timeStr, fileTime);
            fclose(fp);
            return 1;
        }
    }

    fclose(fp);
    return 0;
}

/* --- PROACTIVE HEALTH ALERT --- */
void checkUrgentHealthAlerts(char currentPatientID[])
{
    FILE *fp = fopen("data/ehr.txt", "r");
    char patient_id[20], notes[150], line[300];
    int urgentFound = 0;

    if (!fp)
    {
        return;
    }

    fgets(line, sizeof(line), fp);

    while (fscanf(fp, " %*[^|]|%19[^|]|%*[^|]|%*[^|]|%*[^|]|%149[^\n]", patient_id, notes) == 2)
    {
        trim(patient_id);
        if (strcmp(patient_id, currentPatientID) == 0 &&
            (strstr(notes, "Urgent") || strstr(notes, "Critical")))
        {
            urgentFound = 1;
        }
    }
    fclose(fp);

    if (urgentFound)
    {
        printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        printf("\n! URGENT HEALTH ALERT: Action Required            !");
        printf("\n! Doctor flagged your record for immediate review.!");
        printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
    }
}

/* --- MANDATORY FEATURES --- */

void bookAppointment(char currentPatientID[])
{
    char doctorId[20], date[20], timeStr[10], appointmentId[20];
    FILE *fp;
    int nextNumber;

    printf("\n=====================================\n");
    printf("         BOOK APPOINTMENT            \n");
    printf("=====================================\n");

    if (!chooseAvailableScheduleSlot(doctorId, date, timeStr))
    {
        return;
    }

    nextNumber = generateNextNumber("data/appointments.txt", "APP");
    sprintf(appointmentId, "APP%03d", nextNumber);

    fp = fopen("data/appointments.txt", "a+");
    if (!fp)
    {
        printf("\nFile error.\n");
        return;
    }

    writePatientNewLineIfNeeded(fp);
    fprintf(fp, "%s|%s|%s|%s|%s|Scheduled", appointmentId, currentPatientID, doctorId, date, timeStr);
    fclose(fp);

    updateScheduleSlotStatus(doctorId, date, timeStr, "Booked");

    printf("\nGenerated Appointment ID: %s\n", appointmentId);
    printf("Appointment booked successfully.\n");
}

void viewAllAppointments(char currentPatientID[])
{
    FILE *fp;
    char line[250], appointmentId[20], patientId[20], doctorId[20], date[20], timeStr[10], status[30];
    int foundUpcoming = 0;
    int foundPast = 0;

    fp = fopen("data/appointments.txt", "r");
    if (!fp)
    {
        printf("\nFile error.\n");
        return;
    }

    printf("\n=====================================\n");
    printf("       UPCOMING APPOINTMENTS         \n");
    printf("=====================================\n");
    printf("%-10s %-10s %-12s %-8s %-12s\n", "Appt ID", "DoctorID", "Date", "Time", "Status");
    printf("-------------------------------------------------------------\n");

    fgets(line, sizeof(line), fp);
    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appointmentId, patientId, doctorId, date, timeStr, status) == 6)
    {
        trim(patientId);
        trim(date);
        trim(status);

        if (strcmp(patientId, currentPatientID) == 0 &&
            !isPastAppointment(date) &&
            strcmp(status, "Completed") != 0)
        {
            printf("%-10s %-10s %-12s %-8s %-12s\n", appointmentId, doctorId, date, timeStr, status);
            foundUpcoming = 1;
        }
    }

    if (!foundUpcoming)
    {
        printf("No upcoming appointments found.\n");
    }

    rewind(fp);
    fgets(line, sizeof(line), fp);

    printf("\n=====================================\n");
    printf("         PAST APPOINTMENTS           \n");
    printf("=====================================\n");
    printf("%-10s %-10s %-12s %-8s %-12s\n", "Appt ID", "DoctorID", "Date", "Time", "Status");
    printf("-------------------------------------------------------------\n");

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appointmentId, patientId, doctorId, date, timeStr, status) == 6)
    {
        trim(patientId);
        trim(date);
        trim(status);

        if (strcmp(patientId, currentPatientID) == 0 &&
            strcmp(status, "Completed") == 0)
        {
            printf("%-10s %-10s %-12s %-8s %-12s\n", appointmentId, doctorId, date, timeStr, status);
            foundPast = 1;
        }
    }

    if (!foundPast)
    {
        printf("No past appointments found.\n");
    }

    fclose(fp);
}

void rescheduleAppointment(char currentPatientID[])
{
    char targetID[20], oldDoctorId[20], oldDate[20], oldTime[10];
    char newDoctorId[20], newDate[20], newTime[10];
    FILE *fp;
    FILE *ft;
    char line[250], appointmentId[20], patientId[20], fileDoctorId[20], date[20], timeStr[10], status[30];
    int found = 0;

    printf("\nEnter Appt ID to Reschedule: ");
    scanf("%19s", targetID);

    if (!findAppointmentById(currentPatientID, targetID, oldDoctorId, oldDate, oldTime))
    {
        printf("\n[Error] Appointment ID not found.\n");
        return;
    }

    if (isAppointmentToday(oldDate))
    {
        printf("\n[Policy] Same-day rescheduling is NOT allowed.\n");
        return;
    }

    printf("\nChoose a new available slot.\n");
    if (!chooseAvailableScheduleSlot(newDoctorId, newDate, newTime))
    {
        return;
    }

    fp = fopen("data/appointments.txt", "r");
    ft = fopen("data/temp.txt", "w");

    if (!fp || !ft)
    {
        if (fp) fclose(fp);
        if (ft) fclose(ft);
        return;
    }

    if (fgets(line, sizeof(line), fp) != NULL)
    {
        fputs(line, ft);
    }

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appointmentId, patientId, fileDoctorId, date, timeStr, status) == 6)
    {
        trim(appointmentId);
        trim(patientId);

        if (strcmp(appointmentId, targetID) == 0 && strcmp(patientId, currentPatientID) == 0)
        {
            fprintf(ft, "%s|%s|%s|%s|%s|Reschedule\n",
                    appointmentId, patientId, newDoctorId, newDate, newTime);
            found = 1;
        }
        else
        {
            fprintf(ft, "%s|%s|%s|%s|%s|%s\n",
                    appointmentId, patientId, fileDoctorId, date, timeStr, status);
        }
    }

    fclose(fp);
    fclose(ft);

    remove("data/appointments.txt");
    rename("data/temp.txt", "data/appointments.txt");

    if (found)
    {
        updateScheduleSlotStatus(oldDoctorId, oldDate, oldTime, "Available");
        updateScheduleSlotStatus(newDoctorId, newDate, newTime, "Booked");
        printf("\n[Success] Appointment updated.\n");
    }
}

void cancelAppointment(char currentPatientID[])
{
    char targetID[20], doctorId[20], date[20], timeStr[10];
    FILE *fp;
    FILE *ft;
    char line[250], appointmentId[20], patientId[20], fileDoctorId[20], fileDate[20], fileTime[10], status[30];
    int found = 0;

    printf("\nEnter Appt ID to Cancel: ");
    scanf("%19s", targetID);

    if (!findAppointmentById(currentPatientID, targetID, doctorId, date, timeStr))
    {
        printf("\n[Error] Appointment ID not found.\n");
        return;
    }

    if (isAppointmentToday(date))
    {
        printf("\n[Policy] Same-day cancellation is NOT allowed.\n");
        return;
    }

    fp = fopen("data/appointments.txt", "r");
    ft = fopen("data/temp.txt", "w");

    if (!fp || !ft)
    {
        if (fp) fclose(fp);
        if (ft) fclose(ft);
        return;
    }

    if (fgets(line, sizeof(line), fp) != NULL)
    {
        fputs(line, ft);
    }

    while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                  appointmentId, patientId, fileDoctorId, fileDate, fileTime, status) == 6)
    {
        trim(appointmentId);
        trim(patientId);

        if (strcmp(appointmentId, targetID) == 0 && strcmp(patientId, currentPatientID) == 0)
        {
            fprintf(ft, "%s|%s|%s|%s|%s|Cancelled\n",
                    appointmentId, patientId, fileDoctorId, fileDate, fileTime);
            found = 1;
        }
        else
        {
            fprintf(ft, "%s|%s|%s|%s|%s|%s\n",
                    appointmentId, patientId, fileDoctorId, fileDate, fileTime, status);
        }
    }

    fclose(fp);
    fclose(ft);

    remove("data/appointments.txt");
    rename("data/temp.txt", "data/appointments.txt");

    if (found)
    {
        updateScheduleSlotStatus(doctorId, date, timeStr, "Available");
        printf("\n[Success] Appointment cancelled.\n");
    }
}

void viewEHR(char currentPatientID[])
{
    FILE *fp;
    char line[300], recordId[20], patientId[20], doctorId[20], diagnosis[100], prescription[100], notes[150];
    char appointmentId[20], date[20], timeStr[10], status[30];
    char billId[20], billingAppointmentId[20], billingPatientId[20], billingStatus[30];
    float amount;
    int foundRecord = 0;
    int foundAppointment = 0;
    int foundBill = 0;

    printf("\n=====================================\n");
    printf("          EHR SECURE VIEW            \n");
    printf("=====================================\n");

    fp = fopen("data/ehr.txt", "r");
    if (fp)
    {
        fgets(line, sizeof(line), fp);
        printf("\nMEDICAL HISTORY\n");
        printf("---------------------------------------------------------------\n");
        while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%99[^|]|%99[^|]|%149[^\n]",
                      recordId, patientId, doctorId, diagnosis, prescription, notes) == 6)
        {
            trim(patientId);
            if (strcmp(patientId, currentPatientID) == 0)
            {
                printf("Record ID    : %s\n", recordId);
                printf("Doctor ID    : %s\n", doctorId);
                printf("Diagnosis    : %s\n", diagnosis);
                printf("Prescription : %s\n", prescription);
                printf("Notes        : %s\n\n", notes);
                foundRecord = 1;
            }
        }
        fclose(fp);
    }

    if (!foundRecord)
    {
        printf("No medical record found.\n");
    }

    fp = fopen("data/appointments.txt", "r");
    if (fp)
    {
        fgets(line, sizeof(line), fp);
        printf("\nAPPOINTMENT HISTORY\n");
        printf("---------------------------------------------------------------\n");
        printf("%-10s %-10s %-12s %-8s %-12s\n", "Appt ID", "DoctorID", "Date", "Time", "Status");
        while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%19[^|]|%19[^|]|%29[^\n]",
                      appointmentId, patientId, doctorId, date, timeStr, status) == 6)
        {
            trim(patientId);
            if (strcmp(patientId, currentPatientID) == 0)
            {
                printf("%-10s %-10s %-12s %-8s %-12s\n", appointmentId, doctorId, date, timeStr, status);
                foundAppointment = 1;
            }
        }
        fclose(fp);
    }

    if (!foundAppointment)
    {
        printf("No appointment history found.\n");
    }

    fp = fopen("data/billing.txt", "r");
    if (fp)
    {
        fgets(line, sizeof(line), fp);
        printf("\nBILLING SUMMARY\n");
        printf("---------------------------------------------------------------\n");
        printf("%-8s %-10s %-10s %-10s\n", "Bill ID", "Appt ID", "Amount", "Status");
        while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%f|%29[^\n]",
                      billId, billingAppointmentId, billingPatientId, &amount, billingStatus) == 5)
        {
            trim(billingPatientId);
            if (strcmp(billingPatientId, currentPatientID) == 0)
            {
                printf("%-8s %-10s RM%-8.2f %-10s\n", billId, billingAppointmentId, amount, billingStatus);
                foundBill = 1;
            }
        }
        fclose(fp);
    }

    if (!foundBill)
    {
        printf("No billing summary found.\n");
    }
}

void billingManagement(char currentPatientID[])
{
    int choice;
    printf("\n=====================================\n");
    printf("         BILLING MANAGEMENT          \n");
    printf("=====================================\n");
    printf("1. Search Bills\n");
    printf("2. Update Payment Status\n");
    printf("Choice: ");
    scanf("%d", &choice);

    FILE *fp = fopen("data/billing.txt", "r");
    if (!fp) return;

    if (choice == 1)
    {
        char sID[20], bill_id[20], appointment_id[20], patient_id[20], stat[20], line[250];
        float amt;
        printf("Enter Bill ID (or 'all'): ");
        scanf("%19s", sID);
        fgets(line, sizeof(line), fp);
        while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%f|%19[^\n]", bill_id, appointment_id, patient_id, &amt, stat) == 5)
        {
            trim(patient_id);
            if (strcmp(patient_id, currentPatientID) == 0 &&
                (strcmp(sID, "all") == 0 || strcmp(sID, bill_id) == 0))
            {
                printf("\nBill: %s | Appt: %s | Amount: RM%.2f | Status: %s", bill_id, appointment_id, amt, stat);
            }
        }
        fclose(fp);
    }
    else
    {
        FILE *ft = fopen("data/temp_b.txt", "w");
        char bill_id[20], appointment_id[20], patient_id[20], stat[20], line[250], target[20], nStat[20];
        float amt;
        printf("Enter Bill ID and New Status (Paid/Unpaid): ");
        scanf("%19s %19s", target, nStat);
        fgets(line, sizeof(line), fp);
        fputs(line, ft);
        while (fscanf(fp, " %19[^|]|%19[^|]|%19[^|]|%f|%19[^\n]", bill_id, appointment_id, patient_id, &amt, stat) == 5)
        {
            trim(bill_id);
            trim(patient_id);
            if (strcmp(bill_id, target) == 0 && strcmp(patient_id, currentPatientID) == 0)
                fprintf(ft, "%s|%s|%s|%.2f|%s\n", bill_id, appointment_id, patient_id, amt, nStat);
            else
                fprintf(ft, "%s|%s|%s|%.2f|%s\n", bill_id, appointment_id, patient_id, amt, stat);
        }
        fclose(fp);
        fclose(ft);
        remove("data/billing.txt");
        rename("data/temp_b.txt", "data/billing.txt");
        printf("\n[Success] Billing record updated.");
    }
}

/* --- PATIENT MAIN MENU --- */
void patientMenu(char currentPatientID[])
{
    int choice;
    checkUrgentHealthAlerts(currentPatientID);

    do {
        printf("\n=====================================\n");
        printf("        PATIENT PORTAL: %s\n", currentPatientID);
        printf("=====================================\n");
        printf("\n1. Book Appointment");
        printf("\n2. View Appointments");
        printf("\n3. Reschedule Appointment");
        printf("\n4. Cancel Appointment");
        printf("\n5. View EHR (Medical Records)");
        printf("\n6. Billing Management");
        printf("\n7. Logout");
        printf("\nSelect: ");

        if (scanf("%d", &choice) != 1)
        {
            while (getchar() != '\n');
            continue;
        }

        switch(choice)
        {
            case 1: bookAppointment(currentPatientID); break;
            case 2: viewAllAppointments(currentPatientID); break;
            case 3: rescheduleAppointment(currentPatientID); break;
            case 4: cancelAppointment(currentPatientID); break;
            case 5: viewEHR(currentPatientID); break;
            case 6: billingManagement(currentPatientID); break;
        }
    } while (choice != 7);
}
