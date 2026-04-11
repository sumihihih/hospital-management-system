#ifndef STRUCTURES_H
#define STRUCTURES_H

// ================= USER (LOGIN) =================
typedef struct {
    char id[15];
    char name[50];
    char password[20];
    char role[20];
} User;


// ================= ADMIN =================
typedef struct {
    char admin_id[15];
    char name[50];
} Admin;


// ================= PATIENT =================
typedef struct {
    char patient_id[15];
    char name[50];
    int age;
    char gender[10];
    char contact[20];
} Patient;


// ================= DOCTOR =================
typedef struct {
    char doctor_id[15];
    char name[50];
    char department[50];
    char specialization[50];
} Doctor;


// ================= NURSE =================
typedef struct {
    char nurse_id[15];
    char name[50];
    char department[50];
} Nurse;


// ================= APPOINTMENT =================
typedef struct {
    char appointment_id[10];
    char patient_id[15];
    char doctor_id[15];
    char date[11];   // YYYY-MM-DD
    char time[10];
    char status[20];
} Appointment;


// ================= BILLING =================
typedef struct {
    char bill_id[10];
    char appointment_id[10];
    char patient_id[15];
    int amount;
    char status[15];
} Billing;


// ================= EHR =================
typedef struct {
    char record_id[10];
    char patient_id[15];
    char doctor_id[15];
    char diagnosis[100];
    char prescription[100];
    char notes[150];
} EHR;


// ================= INVENTORY =================
typedef struct {
    char item_id[10];
    char item_name[50];
    char category[30];
    int stock_level;
} Inventory;


// ================= SCHEDULE =================
typedef struct {
    char doctor_id[15];
    char date[11];
    char time[10];
    char status[20];  // Available, Booked, Unavailable
} Schedule;

#endif