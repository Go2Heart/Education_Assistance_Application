#ifndef CUSTOMOBJECT_H
#define CUSTOMOBJECT_H

#include "basicClass.h"
#include "identity.h"
#include "data_structure/string.h"
//#include <string>
//#include <vector>
#include "data_structure/vector.h"
using namespace std;

class File {
private:
    String savePath;
    unsigned long long hash;
public:
    File(String path, unsigned long long hash) :
        savePath(path),
        hash(hash)
    {}
};

class Lesson {
private:
    String classPlace, examPlace, teacher, name, QQnumber;
    Vector<Duration> classDurations;
    Duration examDuration;
    Vector<String> homeworkInfos;
    Vector<File*> files;
    Vector<Student*> students;
public:
    Lesson(
        String classPlace,
        String teacher,
        String name,
        String QQnumber,
        Vector<Duration> classdurations,
        Vector<Student*> students
    ) :
        classPlace(classPlace),
        teacher(teacher),
        name(name),
        QQnumber(QQnumber),
        classDurations(classdurations),
        students(students)
    {}
    String Name() { return name; }
    Vector<Duration> ClassDurations() { return classDurations; }
    bool cross(Vector<Duration> durations) {
        for(int i = 0; i < durations.size(); i++)
            for(int j = 0; j < classDurations.size(); j++)
                if(classDurations[j].cross(durations[i])) return true;
        return false;
    }
    int AddHomework(String info) {
        homeworkInfos.push_back(info);
        return homeworkInfos.size() - 1;
    }
    void AddFile(File* file) { files.push_back(file); }
    void SetExamPlace(String place) { examPlace = place; }
    void SetExamDura(Duration duration) { examDuration = duration; }
    void SetClassPlace(String place) { classPlace = place; }
    void SetClassDura(Vector<Duration> durations) { classDurations = durations; }
};

class Lessons {
private:
    Vector<Lesson*> lessons;
public:
    int AddLesson(Lesson* lesson) {
        lessons.push_back(lesson);
        return lessons.size() - 1;
    }
    Lesson* GetLesson(String name) {
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
    String place, name;
    int type;
    Duration duration;
    Vector<Student*> students;
public:
    Activity(String place, String name, int type, Duration duration, Vector<Student*> students) :
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
    Vector<Activity*> activities;
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