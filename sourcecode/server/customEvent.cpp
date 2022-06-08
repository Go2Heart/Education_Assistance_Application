#include "customObject.h"
#include "customEvent.h"
#include "global.h"

Homework_Student::~Homework_Student() {
    if(file) delete file;
}

void Homework_Student::Upload(File* file) {
    finished = true;
    file = file;
}

bool EventGroup::VerifyDuration(Vector<Duration> durations) {
    for(int i = 0; i < lessons.size(); i++)
        if(lessonGroup.GetLesson(lessons[i]->lessonId)->cross(durations)) return false;
    for(int i = 0; i < activities.size(); i++)
        for(int j = 0; j < durations.size(); j++)
            if(activityGroup.GetActivity(activities[i])->Dura().cross(durations[j])) return false;
    return true;
}

void EventGroup::AddLesson(int lessonId) {
    if(!VerifyDuration(lessonGroup.GetLesson(lessonId)->ClassDurations()))
        puts("NO");
    else
        lessons.push_back(new Lesson_Student(lessonId));
}

void EventGroup::AddActivity(int ActivityId) {
    Vector<Duration> tmpDuration;
    tmpDuration.push_back(activityGroup.GetActivity(ActivityId)->Dura());
    if(!VerifyDuration(tmpDuration))
        puts("NO");
    else
        activities.push_back(ActivityId);
}