#ifndef CUSTOMOBJECT_H
#define CUSTOMOBJECT_H

#include "basicClass.h"
#include "identity.h"
#include <string>
#include <vector>
using namespace std;

class File {
private:
    string savePath;
    unsigned long long hash;
public:
    File(string path, unsigned long long hash) :
        savePath(path),
        hash(hash)
    {}
};

class Lesson {
private:
    string classPlace, examPlace, teacher, name, QQnumber;
    vector<Duration> classDurations;
    Duration examDuration;
    vector<string> homeworkInfos;
    vector<File*> files;
    vector<Student*> students;
public:
    Lesson(
        string classPlace,
        string teacher,
        string name,
        string QQnumber,
        vector<Duration> classdurations,
        vector<Student*> students
    ) :
        classPlace(classPlace),
        teacher(teacher),
        name(name),
        QQnumber(QQnumber),
        classDurations(classdurations),
        students(students)
    {}
    string Name() { return name; }
    vector<Duration> ClassDurations() { return classDurations; }
    bool cross(vector<Duration> durations) {
        for(int i = 0; i < durations.size(); i++)
            for(int j = 0; j < classDurations.size(); j++)
                if(classDurations[j].cross(durations[i])) return true;
        return false;
    }
    int AddHomework(string info) {
        homeworkInfos.push_back(info);
        return homeworkInfos.size() - 1;
    }
    void AddFile(File* file) { files.push_back(file); }
    void SetExamPlace(string place) { examPlace = place; }
    void SetExamDura(Duration duration) { examDuration = duration; }
    void SetClassPlace(string place) { classPlace = place; }
    void SetClassDura(vector<Duration> durations) { classDurations = durations; }
};

class Lessons {
private:
    vector<Lesson*> lessons;
public:
    int AddLesson(Lesson* lesson) {
        lessons.push_back(lesson);
        return lessons.size() - 1;
    }
    Lesson* GetLesson(string name) {
        for(int i = 0; i < lessons.size(); i++)
            if(name == lessons[i]->Name())
                return lessons[i];
        return nullptr;
    }
    Lesson* GetLesson(int id) {
        if(id >= lessons.size()) return nullptr;
        return lessons[id];
    }
};

class Activity {
private:
    string place, name;
    int type;
    Duration duration;
    vector<Student*> students;
public:
    Activity(string place, string name, int type, Duration duration, vector<Student*> students) :
        place(place),
        name(name),
        type(type),
        duration(duration),
        students(students)
    {}
    Duration Dura() { return duration; }
};

class Activities {
private:
    vector<Activity*> activities;
public:
    int AddActivities(Activity* activity){
        activities.push_back(activity);
        return activities.size() - 1;
    }
    Activity* GetActivity(int id) {
        return activities[id];
    }
};

#endif