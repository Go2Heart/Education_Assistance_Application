#include "identity.h"
#include "global.h"
#include "customObject.h"

void Teacher::AddLesson(int id) {
    Lesson* nowLesson = lessonGroup.GetLesson(id);
    lessons.push_back(id);
    lessonNameSort.InsertNode(nowLesson->Name(), nowLesson);
}
