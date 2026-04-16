#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include "../headers/structures.h"
#include "../headers/patient.h"


// --- HELPER FUNCTIONS ---

void trim(char *s) {
    if (s == NULL) return;
    char *p = s + strlen(s);
    while (p > s && isspace((unsigned char)*--p)) *p = '\0';
}

int isAppointmentToday(char apptDate[]) {
    char today[12];
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    sprintf(today, "%04d-%02d-%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday);
    
    char tempAppt[20];
    strcpy(tempAppt, apptDate);
    trim(tempAppt);
    
    return (strcmp(tempAppt, today) == 0);
}

/* --- PROACTIVE HEALTH ALERT --- */
void checkUrgentHealthAlerts(char currentPatientID[]) {
    FILE *fp = fopen("data/ehr.txt", "r");
    if (!fp) return;
    
    char patient_id[20], notes[150], line[300];
    int urgentFound = 0;

    fgets(line, sizeof(line), fp); 
    while (fscanf(fp, " %*[^|]|%[^|]|%*[^|]|%*[^|]|%*[^|]|%[^\n]", patient_id, notes) == 2) {
        trim(patient_id);
        if (strcmp(patient_id, currentPatientID) == 0 && (strstr(notes, "Urgent") || strstr(notes, "Critical"))) {
            urgentFound = 1;
        }
    }
    fclose(fp);

    if (urgentFound) {
        printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
        printf("\n! URGENT HEALTH ALERT: Action Required            !");
        printf("\n! Doctor flagged your record for immediate review.!");
        printf("\n!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n");
        fflush(stdout);
    }
}

/* --- MANDATORY FEATURES --- */

void bookAppointment(char currentPatientID[]) {
    char doctor_id[20], date[20], timeStr[10], appointment_id[20];
    printf("\n--- Book Appointment ---");
    printf("\nEnter Appt ID: "); scanf("%s", appointment_id);
    printf("Enter Doctor ID: "); scanf("%s", doctor_id);
    printf("Enter Date (YYYY-MM-DD): "); scanf("%s", date);
    printf("Enter Time (HH:MM): "); scanf("%s", timeStr);

    FILE *fp = fopen("data/appointments.txt", "a");
    if (fp) {
        fprintf(fp, "%s|%s|%s|%s|%s|Scheduled\n", appointment_id, currentPatientID, doctor_id, date, timeStr);
        fclose(fp);
        printf("\n[Success] Appointment saved.");
    }
    fflush(stdout);
}

void viewAllAppointments(char currentPatientID[]) {
    FILE *fp = fopen("data/appointments.txt", "r");
    if (!fp) { printf("\nFile error."); return; }
    char line[250], appointment_id[20], patient_id[20], doctor_id[20], date[20], timeStr[10], stat[20];
    
    printf("\n%-12s | %-12s | %-8s | %-10s", "Appt ID", "Date", "Time", "Status");
    printf("\n---------------------------------------------------------");
    
    fgets(line, sizeof(line), fp); 
    while (fscanf(fp, " %[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]", appointment_id, patient_id, doctor_id, date, timeStr, stat) == 6) {
        trim(patient_id);
        if (strcmp(patient_id, currentPatientID) == 0) {
            printf("\n%-12s | %-12s | %-8s | %-10s", appointment_id, date, timeStr, stat);
        }
    }
    fclose(fp);
    printf("\n");
    fflush(stdout);
}

void rescheduleAppointment(char currentPatientID[]) {
    char targetID[20], nDate[20], nTime[10];
    printf("\nEnter Appt ID to Reschedule: "); 
    fflush(stdout);
    scanf("%s", targetID);

    FILE *fp = fopen("data/appointments.txt", "r");
    FILE *ft = fopen("data/temp.txt", "w");
    char line[250], appointment_id[20], patient_id[20], doctor_id[20], date[20], timeStr[10], stat[20];
    int found = 0, policyTriggered = 0;

    if (!fp) return;
    fgets(line, sizeof(line), fp); fputs(line, ft);

    while (fscanf(fp, " %[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]", appointment_id, patient_id, doctor_id, date, timeStr, stat) == 6) {
        trim(appointment_id); trim(patient_id);
        if (strcmp(appointment_id, targetID) == 0 && strcmp(patient_id, currentPatientID) == 0) {
            if (isAppointmentToday(date)) {
                policyTriggered = 1;
                fprintf(ft, "%s|%s|%s|%s|%s|%s\n", appointment_id, patient_id, doctor_id, date, timeStr, stat);
            } else {
                printf("Enter New Date (YYYY-MM-DD): "); scanf("%s", nDate);
                printf("Enter New Time (HH:MM): "); scanf("%s", nTime);
                fprintf(ft, "%s|%s|%s|%s|%s|Rescheduled\n", appointment_id, patient_id, doctor_id, nDate, nTime);
                found = 1;
            }
        } else {
            fprintf(ft, "%s|%s|%s|%s|%s|%s\n", appointment_id, patient_id, doctor_id, date, timeStr, stat);
        }
    }
    fclose(fp); fclose(ft);
    remove("data/appointments.txt"); rename("data/temp.txt", "data/appointments.txt");

    if (policyTriggered) printf("\n[Policy] Same-day rescheduling is NOT allowed.");
    else if (found) printf("\n[Success] Appointment updated.");
    else printf("\n[Error] Appointment ID not found.");
    fflush(stdout);
}

void cancelAppointment(char currentPatientID[]) {
    char targetID[20];
    printf("\nEnter Appt ID to Cancel: "); 
    fflush(stdout);
    scanf("%s", targetID);

    FILE *fp = fopen("data/appointments.txt", "r");
    FILE *ft = fopen("data/temp.txt", "w");
    char line[250], appointment_id[20], patient_id[20], doctor_id[20], date[20], timeStr[10], stat[20];
    int found = 0, policyTriggered = 0;

    if (!fp) return;
    fgets(line, sizeof(line), fp); fputs(line, ft);

    while (fscanf(fp, " %[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]", appointment_id, patient_id, doctor_id, date, timeStr, stat) == 6) {
        trim(appointment_id); trim(patient_id);
        if (strcmp(appointment_id, targetID) == 0 && strcmp(patient_id, currentPatientID) == 0) {
            if (isAppointmentToday(date)) {
                policyTriggered = 1;
                fprintf(ft, "%s|%s|%s|%s|%s|%s\n", appointment_id, patient_id, doctor_id, date, timeStr, stat);
            } else {
                fprintf(ft, "%s|%s|%s|%s|%s|Cancelled\n", appointment_id, patient_id, doctor_id, date, timeStr);
                found = 1;
            }
        } else {
            fprintf(ft, "%s|%s|%s|%s|%s|%s\n", appointment_id, patient_id, doctor_id, date, timeStr, stat);
        }
    }
    fclose(fp); fclose(ft);
    remove("data/appointments.txt"); rename("data/temp.txt", "data/appointments.txt");

    if (policyTriggered) printf("\n[Policy] Same-day cancellation is NOT allowed.");
    else if (found) printf("\n[Success] Appointment cancelled.");
    else printf("\n[Error] Appointment ID not found.");
    fflush(stdout);
}

void viewEHR(char currentPatientID[]) {
    FILE *fp = fopen("data/ehr.txt", "r");
    char line[300], record_id[20], patient_id[20], doctor_id[20], diag[100], pres[100], notes[150];
    printf("\n--- EHR SECURE VIEW ---");
    if (!fp) return;
    fgets(line, sizeof(line), fp);
    while (fscanf(fp, " %[^|]|%[^|]|%[^|]|%[^|]|%[^|]|%[^\n]", record_id, patient_id, doctor_id, diag, pres, notes) == 6) {
        trim(patient_id);
        if (strcmp(patient_id, currentPatientID) == 0)
            printf("\nID: %s | Diagnosis: %s | Prescription: %s | Notes: %s", record_id, diag, pres, notes);
    }
    fclose(fp);
    printf("\n");
    fflush(stdout);
}

void billingManagement(char currentPatientID[]) {
    int choice;
    printf("\n--- Billing ---\n1. Search Bills\n2. Update Payment Status\nChoice: ");
    fflush(stdout);
    scanf("%d", &choice);

    FILE *fp = fopen("data/billing.txt", "r");
    if (!fp) return;
    
    if (choice == 1) {
        char sID[20], bill_id[20], appointment_id[20], patient_id[20], stat[20], line[250]; float amt;
        printf("Enter Bill ID (or 'all'): "); scanf("%s", sID);
        fgets(line, sizeof(line), fp);
        while (fscanf(fp, " %[^|]|%[^|]|%[^|]|%f|%[^\n]", bill_id, appointment_id, patient_id, &amt, stat) == 5) {
            trim(patient_id);
            if (strcmp(patient_id, currentPatientID) == 0 && (strcmp(sID, "all") == 0 || strcmp(sID, bill_id) == 0))
                printf("\nBill: %s | Appt: %s | Amount: $%.2f | Status: %s", bill_id, appointment_id, amt, stat);
        }
        fclose(fp);
    } else {
        FILE *ft = fopen("data/temp_b.txt", "w");
        char bill_id[20], appointment_id[20], patient_id[20], stat[20], line[250], target[20], nStat[20]; float amt;
        printf("Enter Bill ID and New Status (Paid/Unpaid): "); scanf("%s %s", target, nStat);
        fgets(line, sizeof(line), fp); fputs(line, ft);
        while (fscanf(fp, " %[^|]|%[^|]|%[^|]|%f|%[^\n]", bill_id, appointment_id, patient_id, &amt, stat) == 5) {
            trim(bill_id); trim(patient_id);
            if (strcmp(bill_id, target) == 0 && strcmp(patient_id, currentPatientID) == 0)
                fprintf(ft, "%s|%s|%s|%.2f|%s\n", bill_id, appointment_id, patient_id, amt, nStat);
            else fprintf(ft, "%s|%s|%s|%.2f|%s\n", bill_id, appointment_id, patient_id, amt, stat);
        }
        fclose(fp); fclose(ft);
        remove("data/billing.txt"); rename("data/temp_b.txt", "data/billing.txt");
        printf("\n[Success] Billing record updated.");
    }
    fflush(stdout);
}

/* --- PATIENT MAIN MENU --- */
void patientMenu(char currentPatientID[]) {
    int choice;
    checkUrgentHealthAlerts(currentPatientID); 

    do {
        printf("\n--- PATIENT PORTAL: %s ---", currentPatientID);
        printf("\n1. Book Appointment");
        printf("\n2. View Appointments");
        printf("\n3. Reschedule Appointment");
        printf("\n4. Cancel Appointment");
        printf("\n5. View EHR (Medical Records)");
        printf("\n6. Billing Management");
        printf("\n7. Logout");
        printf("\nSelect: ");
        fflush(stdout);

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n'); 
            continue;
        }

        switch(choice) {
            case 1: bookAppointment(currentPatientID); break;
            case 2: viewAllAppointments(currentPatientID); break;
            case 3: rescheduleAppointment(currentPatientID); break;
            case 4: cancelAppointment(currentPatientID); break;
            case 5: viewEHR(currentPatientID); break;
            case 6: billingManagement(currentPatientID); break;
        }
    } while (choice != 7);
}