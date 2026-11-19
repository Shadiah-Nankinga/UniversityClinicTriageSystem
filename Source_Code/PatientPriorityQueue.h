#ifndef PATIENT_PRIORITY_QUEUE_H
#define PATIENT_PRIORITY_QUEUE_H

#include "Patient.h"
#include <vector>
#include <algorithm>

class PatientPriorityQueue
{
private:
    vector<Patient> heap;

    // Check which patient should come first
    bool hasHigherPriority(const Patient &a, const Patient &b)
    {
        if (a.priority != b.priority)
        {
            return a.priority > b.priority;
        }

        return a.arrivalOrder < b.arrivalOrder;
    }

    // Move a newly inserted patient upwards
    void heapifyUp(int index)
    {
        while (index > 0)
        {
            int parent = (index - 1) / 2;

            if (hasHigherPriority(heap[index], heap[parent]))
            {
                swap(heap[index], heap[parent]);
                index = parent;
            }
            else
            {
                break;
            }
        }
    }

    // Restore the heap after removing the top patient
    void heapifyDown(int index)
    {
        int size = static_cast<int>(heap.size());

        while (true)
        {
            int left = 2 * index + 1;
            int right = 2 * index + 2;
            int highest = index;

            if (left < size &&
                hasHigherPriority(heap[left], heap[highest]))
            {
                highest = left;
            }

            if (right < size &&
                hasHigherPriority(heap[right], heap[highest]))
            {
                highest = right;
            }

            if (highest == index)
            {
                break;
            }

            swap(heap[index], heap[highest]);
            index = highest;
        }
    }

public:
    // Add a patient to the waiting queue
    void enqueue(const Patient &patient)
    {
        heap.push_back(patient);
        heapifyUp(static_cast<int>(heap.size()) - 1);
    }

    // View the next patient without removing them
    bool peek(Patient &patient) const
    {
        if (heap.empty())
        {
            return false;
        }

        patient = heap[0];
        return true;
    }

    // Remove and return the next patient
    bool dequeue(Patient &patient)
    {
        if (heap.empty())
        {
            return false;
        }

        patient = heap[0];

        if (heap.size() == 1)
        {
            heap.pop_back();
            return true;
        }

        heap[0] = heap.back();
        heap.pop_back();

        heapifyDown(0);
        return true;
    }

    bool isEmpty() const
    {
        return heap.empty();
    }

    int size() const
    {
        return static_cast<int>(heap.size());
    }
};

#endif
