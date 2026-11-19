#ifndef PATIENT_H
#define PATIENT_H

#include <iostream>
#include <string>
using namespace std;

struct Patient
{
    string patientID;
    string fullName;
    int age;
    string symptoms;
    int priority;
    int arrivalOrder;
    string status;
};

#endif
