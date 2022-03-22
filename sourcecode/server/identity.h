#ifndef IDENTITY_H
#define IDENTITY_H

#include "customEvent.h"
using namespace std;

class Student {
//private:
private:
	String name, studentNumber;
	EventGroup* events;
public:
	Student(String number, String name) : 
		studentNumber(number),
		name(name),
		events(new EventGroup) 
	{}
	String Name() { return name; }
	String Number() { return studentNumber; }
	EventGroup* Events() { return events; }
};

class Students {
private:
	Vector<Student*> students;
public:
	void AddStudent(Student *student) { students.push_back(student); }
	Student* GetStudent(String number) {
		for(int i = 0; i < students.size(); i++)
			if(students[i]->Number() == number) return students[i];
		return nullptr;
	}
	Student* GetStudent(int id) {
		return students[id];
	}
};
#endif // IDENTITY_H