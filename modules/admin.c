#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../headers/admin.h"
#include "../headers/structures.h"


void registerUser() {
    FILE *userFile;
    FILE *detailFile;
    User user;
    Patient patient;
    Doctor doctor;
    Nurse nurse;
    int roleChoice;
    int nextNum;
    char prefix[4];

    printf("\n=====================================");
    printf("\n            REGISTER NEW USER        ");
    printf("\n=====================================\n");

    //ROLE CHOICE WITH RETRY
    while (1) {
        printf("Choose user type:\n");
        printf("1. Patient\n");
        printf("2. Doctor\n");
        printf("3. Nurse\n");
        printf("0. Go Back\n");
        printf("Enter choice: ");
        scanf("%d", &roleChoice);

        if (roleChoice == 0) {
            printf("Going back...\n");
            return;  // goes back to admin menu
        }

        if (roleChoice >= 1 && roleChoice <= 3) break;

        printf("Invalid choice. Please try again.\n");
    }

    // assign role + prefix
    if (roleChoice == 1) {
        strcpy(user.role, "Patient");
        strcpy(prefix, "PAT");
    }
    else if (roleChoice == 2) {
        strcpy(user.role, "Doctor");
        strcpy(prefix, "DOC");
    }
    else {
        strcpy(user.role, "Nurse");
        strcpy(prefix, "NUR");
    }

    nextNum = generateNextNumber(prefix);
    sprintf(user.id, "%s%06d", prefix, nextNum);

    printf("Generated ID: %s\n", user.id);

    printf("Enter Name: ");
    scanf(" %49[^\n]", user.name);

    printf("Enter Password: ");
    scanf(" %19[^\n]", user.password);

    //WRITE TO USERS FILE
    userFile = fopen("data/users.txt", "a");
    if (userFile == NULL) {
        printf("Error opening users file.\n");
        return;
    }

    fprintf(userFile, "\n%s|%s|%s|%s",
            user.id, user.name, user.password, user.role);

    fclose(userFile);

    // =============================
    // 👤 PATIENT
    // =============================
    if (roleChoice == 1) {
        detailFile = fopen("data/patients.txt", "a");

        if (detailFile == NULL) {
            printf("Error opening patients file.\n");
            return;
        }

        strcpy(patient.patient_id, user.id);
        strcpy(patient.name, user.name);

        printf("Enter Age: ");
        scanf("%d", &patient.age);
        while (getchar() != '\n');

        printf("Enter Gender: ");
        scanf(" %9[^\n]", patient.gender);

        printf("Enter Contact: ");
        scanf(" %19[^\n]", patient.contact);

        fprintf(detailFile, "\n%s|%s|%d|%s|%s",
                patient.patient_id, patient.name,
                patient.age, patient.gender, patient.contact);

        fclose(detailFile);
    }

    // =============================
    // DOCTOR
    // =============================
    else if (roleChoice == 2) {
        int deptChoice, specChoice;

        detailFile = fopen("data/doctors.txt", "a");

        if (detailFile == NULL) {
            printf("Error opening doctors file.\n");
            return;
        }

        strcpy(doctor.doctor_id, user.id);
        strcpy(doctor.name, user.name);

        // DEPARTMENT WITH RETRY
        while (1) {
            printf("Choose Department:\n");
            printf("1. Cardiology\n");
            printf("2. Neurology\n");
            printf("3. Orthopedics\n");
            printf("4. Pediatrics\n");
            printf("5. Emergency\n");
            printf("6. General Medicine\n");
            printf("Enter choice: ");
            scanf("%d", &deptChoice);

            if (deptChoice >= 1 && deptChoice <= 6) break;

            printf("Invalid choice. Please try again.\n");
        }

        // SPECIALIZATION DEPENDS ON DEPARTMENT
        while (1) {
            if (deptChoice == 1) {
                strcpy(doctor.department, "Cardiology");
                printf("1. Heart Specialist\n2. Cardiac Surgeon\n");
            }
            else if (deptChoice == 2) {
                strcpy(doctor.department, "Neurology");
                printf("1. Brain Specialist\n2. Neurosurgeon\n");
            }
            else if (deptChoice == 3) {
                strcpy(doctor.department, "Orthopedics");
                printf("1. Bone Specialist\n2. Joint Specialist\n");
            }
            else if (deptChoice == 4) {
                strcpy(doctor.department, "Pediatrics");
                printf("1. Child Specialist\n2. Neonatal Specialist\n");
            }
            else if (deptChoice == 5) {
                strcpy(doctor.department, "Emergency");
                printf("1. Emergency Care\n2. Trauma Specialist\n");
            }
            else {
                strcpy(doctor.department, "General Medicine");
                printf("1. General Physician\n2. Internal Medicine Specialist\n");
            }

            printf("Enter specialization choice: ");
            scanf("%d", &specChoice);

            if (specChoice == 1 || specChoice == 2) break;

            printf("Invalid choice. Please try again.\n");
        }

        // assign specialization
        if (deptChoice == 1) {
            if (specChoice == 1) strcpy(doctor.specialization, "Heart Specialist");
            else strcpy(doctor.specialization, "Cardiac Surgeon");
        }
        else if (deptChoice == 2) {
            if (specChoice == 1) strcpy(doctor.specialization, "Brain Specialist");
            else strcpy(doctor.specialization, "Neurosurgeon");
        }
        else if (deptChoice == 3) {
            if (specChoice == 1) strcpy(doctor.specialization, "Bone Specialist");
            else strcpy(doctor.specialization, "Joint Specialist");
        }
        else if (deptChoice == 4) {
            if (specChoice == 1) strcpy(doctor.specialization, "Child Specialist");
            else strcpy(doctor.specialization, "Neonatal Specialist");
        }
        else if (deptChoice == 5) {
            if (specChoice == 1) strcpy(doctor.specialization, "Emergency Care");
            else strcpy(doctor.specialization, "Trauma Specialist");
        }
        else {
            if (specChoice == 1) strcpy(doctor.specialization, "General Physician");
            else strcpy(doctor.specialization, "Internal Medicine Specialist");
        }

        fprintf(detailFile, "\n%s|%s|%s|%s",
                doctor.doctor_id, doctor.name,
                doctor.department, doctor.specialization);

        fclose(detailFile);
    }

    // =============================
    // NURSE
    // =============================
    else {
        int deptChoice;

        detailFile = fopen("data/nurses.txt", "a");

        if (detailFile == NULL) {
            printf("Error opening nurses file.\n");
            return;
        }

        strcpy(nurse.nurse_id, user.id);
        strcpy(nurse.name, user.name);

        while (1) {
            printf("Choose Department:\n");
            printf("1. Cardiology\n");
            printf("2. Neurology\n");
            printf("3. Orthopedics\n");
            printf("4. Pediatrics\n");
            printf("5. Emergency\n");
            printf("6. General Ward\n");
            printf("Enter choice: ");
            scanf("%d", &deptChoice);

            if (deptChoice >= 1 && deptChoice <= 6) break;

            printf("Invalid choice. Please try again.\n");
        }

        if (deptChoice == 1) strcpy(nurse.department, "Cardiology");
        else if (deptChoice == 2) strcpy(nurse.department, "Neurology");
        else if (deptChoice == 3) strcpy(nurse.department, "Orthopedics");
        else if (deptChoice == 4) strcpy(nurse.department, "Pediatrics");
        else if (deptChoice == 5) strcpy(nurse.department, "Emergency");
        else strcpy(nurse.department, "General Ward");

        fprintf(detailFile, "\n%s|%s|%s",
                nurse.nurse_id, nurse.name, nurse.department);

        fclose(detailFile);
    }

    printf("\n%s registered successfully!\n", user.role);
}


void adminMenu() {
    int choice;

    while (1) {
        printf("\n=====================================");
        printf("\n              ADMIN MENU             ");
        printf("\n=====================================\n");
        printf("1. Register new user\n");
        printf("2. View patients\n");
        printf("3. View inventory\n");
        printf("0. Go Back\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter 1-4.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1) {
            registerUser();
        }
        else if (choice == 2) {
            printf("View patients feature coming soon...\n");
        }
        else if (choice == 3) {
            printf("View inventory feature coming soon...\n");
        }
        else if (choice == 0) {
        printf("Going back...\n");
        break;  // THIS RETURNS TO PREVIOUS MENU
        }
        else {
            printf("Invalid choice. Please enter 1-4.\n");
        }
    }
}