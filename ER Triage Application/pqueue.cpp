// CMSC 341 - Fall 2023 - Project 3
#include "pqueue.h"

PQueue::PQueue(prifn_t priFn, HEAPTYPE heapType, STRUCTURE structure)
{
    m_heap = nullptr;
    m_size = 0;
    m_priorFunc = priFn;
    m_heapType = heapType;
    m_structure = structure;
}

PQueue::~PQueue()
{
    clear();
}

void PQueue::clear()
{
    while (m_size > 0)
    {
        getNextPatient();
    }
}

PQueue::PQueue(const PQueue& rhs)
{
    m_heap = nullptr;
    m_size = 0;
    m_priorFunc = rhs.m_priorFunc;
    m_heapType = rhs.m_heapType;
    m_structure = rhs.m_structure;

    // Copy patients from the other queue
    PQueue tempQueue(rhs.m_priorFunc, rhs.m_heapType, rhs.m_structure);
    while (rhs.m_size > 0)
    {
        tempQueue.insertPatient(rhs.getNextPatient());
    }

    // Swap the temporary queue with the current queue
    swap(m_heap, tempQueue.m_heap);
    swap(m_size, tempQueue.m_size);
}

PQueue& PQueue::operator=(const PQueue& rhs)
{
    if (this != &rhs)
    {
        clear(); // Clear the current queue

        m_priorFunc = rhs.m_priorFunc;
        m_heapType = rhs.m_heapType;
        m_structure = rhs.m_structure;

        // Copy patients from the other queue
        PQueue tempQueue(rhs.m_priorFunc, rhs.m_heapType, rhs.m_structure);
        while (rhs.m_size > 0)
        {
            tempQueue.insertPatient(rhs.getNextPatient());
        }

        // Swap the temporary queue with the current queue
        swap(m_heap, tempQueue.m_heap);
        swap(m_size, tempQueue.m_size);
    }

    return *this;
}

void PQueue::mergeWithQueue(PQueue& rhs)
{
    // Merge the current queue with the other queue
    while (rhs.m_size > 0)
    {
        insertPatient(rhs.getNextPatient());
    }
}

void PQueue::insertPatient(const Patient& patient)
{
    Node* newNode = new Node(patient);

    // Merge the new node with the current queue
    PQueue tempQueue(m_priorFunc, m_heapType, m_structure);
    tempQueue.m_heap = newNode;
    tempQueue.m_size = 1;

    mergeWithQueue(tempQueue);
}

int PQueue::numPatients() const
{
    return m_size;
}

prifn_t PQueue::getPriorityFn() const
{
    return m_priorFunc;
}

Patient PQueue::getNextPatient()
{
    if (m_size == 0)
    {
        throw runtime_error("The queue is empty.");
    }

    Patient rootPatient = m_heap->getPatient();

    PQueue leftQueue(m_priorFunc, m_heapType, m_structure);
    PQueue rightQueue(m_priorFunc, m_heapType, m_structure);

    if (m_structure == SKEW)
    {
        leftQueue.m_heap = m_heap->m_left;
        rightQueue.m_heap = m_heap->m_right;
    }
    else if (m_structure == LEFTIST)
    {
        leftQueue.m_heap = m_heap->m_left;
        rightQueue.m_heap = m_heap->m_right;
    }

    delete m_heap;

    mergeWithQueue(leftQueue);
    mergeWithQueue(rightQueue);

    return rootPatient;
}

void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType)
{
    m_priorFunc = priFn;
    m_heapType = heapType;

    PQueue tempQueue(m_priorFunc, m_heapType, m_structure);
    while (m_size > 0)
    {
        tempQueue.insertPatient(getNextPatient());
    }

    swap(m_heap, tempQueue.m_heap);
    swap(m_size, tempQueue.m_size);
}

void PQueue::setStructure(STRUCTURE structure)
{
    if (m_structure == structure)
    {
        return;
    }

    PQueue tempQueue(m_priorFunc, m_heapType, structure);
    while (m_size > 0)
    {
        tempQueue.insertPatient(getNextPatient());
    }

    swap(m_heap, tempQueue.m_heap);
    swap(m_size, tempQueue.m_size);
    m_structure = structure;
}

STRUCTURE PQueue::getStructure() const
{
    return m_structure;
}

HEAPTYPE PQueue::getHeapType() const
{
    return m_heapType;
}

void PQueue::printPatientQueue() const
{
    if (m_size == 0)
    {
        cout << "The queue is empty." << endl;
        return;
    }

    // Create a temporary queue to preserve the original queue
    PQueue tempQueue(m_priorFunc, m_heapType, m_structure);
    tempQueue.mergeWithQueue(const_cast<PQueue&>(*this)); // const_cast is used to call mergeWithQueue on a const object

    // Print patients in order without affecting the original queue
    int position = 1;
    while (tempQueue.m_size > 0)
    {
        Patient patient = tempQueue.getNextPatient();
        cout << "[" << position++ << "] " << patient << endl;
    }
}


void PQueue::dump() const
{
    if (m_size == 0)
    {
        cout << "Empty heap.\n";
    }
    else
    {
        dump(m_heap);
    }
    cout << endl;
}

void PQueue::dump(Node* pos) const
{
    if (pos != nullptr)
    {
        cout << "(";
        dump(pos->m_left);
        if (m_structure == SKEW)
            cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient();
        else
            cout << m_priorFunc(pos->m_patient) << ":" << pos->m_patient.getPatient() << ":" << pos->m_npl;
        dump(pos->m_right);
        cout << ")";
    }
}

ostream& operator<<(ostream& sout, const Patient& patient)
{
    sout << patient.getPatient()
        << ", temperature: " << patient.getTemperature()
        << ", oxygen: " << patient.getOxygen()
        << ", respiratory: " << patient.getRR()
        << ", blood pressure: " << patient.getBP()
        << ", nurse opinion: " << patient.getOpinion();
    return sout;
}

ostream& operator<<(ostream& sout, const Node& node)
{
    sout << node.getPatient();
    return sout;
}
