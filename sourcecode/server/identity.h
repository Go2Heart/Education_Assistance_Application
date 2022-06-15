#ifndef IDENTITY_H
#define IDENTITY_H

#include "customEvent.h"
using namespace std;

class Student {
public:
	String name, studentNumber, password;
	EventGroup* events;
	Vector<int> alarms;
	Vector<String> triggerMessage;
	Student(String number, String name, String password) : 
		name(name),
		studentNumber(number),
		password(password),
		events(new EventGroup) 
	{}
	~Student() { delete(events); }
	void AddAlarm(int id) { alarms.push_back(id); }
	void DeleteAlarm(int id) { 
		for(int i = 0; i < alarms.size(); i++)
			if(alarms[i] == id) alarms.del(i);
	}
	void TriggerMessage(String message) { triggerMessage.push_back(message); }
	void ClearTrigger() { triggerMessage.clear(); }
};

class Students {
private:
	RbTree<String, Student*> numberSort;
public:
	Vector<Student*> students;
	~Students() {
		for(int i = 0; i < students.size(); i++) delete(students[i]);
	}
	void AddStudent(Student *student) {
		students.push_back(student);
		numberSort.InsertNode(student->studentNumber, student);
	}
	Student* GetStudent(String number) {
		Vector<Student*> result = numberSort.Find(number);
		if(result.size()) return result[0];
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
class Lesson;
class Teacher {
public:
	String name, teacherNumber, password;
	Vector<int> lessons;
	RbTree<String, Lesson*> lessonNameSort;
	Teacher(String number, String name, String password) : 
		teacherNumber(number),
		name(name),
		password(password)
	{}
	void AddLesson(int id);
	Vector<Lesson*> FromLessonName(String name) {
		return lessonNameSort.Find(name);
	}
};

class Teachers {
private:
	Vector<Teacher*> teachers;
	RbTree<String, Teacher*> numberSort;
	RbTree<String, Teacher*> nameSort;
public:
	~Teachers() {
		for(int i = 0; i < teachers.size(); i++) delete(teachers[i]);
	}
	void AddTeacher(Teacher* teacher) { 
		teachers.push_back(teacher);
		numberSort.InsertNode(teacher->teacherNumber, teacher);
		nameSort.InsertNode(teacher->name, teacher);
	}
	int GetTeacherCheck(String number, String password) {
		for(int i = 0; i < teachers.size(); i++)
			if(teachers[i]->teacherNumber == number && teachers[i]->password == password) return i;
		return 255;
	}
	Teacher* GetTeacher(int id) {
		return teachers[id];
	}
	Teacher* GetTeacher(String number) {
		Vector<Teacher*> result = numberSort.Find(number);
		if(result.size()) return result[0];
		return nullptr;
	}
	Teacher* FromName(String name) {
		Vector<Teacher*> result = nameSort.Find(name);
		if(result.size()) return result[0];
		return nullptr;
	}
	
};
#endif // IDENTITY_H