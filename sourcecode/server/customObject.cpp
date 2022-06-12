#include "global.h"
#include "customObject.h"

void Activities::WriteToFile(FILE* file) {
    fprintf(file, "%d", activities.size());
    for(int i = 0; i < activities.size(); i++) {
        Activity* nowActivity = activities[i];
        Timer bg = nowActivity->duration.begin;
        Timer ed = nowActivity->duration.end;
        fprintf(file, "%d %d\n", bg.week, ed.week);
        fprintf(file, "1\n");
        fprintf(file, "%d %d %d\n", bg.day, ToBegNum(bg), ToEndNum(ed));
        fprintf(file, "%s %s\n", nowActivity->place.c_str(), nowActivity->name.c_str());
        fprintf(file, "%d", nowActivity->students.size());
        for(int j = 0; j < nowActivity->students.size(); j++)
            fprintf(file, "%s\n", nowActivity->students[j]->studentNumber.c_str());
    }
}