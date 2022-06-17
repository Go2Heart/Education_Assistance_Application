#include "customObject.h"
#include "customEvent.h"
#include "global.h"

Homework_Student::~Homework_Student() {
    for(int i = 0; i < files.size(); i++) delete files[i];
}

void Homework_Student::Upload(Vector<File*> newfiles) {
    finished = true;
    for(int i = 0; i < files.size(); i++) delete files[i];
    files = newfiles;
}

bool EventGroup::VerifyDuration(Vector<Duration> durations) {
    for(int i = 0; i < lessons.size(); i++)
        if(lessonGroup.GetLesson(lessons[i]->lessonId)->cross(durations)) return false;
    for(int i = 0; i < activities.size(); i++)
        for(int j = 0; j < durations.size(); j++)
            if(activityGroup.GetActivity(activities[i])->duration.cross(durations[j])) return false;
    return true;
}

void EventGroup::AddLesson(int lessonId) {
    Lesson* nowLesson = lessonGroup.GetLesson(lessonId);
    Lesson_Student* newLesson = new Lesson_Student(lessonId);
    lessons.push_back(newLesson);
    
    lessonNameSort.InsertNode(nowLesson->Name(), nowLesson);
    lessonIdSort.InsertNode(lessonId, newLesson);
}

void EventGroup::AddActivity(int ActivityId) {
    Activity* nowActivity = activityGroup.GetActivity(ActivityId);
    /*Vector<Duration> tmp;
    tmp.push_back(nowActivity->duration);
    if(!VerifyDuration(tmp)) {
        puts("error input!");
    } */   
    activities.push_back(ActivityId);
    
    activityNameSort.InsertNode(nowActivity->name, nowActivity);
    activityPlaceSort.InsertNode(nowActivity->place, nowActivity);
    activityIdSort.InsertNode(ActivityId, nowActivity);
    
    
}