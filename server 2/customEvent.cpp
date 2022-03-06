#include "customEvent.h"

bool EventGroup::VerifyDuration(vector<Duration> durations) {
    for(int i = 0; i < lessons.size(); i++)
        if(lessonGroup.GetLesson(lessons[i]->Id())->cross(durations)) return false;
    /*for(int i = 0; i < activities.size(); i++)
        if(activities[i]->Duration().cross(duration)) return true;*/
    return true;
}

void EventGroup::AddLesson(int lessonId) {
    if(!VerifyDuration(lessonGroup.GetLesson(lessonId)->ClassDurations()))
        puts("NO");
    else
        lessons.push_back(new Lesson_Student(lessonId));
}
/*
void EventGroup::AddActivity(Activity *activity) {
    if(VerifyDuration(activity->Duration()))
        emit Conflict();
    else
        activities.push_back(activity);
}

void EventGroup::Addhomework(Lesson *lesson, Homework *homework) {
    for(int i = 0; i < lessons.size(); i++)
        if(lessons[i]->Name() == lesson->Name())
            lessons[i]->AddHomework(homework);
}*/
