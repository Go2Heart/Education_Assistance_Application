#include "customObject.h"
#include "customEvent.h"
#include "global.h"

bool EventGroup::VerifyDuration(Vector<Duration> durations) {
    for(int i = 0; i < lessons.size(); i++)
        if(lessonGroup.GetLesson(lessons[i]->Id())->cross(durations)) return false;
    for(int i = 0; i < activities.size(); i++)
        for(int j = 0; j < durations.size(); j++)
            if(ActivityGroup.GetActivity(activities[i])->Dura().cross(durations[j])) return false;
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
    tmpDuration.push_back(ActivityGroup.GetActivity(ActivityId)->Dura());
    if(!VerifyDuration(tmpDuration))
        puts("NO");
    else
        activities.push_back(ActivityId);
}