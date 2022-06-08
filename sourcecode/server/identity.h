#ifndef IDENTITY_H
#define IDENTITY_H

#include "customEvent.h"
using namespace std;

class Student {
public:
	String name, studentNumber, password;
	EventGroup* events;
	Student(String number, String name, String password) : 
		name(name),
		studentNumber(number),
		password(password),
		events(new EventGroup) 
	{}
	~Student() { delete(events); }
};

class Students {
private:
	Vector<Student*> students;
public:
	~Students() {
		for(int i = 0; i < students.size(); i++) delete(students[i]);
	}
	void AddStudent(Student *student) { students.push_back(student); }
	Student* GetStudent(String number) {
		for(int i = 0; i < students.size(); i++)
			if(students[i]->studentNumber == number) return students[i];
		return nullptr;
	}
	int GetStudentCheck(String number, String password) {
		for(int i = 0; i < students.size(); i++)
			if(students[i]->studentNumber == number && students[i]->password == password) return i;
		return 255;
	}
	Student* GetStudent(int id) {
		return students[id];
	}
};

class Teacher {
public:
	String name, password;
	Vector<int> lessons;
	Teacher(String name, String password) : 
		name(name),
		password(password)
	{}
	void AddLesson(int id) { lessons.push_back(id); }
};

class Teachers {
private:
	Vector<Teacher*> teachers;
public:
	~Teachers() {
		for(int i = 0; i < teachers.size(); i++) delete(teachers[i]);
	}
	void AddTeacher(Teacher* teacher) { teachers.push_back(teacher); }
	int GetTeacherCheck(String name, String password) {
		for(int i = 0; i < teachers.size(); i++)
			if(teachers[i]->name == name && teachers[i]->password == password) return i;
		return 255;
	}
	Teacher* GetTeacher(int id) {
		return teachers[id];
	}
};
#endif // IDENTITY_H