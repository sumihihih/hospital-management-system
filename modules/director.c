#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/structures.h"
#include "../headers/director.h"

void viewAppointmentTrends() {
    
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
