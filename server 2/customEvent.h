#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include <string>
#include <vector>
#include "basicClass.h"

using namespace std;

class Homework_Student {
private:
    vector<string> savePath;
    bool finished = false;
    int version = 0;
public:
    Homework_Student(){}
    void Upload(string path) {
        finished = true;
        savePath.push_back(path);
        version++;
    }
    int Ver() { return version; }
};

class Lesson_Student {
private:
    int id;
    vector<Homework_Student*> homeworkStates;
public:
    Lesson_Student(int id) : id(id) {}
    int Id() { return id; }
    void AddHomework() { homeworkStates.push_back(new Homework_Student()); }
    Homework_Student* GetHomework(int id) { return homeworkStates[id]; }
};

class EventGroup {
private:
    vector<Lesson_Student*> lessons;
    vector<int> activities;
public:
    void AddLesson(int lessonId);
    void AddActivity(int ActivityId);
    bool VerifyDuration(vector<Duration> durations);
    Lesson_Student* GetLesson(int lessonId) {
        for(int i = 0; i < lessons.size(); i++)
            if(lessons[i]->Id() == lessonId) return lessons[i];
        return nullptr;
    }
};

#endif