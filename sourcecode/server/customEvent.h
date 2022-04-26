#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

//#include <string>
//#include <vector>
#include "data_structure/string.h"
#include "data_structure/vector.h"
#include "basicClass.h"

using namespace std;

class Homework_Student {
private:
    Vector<String> savePath;
    bool finished = false;
    int version = 0;
public:
    Homework_Student(){}
    void Upload(String path) {
        finished = true;
        savePath.push_back(path);
        version++;
    }
    int Ver() { return version; }
};

class Lesson_Student {
private:
    int id;
    Vector<Homework_Student*> homeworkStates;
public:
    Lesson_Student(int id) : id(id) {}
    int Id() { return id; }
    void AddHomework() { homeworkStates.push_back(new Homework_Student()); }
    Homework_Student* GetHomework(int id) { return homeworkStates[id]; }
};

class EventGroup {
private:
    Vector<Lesson_Student*> lessons;
    Vector<int> activities;
public:
    void AddLesson(int lessonId);
    void AddActivity(int ActivityId);
    bool VerifyDuration(Vector<Duration> durations);
    Lesson_Student* GetLesson(int lessonId) {
        for(int i = 0; i < lessons.size(); i++)
            if(lessons[i]->Id() == lessonId) return lessons[i];
        return nullptr;
    }
};

#endif