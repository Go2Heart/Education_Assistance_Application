#include "global.h"
#include "customObject.h"

bool nameCompare(Activity* x, Activity* y) { return x->name < y->name; }
bool nameEqual(Activity* x, Activity* y) { return x->name ==  y->name; }

void Lessons::WriteToFile(FILE* file) {
    fprintf(file, "%d\n", lessons.size());
    for(int i = 0; i < lessons.size(); i++) {
        Vector<Duration> durations = lessons[i]->ClassDurations();
        fprintf(file, "%d %d\n", durations[0].begin.week, durations[0].end.week);
        fprintf(file, "%d\n", durations.size());
        for(int j = 0; j < durations.size(); j++) {
            fprintf(file, "%d %d %d\n",
                durations[j].begin.day,
                ToBegNum(Timer(durations[j].begin.hour, durations[j].begin.minute)),
                ToEndNum(Timer(durations[j].end.hour, durations[j].end.minute)));
        }
        fprintf(file, "%s %s %s %s\n",
            lessons[i]->Place().c_str(),
            lessons[i]->Teacher().c_str(),
            lessons[i]->Name().c_str(),
            lessons[i]->QQ().c_str()
        );
        fprintf(file, "%d\n", lessons[i]->students.size());
        for(int j = 0; j < lessons[i]->students.size(); j++) {
            fprintf(file, "%s\n", lessons[i]->students[j]->studentNumber.c_str());
        }
        fprintf(file, "%d\n", lessons[i]->homeworks.size());
        for(int j = 0; j < lessons[i]->homeworks.size(); j++) {
            fprintf(file, "%s\n", lessons[i]->homeworks[j]->desc.c_str());
        }
        for(int j = 0; j < lessons[i]->students.size(); j++) {
            for(int k = 0; k < lessons[i]->homeworks.size(); k++) {
                Homework_Student* nowHomework = lessons[i]->students[j]->events->GetLesson(lessons[i]->lessonId)->GetHomework(k);
                fprintf(file, "%d\n", nowHomework->files.size());
                for(int kk = 0; kk < nowHomework->files.size(); kk++) {
                    fprintf(file, "%s %s %s\n",
                        nowHomework->files[kk]->savePath.c_str(),
                        nowHomework->files[kk]->name.c_str(),
                        nowHomework->files[kk]->hash.c_str());
                }
            }
        }
        fprintf(file, "%d\n", lessons[i]->files.size());
        for(int j = 0; j < lessons[i]->files.size(); j++) {
            fprintf(file, "%s %s %s\n",
                lessons[i]->files[j]->savePath.c_str(),
                lessons[i]->files[j]->name.c_str(),
                lessons[i]->files[j]->hash.c_str());
        }
    }
}

void Activities::WriteToFile(FILE* file) {
    fprintf(file, "%d\n", activities.size());
    for(int i = 0; i < activities.size(); i++) {
        Activity* nowActivity = activities[i];
        Timer bg = nowActivity->duration.begin;
        Timer ed = nowActivity->duration.end;
        fprintf(file, "%d %d\n", bg.week, ed.week);
        fprintf(file, "1\n");
        fprintf(file, "%d %d %d\n", bg.day, ToBegNum(bg), ToEndNum(ed));
        fprintf(file, "%s %s\n", nowActivity->place.c_str(), nowActivity->name.c_str());
        fprintf(file, "%d\n", nowActivity->students.size());
        for(int j = 0; j < nowActivity->students.size(); j++)
            fprintf(file, "%s\n", nowActivity->students[j]->studentNumber.c_str());
    }
}