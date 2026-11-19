#ifndef CLINIC_SYSTEM_H
#define CLINIC_SYSTEM_H

#include "Patient.h"
#include "PatientPriorityQueue.h"
#include "PatientHashTable.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

class ClinicSystem
{
private:
    PatientHashTable patientRecords;
    PatientPriorityQueue waitingQueue;
    int nextArrivalOrder;

    // Rebuild the queue using only patients who are waiting
    void rebuildWaitingQueue()
    {
        waitingQueue = PatientPriorityQueue();

        vector<Patient> records = patientRecords.getAllPatients();

        for (const Patient &patient : records)
        {
            if (patient.status == "Waiting")
            {
                waitingQueue.enqueue(patient);
            }
        }
    }

public:
    // Constructor
    ClinicSystem()
    {
        nextArrivalOrder = 1;
    }

    // Register a new patient
    bool registerPatient(Patient patient)
    {
        if (patient.patientID.empty() ||
            patient.fullName.empty() ||
            patient.age < 1 || patient.age > 120 ||
            patient.priority < 1 || patient.priority > 5 ||
            patientRecords.patientExists(patient.patientID))
        {
            return false;
        }

        patient.arrivalOrder = nextArrivalOrder++;
        patient.status = "Waiting";

        if (!patientRecords.insertPatient(patient))
        {
            return false;
        }

        waitingQueue.enqueue(patient);

        return true;
    }

    // Search for a patient
    bool searchPatient(const string &id, Patient &patient) const
    {
        return patientRecords.searchPatient(id, patient);
    }

    // Update patient information
    bool updatePatient(const string &id, Patient updatedPatient)
    {
        Patient existingPatient;

        if (!patientRecords.searchPatient(id, existingPatient))
        {
            return false;
        }

        if (existingPatient.status == "Attended")
        {
            return false;
        }

        if (updatedPatient.fullName.empty() ||
            updatedPatient.age < 1 ||
            updatedPatient.age > 120 ||
            updatedPatient.symptoms.empty())
        {
            return false;
        }

        updatedPatient.patientID = existingPatient.patientID;
        updatedPatient.arrivalOrder = existingPatient.arrivalOrder;
        updatedPatient.status = existingPatient.status;
        updatedPatient.priority = existingPatient.priority;

        if (!patientRecords.updatePatient(id, updatedPatient))
        {
            return false;
        }

        rebuildWaitingQueue();

        return true;
    }

    // Update a patient's priority
    bool updatePriority(const string &id, int newPriority)
    {
        Patient patient;

        if (!patientRecords.searchPatient(id, patient))
        {
            return false;
        }

        if (patient.status != "Waiting" ||
            newPriority < 1 || newPriority > 5)
        {
            return false;
        }

        patient.priority = newPriority;

        if (!patientRecords.updatePatient(id, patient))
        {
            return false;
        }

        rebuildWaitingQueue();

        return true;
    }

    // View the next patient without removing them
    bool viewNextPatient(Patient &patient) const
    {
        return waitingQueue.peek(patient);
    }

    // Attend to the highest-priority patient
    bool attendNextPatient(Patient &patient)
    {
        if (!waitingQueue.dequeue(patient))
        {
            return false;
        }

        patient.status = "Attended";

        patientRecords.updatePatient(patient.patientID, patient);

        return true;
    }

    // Delete a patient record
    bool deletePatient(const string &id)
    {
        Patient patient;

        if (!patientRecords.searchPatient(id, patient))
        {
            return false;
        }

        if (patient.status == "Attended")
        {
            return false;
        }

        if (!patientRecords.deletePatient(id))
        {
            return false;
        }

        rebuildWaitingQueue();

        return true;
    }

    // Display all waiting patients in priority order
    void displayWaitingPatients() const
    {
        vector<Patient> records = patientRecords.getAllPatients();

        records.erase(
            remove_if(records.begin(), records.end(),
                      [](const Patient &patient)
                      {
                          return patient.status != "Waiting";
                      }),
            records.end());

        sort(records.begin(), records.end(),
             [](const Patient &a, const Patient &b)
             {
                 if (a.priority != b.priority)
                 {
                     return a.priority > b.priority;
                 }

                 return a.arrivalOrder < b.arrivalOrder;
             });

        if (records.empty())
        {
            cout << "\nNo patients are currently waiting.\n";
            return;
        }

        cout << "\n========== PATIENT WAITING LIST ==========\n";

        for (const Patient &patient : records)
        {
            cout << "ID: " << patient.patientID
                 << " | Name: " << patient.fullName
                 << " | Priority: " << patient.priority
                 << " | Arrival: " << patient.arrivalOrder
                 << '\n';
        }
    }

    // Save all patient records to a text file
    bool saveToFile(const string &filename) const
    {
        ofstream file(filename);

        if (!file.is_open())
        {
            return false;
        }

        vector<Patient> records = patientRecords.getAllPatients();

        for (const Patient &patient : records)
        {
            file << patient.patientID << '|'
                 << patient.fullName << '|'
                 << patient.age << '|'
                 << patient.symptoms << '|'
                 << patient.priority << '|'
                 << patient.arrivalOrder << '|'
                 << patient.status << '\n';
        }

        file.close();

        return !file.fail();
    }

    // Load patient records from a text file
    bool loadFromFile(const string &filename)
    {
        ifstream file(filename);

        if (!file.is_open())
        {
            return false;
        }

        string line;
        int largestArrival = 0;

        while (getline(file, line))
        {
            stringstream ss(line);
            Patient patient;

            string ageText;
            string priorityText;
            string arrivalText;

            if (!getline(ss, patient.patientID, '|') ||
                !getline(ss, patient.fullName, '|') ||
                !getline(ss, ageText, '|') ||
                !getline(ss, patient.symptoms, '|') ||
                !getline(ss, priorityText, '|') ||
                !getline(ss, arrivalText, '|') ||
                !getline(ss, patient.status))
            {
                continue;
            }

            // Remove Windows carriage return if present
            if (!patient.status.empty() &&
                patient.status.back() == '\r')
            {
                patient.status.pop_back();
            }

            try
            {
                patient.age = stoi(ageText);
                patient.priority = stoi(priorityText);
                patient.arrivalOrder = stoi(arrivalText);
            }
            catch (...)
            {
                continue;
            }

            // Validate loaded patient information
            if (patient.patientID.empty() ||
                patient.fullName.empty() ||
                patient.age < 1 || patient.age > 120 ||
                patient.priority < 1 || patient.priority > 5 ||
                patient.arrivalOrder < 1 ||
                (patient.status != "Waiting" &&
                 patient.status != "Attended") ||
                patientRecords.patientExists(patient.patientID))
            {
                continue;
            }

            if (patientRecords.insertPatient(patient))
            {
                largestArrival = max(
                    largestArrival,
                    patient.arrivalOrder);
            }
        }

        file.close();

        nextArrivalOrder = largestArrival + 1;

        rebuildWaitingQueue();

        return true;
    }

    // Get the total number of registered patients
    int getTotalPatients() const
    {
        return patientRecords.getPatientCount();
    }
};

#endif
