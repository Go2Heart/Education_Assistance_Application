#ifndef CUSTOMOBJECT_H
#define CUSTOMOBJECT_H

#include "basicClass.h"
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
public:
    Lesson(
        string classPlace,
        string examPlace,
        string teacher,
        string name,
        string QQnumber,
        vector<Duration> classdurations
    ) :
        classPlace(classPlace),
        examPlace(examPlace),
        teacher(teacher),
        name(name),
        QQnumber(QQnumber),
        classDurations(classdurations)
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
    void SetExamDuration(Duration duration) { examDuration = duration; }
    void SetClassPlace(string place) { classPlace = place; }
    void SetClassDuration(vector<Duration> durations) { classDurations = durations; }
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
    }
    Lesson* GetLesson(int id) {
        if(id >= lessons.size()) return nullptr;
        return lessons[id];
    }
};
extern Lessons lessonGroup;
/*
class Activity {
private:
    string place, name;
    int type;
    Duration duration;
public:
    /*enum { PERSONAL = 1, PUBLIC = 2 };
    Activity(string place, string name, int type, Duration duration) :
        place(place),
        name(name),
        type(type),
        duration(duration)
    {}
    Duration Duration() { return duration; }
};*/
#endif