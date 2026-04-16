#ifndef PATIENT_H
#define PATIENT_H

void trim(char *s);
int isAppointmentToday(char apptDate[]);

void checkUrgentHealthAlerts(char currentPatientID[]);
void bookAppointment(char currentPatientID[]);
void viewAllAppointments(char currentPatientID[]);
void rescheduleAppointment(char currentPatientID[]);
void cancelAppointment(char currentPatientID[]);
void viewEHR(char currentPatientID[]);
void billingManagement(char currentPatientID[]);

void patientMenu(char currentPatientID[]);

#endif