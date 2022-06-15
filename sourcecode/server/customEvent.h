#ifndef CUSTOMEVENT_H
#define CUSTOMEVENT_H

//#include <string>
//#include <vector>
#include "data_structure/string.h"
#include "data_structure/vector.h"
#include "basicClass.h"
#include "data_structure/rbtree.h"

using namespace std;
class File;
class Homework_Student {
public:
    Vector<File*> files;
    int id;
    String desc;
    bool finished = false;
    Homework_Student(int id, String desc) : id(id), desc(desc) {}
    ~Homework_Student();
    void Upload(Vector<File*> files);
};

class Lesson_Student {
private:
    Vector<Homework_Student*> homeworkStates;
    RbTree<String, Homework_Student*> nameSort;
public:
    int lessonId;
    Lesson_Student(int id) : lessonId(id) {}
    ~Lesson_Student() {
        for(int i = 0; i < homeworkStates.size(); i++) delete(homeworkStates[i]);
    }
    void AddHomework(int id, String desc) { 
        Homework_Student* newHomework = new Homework_Student(id, desc);
        homeworkStates.push_back(newHomework);
        nameSort.InsertNode(desc, newHomework); 
    }
    Homework_Student* GetHomework(int id) { return homeworkStates[id]; }
    Vector<Homework_Student*> HomeworkStates() { return homeworkStates; }
    Vector<Homework_Student*> FromHomeworkName(String name) { return nameSort.Find(name); }
};

class Lesson;
class EventGroup {
    RbTree<int, Lesson_Student*> idSort;
    RbTree<String, Lesson*> lessonNameSort;
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
        Vector<Lesson_Student*> result = idSort.Find(lessonId);
        if(result.size()) return result[0];
        else return nullptr;
        /*
        for(int i = 0; i < lessons.size(); i++)
            if(lessons[i]->lessonId == lessonId) return lessons[i];
        return nullptr;
        */
    }
    Vector<Lesson*> FromLessonName(String name) { return lessonNameSort.Find(name); }
};

#endif