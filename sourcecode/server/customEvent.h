#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

//#include <string>
//#include <vector>
#include "data_structure/string.h"
#include "data_structure/vector.h"
#include "basicClass.h"

using namespace std;
class File;
class Homework_Student {
public:
    File* file;
    int id;
    bool finished = false;
    Homework_Student(int id) : id(id) {}
    ~Homework_Student();
    void Upload(File* file);
};

class Lesson_Student {
private:
    Vector<Homework_Student*> homeworkStates;
public:
    int lessonId;
    Lesson_Student(int id) : lessonId(id) {}
    ~Lesson_Student() {
        for(int i = 0; i < homeworkStates.size(); i++) delete(homeworkStates[i]);
    }
    void AddHomework(int id) { homeworkStates.push_back(new Homework_Student(id)); }
    Homework_Student* GetHomework(int id) { return homeworkStates[id]; }
};

class EventGroup {
public:
    Vector<Lesson_Student*> lessons;
    Vector<int> activities;
    ~EventGroup() {
        for(int i = 0; i < lessons.size(); i++) delete(lessons[i]);
    }
    void AddLesson(int lessonId);
    void AddActivity(int ActivityId);
    bool VerifyDuration(Vector<Duration> durations);
    Lesson_Student* GetLesson(int lessonId) {
        for(int i = 0; i < lessons.size(); i++)
            if(lessons[i]->lessonId == lessonId) return lessons[i];
        return nullptr;
    }
};

#endif