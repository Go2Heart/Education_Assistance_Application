#ifndef IDENTITY_H
#define IDENTITY_H

#include "customEvent.h"
using namespace std;

class Student {
//private:
private:
	String name, studentNumber, password;
	EventGroup* events;
public:
	Student(String number, String name, String password) : 
		name(name),
		studentNumber(number),
		password(password),
		events(new EventGroup) 
	{}
	String Name() { return name; }
	String Number() { return studentNumber; }
	String Password() { return password; }
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
	int GetStudentCheck(String number, String password) {
		for(int i = 0; i < students.size(); i++)
			if(students[i]->Number() == number && students[i]->Password() == password) return i;
		return 255;
	}
	Student* GetStudent(int id) {
		return students[id];
	}
};

class Teacher {
//private:
private:
	String name, password;
public:
	Teacher(String name, String password) : 
		name(name),
		password(password)
	{}
	String Name() { return name; }
	String Password() { return password; }
};

class Teachers {
private:
	Vector<Teacher*> teachers;
public:
	void AddTeacher(Teacher* teacher) { teachers.push_back(teacher); }
	int GetTeacherCheck(String name, String password) {
		for(int i = 0; i < teachers.size(); i++)
			if(teachers[i]->Name() == name && teachers[i]->Password() == password) return i;
		return 255;
	}
	Teacher* GetTeacher(int id) {
		return teachers[id];
	}
};
#endif // IDENTITY_H