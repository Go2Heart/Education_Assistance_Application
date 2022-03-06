#ifndef IDENTITY_H
#define IDENTITY_H

#include "customEvent.h"

class Student {
private:
	string name;
	EventGroup *events;
public:
	Student(string name) : 
		name(name)
	{}
	string Name() { return name; }
	EventGroup *Events() { return events; }
};

class Students {
private:
	vector<Student*> students;
public:
	void AddStudent(Student *student) { students.push_back(student); }
	Student *GetStudent(string name) {
		for(int i = 0; i < students.size(); i++)
			if(students[i]->Name() == name) return students[i];
		return nullptr;
	}
};
extern Students students;
#endif // IDENTITY_H
