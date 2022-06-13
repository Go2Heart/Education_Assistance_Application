#include <cstdio>
#include <iostream>
//#include <string>
#include "basicClass.h"
#include "customObject.h"
#include "identity.h"
#include "graph.h"
#include "server.h"
#include "timeHeap.h"
#include "clock.h"

Timer beg[20];
Timer fin[20];
Students studentGroup;
Teachers teacherGroup;
Activities activityGroup;
Lessons lessonGroup;
Alarms alarmGroup;
Graph graph;
Server server;
Clock timeTracker;
TimeHeap timeHeap(20005);

int timeN;
int alarmN;

String GetStr(FILE *fin) {
	char tmp[100];
	fscanf(fin, "%s", tmp);
	String trans = tmp;
	return trans;
}
void IniTeacher() {
    FILE *in = fopen("/home/ubuntu/Codefield/EduServer/server 3/default_settings/teacher.in", "r");
    int n;
    fscanf(in, "%d", &n);
    for(int i = 1; i <= n; i++) {
        String teacherNumber = GetStr(in), name = GetStr(in), password = GetStr(in);
        //todo teacherGroup.AddTeacher()
    }
    fclose(in);

}
void InitStudent() {
	FILE *in = fopen("/home/ubuntu/Codefield/EduServer/server 3/default_settings/student.in", "r");//fopen("default_settings/student.in", "r");
	int n;
	fscanf(in, "%d", &n);
	for(int i = 1; i <= n; i++) {
		String studentNumber = GetStr(in), name = GetStr(in), password = GetStr(in);
		studentGroup.AddStudent(new Student(studentNumber, name, password));
	}
	fclose(in);
}

void InitTime() {
	FILE* in = fopen("/home/ubuntu/Codefield/EduServer/server 3/default_settings/time.in", "r");
	fscanf(in, "%d", &timeN);
	for(int i = 1; i <= timeN; i++) {
		int beg_hour, beg_min, end_hour, end_min;
		fscanf(in, "%d%d%d%d", &beg_hour, &beg_min, &end_hour, &end_min);
		beg[i] = Timer(beg_hour, beg_min);
		fin[i] = Timer(end_hour, end_min);
	}
	fclose(in);
}

void InitLesson() {
	FILE* in = fopen("/home/ubuntu/Codefield/EduServer/server 3/default_settings/class.in", "r");
	int n, m;
	fscanf(in, "%d", &n);
	for(int i = 1; i <= n; i++) {
		int weekbg, weeked;
		fscanf(in, "%d%d", &weekbg, &weeked);
		fscanf(in, "%d", &m);
		Vector<Duration> tmpDurations;
		for(int j = 1; j <= m; j++) {
			int day, bg, ed;
			fscanf(in, "%d%d%d", &day, &bg, &ed);
			Duration tmpDuration = Duration(
				Timer(beg[bg].hour, beg[bg].minute, day, weekbg),
				Timer(fin[ed].hour, fin[ed].minute, day, weeked)
			);
			tmpDurations.push_back(tmpDuration);
		}
		String classPlace = GetStr(in), teacher = GetStr(in), name = GetStr(in), QQnumber = GetStr(in);
		fscanf(in, "%d", &m);
		Vector<Student*> tmpStudents;
		for(int j = 1; j <= m ; j++) {
			String number = GetStr(in);
			tmpStudents.push_back(studentGroup.GetStudent(number));
		}
		Lesson* tmpLesson = new Lesson(classPlace, teacher, name, QQnumber, tmpDurations, tmpStudents);
		int lessonId = lessonGroup.AddLesson(tmpLesson);
		tmpLesson->lessonId = lessonId;
		for(int j = 0; j < m; j++)
			tmpStudents[j]->events->AddLesson(lessonId);
	}
	fclose(in);
}

void InitActivity() {
	FILE* in = fopen("/home/ubuntu/Codefield/EduServer/server 3/default_settings/activity.in", "r");
	int n, m;
	fscanf(in, "%d", &n);
	for(int i = 1; i <= n; i++) {
		int day, bg, ed, type, weekbg, weeked;
		fscanf(in, "%d%d", &weekbg, &weeked);
		fscanf(in, "%d" ,&m);
		fscanf(in, "%d%d%d", &day, &bg, &ed);
		Duration tmpDuration = Duration(
			Timer(beg[bg].hour, beg[bg].minute, day, weekbg),
			Timer(fin[ed].hour, fin[ed].minute, day, weeked)
		);		
		String place = GetStr(in), name = GetStr(in);
		fscanf(in, "%d", &m);
		type = m == 1;
		Vector<Student*> tmpStudents;
		for(int j = 1; j <= m ; j++) {
			String number = GetStr(in);
			tmpStudents.push_back(studentGroup.GetStudent(number));
		}
		Activity* tmpActivity = new Activity(place, name, type, tmpDuration, tmpStudents);
		int activityId = activityGroup.AddActivities(tmpActivity);
		tmpActivity->activityId = activityId;
		for(int j = 0; j < m; j++)
			tmpStudents[j]->events->AddActivity(activityId);
	}
	fclose(in);
}

int GenerateAlarmId() {
	++alarmN;
	return alarmN;
}
void InitAlarms() {
	FILE* in = fopen("/home/ubuntu/Codefield/EduServer/server 3/default_settings/alarm.in", "r");
	int n;
	fscanf(in, "%d", &n);
	for(int i = 1; i <= n; i++) {
		int week, day, hour, min, frequency, belongId, id, enabled;
		fscanf(in, "%d%d%d%d", &week, &day, &hour, &min);
		Timer t = Timer(hour, min, day, week);
		fscanf(in, "%d%d%d%d", &frequency, &belongId, &id, &enabled);
		String desc = GetStr(in), place = GetStr(in);
		alarmGroup.AddAlarm(new Alarm(t, frequency, desc, place, belongId, id, enabled));
		studentGroup.GetStudent(id)->AddAlarm(id);
	}
	fclose(in);
}

int ToBegNum(Timer x) {
	for(int i = 1; i <= timeN; i++)
		if(beg[i].hour == x.hour && beg[i].minute == x.minute) return i;
	return -1;
}

int ToEndNum(Timer x) {
	for(int i = 1; i <= timeN; i++)
		if(fin[i].hour == x.hour && fin[i].minute == x.minute) return i;
	return -1;
}

void LoadToFile() {
	FILE* file = fopen("/home/ubuntu/Codefield/EduServer/server 3/default_settings/activity.in", "w");
	activityGroup.WriteToFile(file);
	fclose(file);
	file = fopen("/home/ubuntu/Codefield/EduServer/server 3/default_settings/alarm.in", "r");
	alarmGroup.WriteToFile(file);
	fclose(file);
	file = fopen("/home/ubuntu/Codefield/EduServer/server 3/default_settings/alarm.in", "r");
	fclose(file);
	file = fopen("/home/ubuntu/Codefield/EduServer/server 3/default_settings/alarm.in", "r");
	fclose(file);
	file = fopen("/home/ubuntu/Codefield/EduServer/server 3/default_settings/alarm.in", "r");
	fclose(file);
}

int main() {
	InitTime();
	puts("phase 1 finished.");
	InitStudent();
	puts("phase 2 finished.");
	InitLesson();
	puts("phase 3 finished.");
	InitActivity();
	puts("phase 4 finished.");
	graph.Init();
	puts("phase 5 finished.");
	server.run();
}