#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "../headers/users.h"
#include "../headers/structures.h"
#include "../headers/helper.h"

//// Gets the current date and time for report display.
void getUsersReportDateTime(char dateTime[])
{
    time_t t;
    struct tm *currentTime;

    t = time(NULL);
    currentTime = localtime(&t);

    strftime(dateTime, 50, "%Y-%m-%d %H:%M:%S", currentTime);
}

//// Reads one user record from users.txt.
int readUser(FILE *file, User *user)
{
    return fscanf(file, " %14[^|]|%49[^|]|%19[^|]|%19[^|]|%19[^\n]\n",
                  user->id, user->name, user->password, user->role, user->status) == 5;
}

//// Writes one user record to users.txt.
void writeUser(FILE *file, User user)
{
    fprintf(file, "%s|%s|%s|%s|%s\n",
            user.id, user.name, user.password, user.role, user.status);
}

//// Prints the matched user details in a simple format.
void printUserDetails(User user)
{
    printf("\nUser Found:\n");
    printf("ID     : %s\n", user.id);
    printf("Name   : %s\n", user.name);
    printf("Role   : %s\n", user.role);
    printf("Status : %s\n", user.status);
}

//// Checks if the role is patient, doctor, or nurse.
int isManagedRole(char role[])
{
    return strcmp(role, "Patient") == 0 ||
           strcmp(role, "Doctor") == 0 ||
           strcmp(role, "Nurse") == 0;
}

//// Lets the admin choose whether the new user is a patient, doctor, or nurse.
int chooseUserType(void)
{
    int roleChoice;

    while (1)
    {
        printf("Choose user type:\n");
        printf("1. Patient\n");
        printf("2. Doctor\n");
        printf("3. Nurse\n");
        printf("0. Go Back\n");
        printf("Enter choice: ");

        if (scanf("%d", &roleChoice) != 1)
        {
            printf("Invalid input. Please enter 0-3.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (roleChoice >= 0 && roleChoice <= 3)
        {
            return roleChoice;
        }

        printf("Invalid choice. Please try again.\n");
    }
}

//// Sets the role name and generates a matching user ID.
void setUserRoleAndID(User *user, int roleChoice)
{
    int nextNum;
    char prefix[4];

    if (roleChoice == 1)
    {
        strcpy(user->role, "Patient");
        strcpy(prefix, "PAT");
    }
    else if (roleChoice == 2)
    {
        strcpy(user->role, "Doctor");
        strcpy(prefix, "DOC");
    }
    else
    {
        strcpy(user->role, "Nurse");
        strcpy(prefix, "NUR");
    }

    nextNum = generateNextNumber("data/users.txt", prefix);
    sprintf(user->id, "%s%03d", prefix, nextNum);
}

//// Saves the common account data to users.txt with Active status.
int saveUserAccount(User user)
{
    FILE *userFile;

    userFile = fopen("data/users.txt", "a");

    if (userFile == NULL)
    {
        printf("Error opening users file.\n");
        return 0;
    }

    fprintf(userFile, "\n%s|%s|%s|%s|Active",
            user.id, user.name, user.password, user.role);

    fclose(userFile);
    return 1;
}

//// Finds a user in users.txt by ID.
int findUserByID(char userID[], User *user)
{
    FILE *file;

    file = fopen("data/users.txt", "r");

    if (file == NULL)
    {
        printf("Error opening users file.\n");
        return 0;
    }

    fscanf(file, "%*[^\n]\n");

    while (readUser(file, user))
    {
        if (strcmp(userID, user->id) == 0)
        {
            fclose(file);
            return 1;
        }
    }

    fclose(file);
    return 0;
}

//// Updates a user's status or password in users.txt.
int updateUserFile(char userID[], char newValue[], int mode)
{
    FILE *file;
    FILE *tempFile;
    User user;
    char header[100];
    int updated;

    file = fopen("data/users.txt", "r");
    tempFile = fopen("data/temp_users.txt", "w");

    if (file == NULL || tempFile == NULL)
    {
        printf("Error opening users file.\n");
        if (file != NULL) fclose(file);
        if (tempFile != NULL) fclose(tempFile);
        return 0;
    }

    updated = 0;

    if (fgets(header, sizeof(header), file) != NULL)
    {
        fputs(header, tempFile);
    }

    while (readUser(file, &user))
    {
        if (strcmp(userID, user.id) == 0)
        {
            if (mode == 1) strcpy(user.status, newValue);
            else if (mode == 2) strcpy(user.password, newValue);
            updated = 1;
        }

        writeUser(tempFile, user);
    }

    fclose(file);
    fclose(tempFile);

    remove("data/users.txt");
    rename("data/temp_users.txt", "data/users.txt");

    return updated;
}

//// Asks the admin to confirm the deactivate action.
int confirmDeactivateUser(void)
{
    int choice;

    while (1)
    {
        printf("\n1. Confirm Deactivate\n");
        printf("0. Cancel\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter 1 or 0.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1 || choice == 0)
        {
            return choice;
        }

        printf("Invalid choice. Please enter 1 or 0.\n");
    }
}

//// Asks the admin to confirm the password reset action.
int confirmResetPassword(void)
{
    int choice;

    while (1)
    {
        printf("\n1. Confirm Reset Password\n");
        printf("0. Cancel\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter 1 or 0.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1 || choice == 0)
        {
            return choice;
        }

        printf("Invalid choice. Please enter 1 or 0.\n");
    }
}

//// Lets the admin choose a department for doctor or nurse users.
int chooseDepartment(char department[], const char lastDepartment[])
{
    int deptChoice;

    while (1)
    {
        printf("\nChoose Department:\n");
        printf("1. Cardiology\n");
        printf("2. Neurology\n");
        printf("3. Orthopedics\n");
        printf("4. Pediatrics\n");
        printf("5. Emergency\n");
        printf("6. %s\n", lastDepartment);
        printf("Enter choice: ");
        scanf("%d", &deptChoice);

        if (deptChoice >= 1 && deptChoice <= 6)
        {
            if (deptChoice == 1) strcpy(department, "Cardiology");
            else if (deptChoice == 2) strcpy(department, "Neurology");
            else if (deptChoice == 3) strcpy(department, "Orthopedics");
            else if (deptChoice == 4) strcpy(department, "Pediatrics");
            else if (deptChoice == 5) strcpy(department, "Emergency");
            else strcpy(department, lastDepartment);

            return deptChoice;
        }

        printf("Invalid choice. Please try again.\n");
    }
}

//// Lets the admin choose gender for a patient user.
void chooseGender(char gender[])
{
    int genderChoice;

    while (1)
    {
        printf("Choose Gender:\n");
        printf("1. Female\n");
        printf("2. Male\n");
        printf("Enter choice: ");
        scanf("%d", &genderChoice);

        if (genderChoice == 1)
        {
            strcpy(gender, "Female");
            return;
        }
        else if (genderChoice == 2)
        {
            strcpy(gender, "Male");
            return;
        }

        printf("Invalid choice. Please try again.\n");
    }
}

//// Lets the admin choose a specialization for a doctor user.
void chooseDoctorSpecialization(int deptChoice, char specialization[])
{
    int specChoice;
    char firstOption[30];
    char secondOption[35];

    if (deptChoice == 1)
    {
        strcpy(firstOption, "Heart Specialist");
        strcpy(secondOption, "Cardiac Surgeon");
    }
    else if (deptChoice == 2)
    {
        strcpy(firstOption, "Brain Specialist");
        strcpy(secondOption, "Neurosurgeon");
    }
    else if (deptChoice == 3)
    {
        strcpy(firstOption, "Bone Specialist");
        strcpy(secondOption, "Joint Specialist");
    }
    else if (deptChoice == 4)
    {
        strcpy(firstOption, "Child Specialist");
        strcpy(secondOption, "Neonatal Specialist");
    }
    else if (deptChoice == 5)
    {
        strcpy(firstOption, "Emergency Care");
        strcpy(secondOption, "Trauma Specialist");
    }
    else
    {
        strcpy(firstOption, "General Physician");
        strcpy(secondOption, "Internal Medicine Specialist");
    }

    while (1)
    {
        printf("1. %s\n", firstOption);
        printf("2. %s\n", secondOption);
        printf("Enter specialization choice: ");
        scanf("%d", &specChoice);

        if (specChoice == 1)
        {
            strcpy(specialization, firstOption);
            return;
        }
        else if (specChoice == 2)
        {
            strcpy(specialization, secondOption);
            return;
        }

        printf("Invalid choice. Please try again.\n");
    }
}

//// Saves the patient-specific details after account creation.
void savePatientDetails(User user)
{
    FILE *detailFile;
    Patient patient;
    long fileSize;

    detailFile = fopen("data/patients.txt", "a+");

    if (detailFile == NULL)
    {
        printf("Error opening patients file.\n");
        return;
    }

    strcpy(patient.patient_id, user.id);
    strcpy(patient.name, user.name);

    printf("Enter Age: ");
    scanf("%d", &patient.age);
    while (getchar() != '\n');

    chooseGender(patient.gender);

    printf("Enter Contact: ");
    scanf(" %19[^\n]", patient.contact);

    fseek(detailFile, 0, SEEK_END);
    fileSize = ftell(detailFile);

    if (fileSize > 0)
    {
        fseek(detailFile, -1, SEEK_END);

        if (fgetc(detailFile) != '\n')
        {
            fputc('\n', detailFile);
        }
    }

    fprintf(detailFile, "%s|%s|%d|%s|%s",
            patient.patient_id, patient.name,
            patient.age, patient.gender, patient.contact);

    fclose(detailFile);
}

//// Saves the doctor-specific details after account creation.
void saveDoctorDetails(User user)
{
    FILE *detailFile;
    Doctor doctor;
    int deptChoice;

    detailFile = fopen("data/doctors.txt", "a");

    if (detailFile == NULL)
    {
        printf("Error opening doctors file.\n");
        return;
    }

    strcpy(doctor.doctor_id, user.id);
    strcpy(doctor.name, user.name);

    deptChoice = chooseDepartment(doctor.department, "General");
    chooseDoctorSpecialization(deptChoice, doctor.specialization);

    fprintf(detailFile, "\n%s|%s|%s|%s",
            doctor.doctor_id, doctor.name,
            doctor.department, doctor.specialization);

    fclose(detailFile);
}

//// Saves the nurse-specific details after account creation.
void saveNurseDetails(User user)
{
    FILE *detailFile;
    Nurse nurse;

    detailFile = fopen("data/nurses.txt", "a");

    if (detailFile == NULL)
    {
        printf("Error opening nurses file.\n");
        return;
    }

    strcpy(nurse.nurse_id, user.id);
    strcpy(nurse.name, user.name);

    chooseDepartment(nurse.department, "General");

    fprintf(detailFile, "\n%s|%s|%s",
            nurse.nurse_id, nurse.name, nurse.department);

    fclose(detailFile);
}

//// Registers a new patient, doctor, or nurse account.
void registerUser(void)
{
    User user;
    int roleChoice;

    printf("\n=====================================");
    printf("\n            REGISTER NEW USER        ");
    printf("\n=====================================\n");

    roleChoice = chooseUserType();

    if (roleChoice == 0)
    {
        printf("Going back...\n");
        return;
    }

    setUserRoleAndID(&user, roleChoice);

    printf("\nGenerated ID: %s\n", user.id);

    printf("Enter Name: ");
    scanf(" %49[^\n]", user.name);

    printf("Enter Password: ");
    scanf(" %19[^\n]", user.password);

    if (!saveUserAccount(user))
    {
        return;
    }

    if (roleChoice == 1)
    {
        savePatientDetails(user);
    }
    else if (roleChoice == 2)
    {
        saveDoctorDetails(user);
    }
    else if (roleChoice == 3)
    {
        saveNurseDetails(user);
    }

    printf("\n%s registered successfully!\n", user.role);
}

//// Lets the admin reset a patient, doctor, or nurse password.
void resetUserPassword(void)
{
    User user;
    char userID[15];

    while (1)
    {
        printf("\n=====================================");
        printf("\n        RESET USER PASSWORD          ");
        printf("\n=====================================\n");

        printf("Enter User ID (Patient/Doctor/Nurse): ");
        scanf(" %14[^\n]", userID);
        while (getchar() != '\n');

        if (!findUserByID(userID, &user))
        {
            printf("User not found.\n");

            if (askTryAgainOrGoBack() == 0)
            {
                return;
            }

            continue;
        }

        printUserDetails(user);

        if (!isManagedRole(user.role))
        {
            printf("Only Patient, Doctor, and Nurse passwords can be reset.\n");
            return;
        }

        if (strcmp(user.status, "Deactivated") == 0)
        {
            printf("Password cannot be reset for a deactivated account.\n");
            return;
        }

        if (confirmResetPassword() == 0)
        {
            return;
        }

        if (updateUserFile(userID, "Reset123", 2))
        {
            printf("User password reset successfully!\n");
            printf("Temporary password is: Reset123\n");
        }
        else
        {
            printf("Failed to reset user password.\n");
        }

        return;
    }
}

//// Counts all users that belong to one role or role group.
int countUsersByRole(char role1[], char role2[])
{
    FILE *file;
    User user;
    int count;

    file = fopen("data/users.txt", "r");

    if (file == NULL)
    {
        return 0;
    }

    count = 0;
    fscanf(file, "%*[^\n]\n");

    while (readUser(file, &user))
    {
        if (strcmp(user.role, role1) == 0 ||
            (role2[0] != '\0' && strcmp(user.role, role2) == 0))
        {
            count++;
        }
    }

    fclose(file);
    return count;
}

//// Prints all users for one role or role group.
void printUsersByRole(char title[], char role1[], char role2[])
{
    FILE *file;
    User user;

    file = fopen("data/users.txt", "r");

    if (file == NULL)
    {
        printf("Error opening users file.\n");
        return;
    }

    printf("\n%s\n", title);
    printf("%-12s %-20s %-12s\n", "UserID", "Name", "Status");
    fscanf(file, "%*[^\n]\n");

    while (readUser(file, &user))
    {
        if (strcmp(user.role, role1) == 0 ||
            (role2[0] != '\0' && strcmp(user.role, role2) == 0))
        {
            printf("%-12s %-20s %-12s\n", user.id, user.name, user.status);
        }
    }

    fclose(file);
}

//// Shows all users in a grouped report with a summary at the top.
void viewAllUsersReport(char currentUserName[])
{
    char dateTime[50];
    int totalAdmins;
    int totalDoctors;
    int totalNurses;
    int totalPatients;
    int totalUsers;

    getUsersReportDateTime(dateTime);
    totalAdmins = countUsersByRole("Admin", "Director");
    totalDoctors = countUsersByRole("Doctor", "");
    totalNurses = countUsersByRole("Nurse", "");
    totalPatients = countUsersByRole("Patient", "");
    totalUsers = totalAdmins + totalDoctors + totalNurses + totalPatients;

    printf("\n=====================================\n");
    printf("        VIEW ALL USERS REPORT        \n");
    printf("=====================================\n");
    printf("Report generated by %s at %s\n", currentUserName, dateTime);

    printf("\nSUMMARY\n");
    printf("Total Administrators : %d\n", totalAdmins);
    printf("Total Doctors        : %d\n", totalDoctors);
    printf("Total Nurses         : %d\n", totalNurses);
    printf("Total Patients       : %d\n", totalPatients);
    printf("Total Users          : %d\n", totalUsers);

    printUsersByRole("ADMINISTRATORS", "Admin", "Director");
    printUsersByRole("DOCTORS", "Doctor", "");
    printUsersByRole("NURSES", "Nurse", "");
    printUsersByRole("PATIENTS", "Patient", "");

    printf("\nPress Enter to go back...");
    getchar();
}

//// Lets the admin deactivate a patient, doctor, or nurse account.
void deactivateUser(void)
{
    User user;
    char userID[15];

    printf("\n=====================================");
    printf("\n           DEACTIVATE USER           ");
    printf("\n=====================================\n");

    printf("Enter User ID (Patient/Doctor/Nurse): ");
    scanf(" %14[^\n]", userID);
    while (getchar() != '\n');

    if (!findUserByID(userID, &user))
    {
        printf("User not found.\n");
        return;
    }

    printUserDetails(user);

    if (!isManagedRole(user.role))
    {
        printf("Only Patient, Doctor, and Nurse accounts can be deactivated.\n");
        return;
    }

    if (strcmp(user.status, "Deactivated") == 0)
    {
        printf("This user is already deactivated.\n");
        return;
    }

    if (confirmDeactivateUser() == 0)
    {
        printf("Deactivation cancelled.\n");
        return;
    }

    if (updateUserFile(userID, "Deactivated", 1))
    {
        printf("User deactivated successfully!\n");
    }
    else
    {
        printf("Failed to deactivate user.\n");
    }
}

//// Shows the user management menu for register, reset password, deactivate, and reports.
void manageUsersMenu(char currentUserName[])
{
    int choice;

    while (1)
    {
        printf("\n=====================================");
        printf("\n            MANAGE USERS             ");
        printf("\n=====================================\n");
        printf("1. Register New User\n");
        printf("2. Reset User Password\n");
        printf("3. Deactivate User\n");
        printf("4. View All Users Report\n");
        printf("0. Go Back\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1)
        {
            printf("Invalid input. Please enter 0-4.\n");
            while (getchar() != '\n');
            continue;
        }

        while (getchar() != '\n');

        if (choice == 1)
        {
            registerUser();
        }
        else if (choice == 2)
        {
            resetUserPassword();
        }
        else if (choice == 3)
        {
            deactivateUser();
        }
        else if (choice == 4)
        {
            viewAllUsersReport(currentUserName);
        }
        else if (choice == 0)
        {
            printf("Going back...\n");
            break;
        }
        else
        {
            printf("Invalid choice. Please enter 0-4.\n");
        }
    }
}
