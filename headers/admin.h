#ifndef ADMIN_H
#define ADMIN_H

void adminMenu(char currentUserName[]);

void patientReportMenu(char currentUserName[]);
void viewPatients(char currentUserName[]);

void searchPatient(char currentUserName[]);
void searchPatientByID(char currentUserName[]);

void filterPatientsMenu(char currentUserName[]);
void filterByGender(char currentUserName[]);
void filterByAgeRange(char currentUserName[]);
void filterByName(char currentUserName[]);

#endif
