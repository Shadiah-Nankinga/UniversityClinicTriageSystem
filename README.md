# University Clinic Triage System

## Project Overview
The University Clinic Triage System is a C++17 application designed to manage patient registration, records and prioritisation in a university clinic.

The system prioritises patients according to the urgency of their medical conditions, ensuring that patients with higher priority are attended to first.

## Main Features
- Register new patients.
- Search for patients using their unique patient ID.
- Update patient information and priority.
- Delete patient records.
- View the next patient to be attended to.
- Attend patients according to priority.
- Display the waiting list in priority order.
- Save and load patient records using a text file.

## Data Structures
- **Binary Max Heap:** Used as a priority queue to organise patients according to urgency.
- **Hash Table:** Used for patient record management and searching by patient ID.

## Technologies Used
- Programming Language: C++17
- Development Environment: Visual Studio Code
- Compiler: GCC (g++)

## Project Structure
- `Patient.h` – Defines the patient structure.
- `PatientPriorityQueue.h` – Implements the priority queue.
- `PatientHashTable.h` – Manages patient records.
- `ClinicSystem.h` – Handles clinic operations.
- `main.cpp` – Contains the main menu and program execution.
- `patients.txt` – Stores patient records.

## Compilation and Execution

Open a terminal inside the Source_Code folder and run:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic main.cpp -o clinic
```

Run the program:

```bash
./clinic
```

## Course Information
BCS 2204 – Data Structures and Algorithms

Group Coursework 2 – Scenario 2
