#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

#include <string>
#include <vector>
#include "basicClass.h"
#include "customObject.h"
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
/*
class Activity {
private:
    QString place, name;
    int type;
    Duration duration;
public:
    /*enum { PERSONAL = 1, PUBLIC = 2 };
    Activity(QString place, QString name, int type, Duration duration) :
        place(place),
        name(name),
        type(type),
        duration(duration)
    {}
    Duration Duration() { return duration; }
};*/

class EventGroup {
private:
    vector<Lesson_Student*> lessons;
    //QVector<Activity*> activities;
public:
    //void SetSavePath(QString path) { savePath = path; }
    void AddLesson(int lessonId);
    //void AddActivity(Activity *activity);
    bool VerifyDuration(vector<Duration> durations);
};

#endif // CUSTOMEVENT_H
