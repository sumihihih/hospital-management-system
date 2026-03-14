#ifndef STRUCTURES_H
#define STRUCTURES_H

// Core User Structure for Logins (Mandatory for all roles)
typedef struct {
    char id[15];
    char name[50];
    char password[20];
    char role[20]; // Director, Admin, Doctor, Staff Nurse, Patient
} User;

// Inventory Structure (Admin monitors, Nurse adds)
typedef struct {
    char item_name[50];
    char category[30];
    int stock_level;
} Inventory;

// Appointment Structure (Doctor manages, Patient views)
typedef struct {
    char appointment_id[10];
    char patient_id[15];
    char doctor_id[15];
    char date[11]; // YYYY-MM-DD
    char status[15]; // Scheduled, Rescheduled, Canceled
} Appointment;

#endif