#ifndef CUSTOMOBJECT_H
#define CUSTOMOBJECT_H

#include "basicClass.h"
#include "identity.h"
#include "data_structure/string.h"
//#include <string>
//#include <vector>
#include "data_structure/vector.h"
using namespace std;

class Homework {
public:
    String desc;
    Homework(String desc) : desc(desc) {}
};

class File {
public:
    String savePath, name;
    unsigned long long hash;
    File(String path, String name, unsigned long long hash) :
        savePath(path),
        name(name),
        hash(hash)
    {}
};

class Lesson {
private:
    String classPlace, examPlace, teacher, name, QQnumber;
    Vector<Duration> classDurations;
    Duration examDuration;
    Vector<Homework*> homeworks;
    Vector<File*> files;
    Vector<Student*> students;
public:
    int lessonId;
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
    ~Lesson() {
        for(int i = 0; i < files.size(); i++) delete(files[i]);
        for(int i = 0; i < homeworks.size(); i++) delete(homeworks[i]);
    }
    String Name() { return name; }
    String Teacher() { return teacher; }
    String Place() { return classPlace; }
    String QQ() { return QQnumber; }
    Vector<Duration> ClassDurations() { return classDurations; }
    Vector<File*> Files() { return files; }

    bool cross(Vector<Duration> durations) {
        for(int i = 0; i < durations.size(); i++)
            for(int j = 0; j < classDurations.size(); j++)
                if(classDurations[j].cross(durations[i])) return true;
        return false;
    }
    void AddHomework(Homework* homework) {
        homeworks.push_back(homework);
        int id = homeworks.size() - 1;
        for(int i = 0; i < students.size(); i++) {
            students[i]->events->GetLesson(lessonId)->AddHomework(id, homework->desc);
        }
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
    ~Lessons() {
        for(int i = 0; i < lessons.size(); i++) delete(lessons[i]);
    }
    int AddLesson(Lesson* lesson) {
        lessons.push_back(lesson);
        return lessons.size() - 1;
    }
    int size() {return lessons.size(); }
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
    Vector<Lesson*> FromName(String name) {
        Vector<Lesson*> tmp;
        for(int i = 0; i < lessons.size(); i++)
            if(name == lessons[i]->Name())
                tmp.push_back(lessons[i]);
        return tmp;
    }
};

class Activity {
public:
    String place, name;
    int type;
    Duration duration;
    Vector<Student*> students;
    Vector<File*> files;
    int activityId;
    Activity(String place, String name, int type, Duration duration, Vector<Student*> students) :
            place(place),
            name(name),
            type(type),
            duration(duration),
            students(students)
    {}
    ~Activity() {
        for(int i = 0; i < files.size(); i++) delete(files[i]);
    }
    void AddFile(File* file) { files.push_back(file); }
};

class Activities {
private:
    Vector<Activity*> activities;
public:
    ~Activities() {
        for(int i = 0; i < activities.size(); i++) delete(activities[i]);
    }
    int AddActivities(Activity* activity){
        activities.push_back(activity);
        return activities.size() - 1;
    }
    int GetActivityId(String name) {
        for(int i = 0; i < activities.size(); i++)
            if(name == activities[i]->name)
                return i;
        return -1;
    }
    int size() {return activities.size(); }
    Activity* GetActivity(int id) {
        return activities[id];
    }
    Vector<Activity*> FromName(String name) {
        Vector<Activity*> result;
        for(int i = 0; i < activities.size(); i++) if(activities[i]->name == name) result.push_back(activities[i]);
        return result;
    }
    void WriteToFile(FILE* file);
};

class Alarm {
public:
    enum TYPE { ONCE = 0, EVERYDAY, WEEKLY };
    Timer t;
    int frequency, belongId, id, enabled;
    String desc, place;
    Alarm(Timer t, int frequency, String desc, String place, int belongId, int id, int enabled) :
        t(t), frequency(frequency), desc(desc), place(place), belongId(belongId), id(id), enabled(enabled) {}  
    void updateMsg(Timer t, int frequency, String desc, String place, int enabled) {
        this->t = t;
        this->frequency = frequency;
        this->desc = desc;
        this->place = place;
        this->enabled = enabled;
    }
    Timer GetNextTime(Timer x) {
        if(frequency == ONCE) {
            if(t < x) return Timer(0, 0, 0, 21);
            else return t;
        } else if(frequency == EVERYDAY) {
            if(t.HMLess(x)) return Timer(t.hour, t.minute, x.day, x.week) + Timer(0, 0, 1);
            else return Timer(t.hour, t.minute, x.day, x.minute);
        } else if(frequency == WEEKLY) {
            if(t.day < x.day || (t.day == x.day && t.HMLess(x))) return Timer(t.hour, t.minute, t.day, x.week) + Timer(0, 0, 0, 1);
            else return Timer(t.hour, t.minute, t.day, x.week);
        }
    }
};

class Alarms {
private:
    Vector<Alarm*> alarms;
public:
    ~Alarms() {
        for(int i = 0; i < alarms.size(); i++) delete(alarms[i]);
    }
    void AddAlarm(Alarm* alarm) { alarms.push_back(alarm); }
    void DeleteAlarm(Alarm* alarm) {
        for(int i = 0; i < alarms.size(); i++) {
            if(alarms[i] == alarm) alarms.del(i);
        }
    }
    Alarm* FromId(int id) { 
        for(int i = 0; i < alarms.size(); i++) {
            if(alarms[i]->id == id) return alarms[i];
        }
        return nullptr;
    }
    int Size() { return alarms.size(); }
    void WriteToFile(FILE* file) {
        fprintf(file, "%d", alarms.size());
        for(int i = 0; i < alarms.size(); i++) {
            Alarm* nowAlarm = alarms[i];
            fprintf(file, "%d %d %d %d\n", nowAlarm->t.week, nowAlarm->t.day, nowAlarm->t.hour, nowAlarm->t.minute);
            fprintf(file, "%d %d %d %d\n", nowAlarm->frequency, nowAlarm->belongId, nowAlarm->id, nowAlarm->enabled);
            fprintf(file, "%s %s\n", nowAlarm->desc.c_str(), nowAlarm->place.c_str());
        }
    }
};

#endif