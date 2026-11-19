#ifndef PATIENT_HASH_TABLE_H
#define PATIENT_HASH_TABLE_H

#include "Patient.h"
#include <unordered_map>
#include <vector>
#include <string>

class PatientHashTable
{
private:
    // Store patient records using patient ID as the key
    std::unordered_map<std::string, Patient> patients;

public:
    // Insert a new patient
    bool insertPatient(const Patient &patient)
    {
        if (patient.patientID.empty())
        {
            return false;
        }

        return patients.emplace(patient.patientID, patient).second;
    }

    // Search for a patient using their ID
    bool searchPatient(const std::string &id, Patient &patient) const
    {
        auto it = patients.find(id);

        if (it == patients.end())
        {
            return false;
        }

        patient = it->second;
        return true;
    }

    // Update an existing patient's information
    bool updatePatient(
        const std::string &id,
        const Patient &updatedPatient)
    {
        auto it = patients.find(id);

        if (it == patients.end())
        {
            return false;
        }

        if (updatedPatient.patientID != id)
        {
            return false;
        }

        it->second = updatedPatient;
        return true;
    }

    // Delete a patient using their ID
    bool deletePatient(const std::string &id)
    {
        return patients.erase(id) > 0;
    }

    // Check whether a patient ID already exists
    bool patientExists(const std::string &id) const
    {
        return patients.find(id) != patients.end();
    }

    // Return all registered patient records
    std::vector<Patient> getAllPatients() const
    {
        std::vector<Patient> records;

        for (const auto &entry : patients)
        {
            records.push_back(entry.second);
        }

        return records;
    }

    // Return the total number of registered patients
    int getPatientCount() const
    {
        return static_cast<int>(patients.size());
    }

    // Check whether the hash table is empty
    bool isEmpty() const
    {
        return patients.empty();
    }
};

#endif
