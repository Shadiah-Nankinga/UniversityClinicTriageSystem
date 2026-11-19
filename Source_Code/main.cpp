#include "ClinicSystem.h"
#include <iostream>
#include <string>
#include <limits>
#include <stdexcept>

using namespace std;

// Read and validate integer input
int readInteger(const string &message)
{
    string input;
    int number;

    while (true)
    {
        cout << message;
        getline(cin, input);

        try
        {
            size_t position;
            number = stoi(input, &position);

            if (position != input.length())
            {
                throw invalid_argument("Invalid input");
            }

            return number;
        }
        catch (...)
        {
            cout << "Invalid input! Please enter a whole number.\n";
        }
    }
}

// Display patient information
void displayPatient(const Patient &patient)
{
    cout << "\n---------- PATIENT DETAILS ----------\n";
    cout << "Patient ID: " << patient.patientID << '\n';
    cout << "Full Name: " << patient.fullName << '\n';
    cout << "Age: " << patient.age << '\n';
    cout << "Symptoms: " << patient.symptoms << '\n';
    cout << "Priority: " << patient.priority << '\n';
    cout << "Arrival Order: " << patient.arrivalOrder << '\n';
    cout << "Status: " << patient.status << '\n';
    cout << "-------------------------------------\n";
}

// Display application menu
void displayMenu()
{
    cout << "\n========================================\n";
    cout << "       UNIVERSITY CLINIC SYSTEM\n";
    cout << "          TRIAGE MANAGEMENT\n";
    cout << "========================================\n";
    cout << "1. Register New Patient\n";
    cout << "2. Search Patient\n";
    cout << "3. Update Patient Information\n";
    cout << "4. Update Patient Priority\n";
    cout << "5. View Next Patient\n";
    cout << "6. Attend to Next Patient\n";
    cout << "7. Delete Patient Record\n";
    cout << "8. Display Waiting Patients\n";
    cout << "9. View Total Patients\n";
    cout << "0. Save and Exit\n";
    cout << "========================================\n";
}

int main()
{
    ClinicSystem clinic;

    // Load saved patient records
    if (clinic.loadFromFile("patients.txt"))
    {
        cout << "Patient records loaded successfully.\n";
    }
    else
    {
        cout << "No existing patient file found. Starting a new system.\n";
    }

    int choice;

    do
    {
        displayMenu();

        choice = readInteger("Enter your choice: ");

        switch (choice)
        {

        case 1:
        {
            Patient patient;

            cout << "\n--- REGISTER NEW PATIENT ---\n";

            cout << "Enter Patient ID: ";
            getline(cin, patient.patientID);

            cout << "Enter Full Name: ";
            getline(cin, patient.fullName);

            patient.age = readInteger("Enter Age: ");

            cout << "Enter Symptoms: ";
            getline(cin, patient.symptoms);

            cout << "\nPriority Categories:\n";
            cout << "5 - Emergency\n";
            cout << "4 - Very Urgent\n";
            cout << "3 - Urgent\n";
            cout << "2 - Less Urgent\n";
            cout << "1 - Non-Urgent\n";

            patient.priority = readInteger("Enter Priority (1-5): ");

            if (clinic.registerPatient(patient))
            {
                cout << "\nPatient registered successfully!\n";
            }
            else
            {
                cout << "\nRegistration failed! Check the details, "
                     << "priority, age or duplicate patient ID.\n";
            }

            break;
        }

        case 2:
        {
            string id;
            Patient patient;

            cout << "Enter Patient ID to search: ";
            getline(cin, id);

            if (clinic.searchPatient(id, patient))
            {
                displayPatient(patient);
            }
            else
            {
                cout << "Patient not found.\n";
            }

            break;
        }

        case 3:
        {
            string id;
            Patient patient;

            cout << "Enter Patient ID to update: ";
            getline(cin, id);

            if (!clinic.searchPatient(id, patient))
            {
                cout << "Patient not found.\n";
                break;
            }

            if (patient.status == "Attended")
            {
                cout << "Attended patient records cannot be updated.\n";
                break;
            }

            cout << "Enter New Full Name: ";
            getline(cin, patient.fullName);

            cout << "Enter New Age: ";
            patient.age = readInteger("");

            cout << "Enter New Symptoms: ";
            getline(cin, patient.symptoms);

            if (patient.fullName.empty() ||
                patient.age < 1 || patient.age > 120)
            {
                cout << "Invalid patient information.\n";
            }
            else if (clinic.updatePatient(id, patient))
            {
                cout << "Patient information updated successfully.\n";
            }
            else
            {
                cout << "Patient update failed.\n";
            }

            break;
        }

        case 4:
        {
            string id;

            cout << "Enter Patient ID: ";
            getline(cin, id);

            int priority = readInteger("Enter New Priority (1-5): ");

            if (clinic.updatePriority(id, priority))
            {
                cout << "Patient priority updated successfully.\n";
            }
            else
            {
                cout << "Priority update failed. Check patient ID, "
                     << "status and priority value.\n";
            }

            break;
        }

        case 5:
        {
            Patient patient;

            if (clinic.viewNextPatient(patient))
            {
                cout << "\nNEXT PATIENT TO BE ATTENDED:\n";
                displayPatient(patient);
            }
            else
            {
                cout << "No patients are waiting.\n";
            }

            break;
        }

        case 6:
        {
            Patient patient;

            if (clinic.attendNextPatient(patient))
            {
                cout << "\nPatient attended successfully:\n";
                displayPatient(patient);
            }
            else
            {
                cout << "No patients are waiting to be attended.\n";
            }

            break;
        }

        case 7:
        {
            string id;

            cout << "Enter Patient ID to delete: ";
            getline(cin, id);

            if (clinic.deletePatient(id))
            {
                cout << "Patient record deleted successfully.\n";
            }
            else
            {
                cout << "Deletion failed. Patient may not exist "
                     << "or may already have been attended.\n";
            }

            break;
        }

        case 8:
        {
            clinic.displayWaitingPatients();
            break;
        }

        case 9:
        {
            cout << "Total registered patients: "
                 << clinic.getTotalPatients() << '\n';
            break;
        }

        case 0:
        {
            if (clinic.saveToFile("patients.txt"))
            {
                cout << "Patient records saved successfully.\n";
            }
            else
            {
                cout << "Error: Could not save patient records.\n";
            }

            cout << "Exiting University Clinic System. Goodbye!\n";
            break;
        }

        default:
            cout << "Invalid menu choice! Select a number from 0 to 9.\n";
        }

    } while (choice != 0);

    return 0;
}
