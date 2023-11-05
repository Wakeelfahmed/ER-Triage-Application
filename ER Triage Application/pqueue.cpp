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
PQueue::~PQueue() { clear(); }
void PQueue::clear() {
	if (m_size > 0) {
		clear(m_heap);
		m_heap = nullptr;
		m_size = 0;
	}
}
void PQueue::clear(Node* pos) {
	if (pos != nullptr) {
		clear(pos->m_left);
		clear(pos->m_right);
		delete pos;
	}
}
PQueue::PQueue(const PQueue& rhs)
{
	m_heap = nullptr;
	m_size = 0;
	m_priorFunc = rhs.m_priorFunc;
	m_heapType = rhs.m_heapType;
	m_structure = rhs.m_structure;

	if (rhs.m_size > 0) {
		m_heap = new Node(rhs.m_heap->getPatient());
		m_heap->m_left = clone(rhs.m_heap->m_left);
		m_heap->m_right = clone(rhs.m_heap->m_right);
		m_size = rhs.m_size;
	}
}
PQueue& PQueue::operator=(const PQueue& rhs) {
	if (this != &rhs) {
		clear();
		m_priorFunc = rhs.m_priorFunc;
		m_heapType = rhs.m_heapType;
		m_structure = rhs.m_structure;

		if (rhs.m_size > 0) {
			m_heap = new Node(rhs.m_heap->getPatient());
			m_heap->m_left = clone(rhs.m_heap->m_left);
			m_heap->m_right = clone(rhs.m_heap->m_right);
			m_size = rhs.m_size;
		}
	}
	return *this;
}
void PQueue::mergeWithQueue(PQueue& rhs) {
	if (m_priorFunc != rhs.m_priorFunc || m_structure != rhs.m_structure)
		throw std::domain_error("Cannot merge with different priority functions or structures");

	if (this == &rhs)
		throw std::invalid_argument("Cannot merge a queue with itself");

	m_heap = merge(m_heap, rhs.m_heap, m_priorFunc, m_heapType, m_structure);
	m_size += rhs.m_size;

	rhs.clear();
}
void PQueue::insertPatient(const Patient& patient) {
	Node* newPatient = new Node(input);
	m_heap = merge(m_heap, newPatient, m_priorFunc, m_heapType, m_structure);
	m_size++;
}
int PQueue::numPatients() const { return m_size; }
prifn_t PQueue::getPriorityFn() const { return m_priorFunc; }
Patient PQueue::getNextPatient() {
	if (m_size == 0)
		throw std::out_of_range("Queue is empty");

	Node* oldHeap = m_heap;
	m_heap = merge(m_heap->m_left, m_heap->m_right, m_priorFunc, m_heapType, m_structure);
	Patient nextPatient = oldHeap->getPatient();
	delete oldHeap;
	m_size--;

	return nextPatient;
}
void PQueue::setPriorityFn(prifn_t priFn, HEAPTYPE heapType) {
	if (m_priorFunc != priFn || m_heapType != heapType) {
		m_priorFunc = priFn;
		m_heapType = heapType;
		m_heap = rebuildHeap(m_heap, m_priorFunc, m_heapType, m_structure);
	}
}
void PQueue::setStructure(STRUCTURE structure) {
	if (m_structure != structure) {
		m_structure = structure;
		m_heap = rebuildHeap(m_heap, m_priorFunc, m_heapType, m_structure);
	}
}
STRUCTURE PQueue::getStructure() const { return m_structure; }
HEAPTYPE PQueue::getHeapType() const { return m_heapType; }
void PQueue::printPatientQueue() const {
	printPatientQueue(m_heap);
	std::cout << std::endl;
}
void PQueue::dump() const {
	if (m_size == 0)
		cout << "Empty heap.\n";
	else
		dump(m_heap);
	cout << endl;
}
void PQueue::dump(Node* pos) const {
	if (pos != nullptr) {
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
ostream& operator<<(ostream& sout, const Patient& patient) {
	sout << patient.getPatient()
		<< ", temperature: " << patient.getTemperature()
		<< ", oxygen: " << patient.getOxygen()
		<< ", respiratory: " << patient.getRR()
		<< ", blood pressure: " << patient.getBP()
		<< ", nurse opinion: " << patient.getOpinion();
	return sout;
}
ostream& operator<<(ostream& sout, const Node& node) {
	sout << node.getPatient();
	return sout;
}