#include <cstdio>
#include <iostream>
//#include <string>
#include "basicClass.h"
#include "customObject.h"
#include "identity.h"
#include "connect.h"
#include "graph.h"

Timer beg[20];
Timer fin[20];
Socket sock;
Students studentGroup;
Activities ActivityGroup;
Lessons lessonGroup;
Graph graph;

String GetStr(FILE *fin) {
	char tmp[100];
	fscanf(fin, "%s", tmp);
	String trans = tmp;
	return trans;
}

void InitStudent() {
	FILE *in = fopen("default_settings/student.in", "r");
	int n;
	fscanf(in, "%d", &n);
	for(int i = 1; i <= n; i++) {
		String studentNumber = GetStr(in), name = GetStr(in);
		studentGroup.AddStudent(new Student(studentNumber, name));
	}
}

void InitTime() {
	FILE* in = fopen("default_settings/time.in", "r");
	int n;
	fscanf(in, "%d", &n);
	for(int i = 1; i <= n; i++) {
		int beg_hour, beg_min, end_hour, end_min;
		fscanf(in, "%d%d%d%d", &beg_hour, &beg_min, &end_hour, &end_min);
		beg[i] = Timer(beg_hour, beg_min);
		fin[i] = Timer(end_hour, end_min);
	}
}

void InitLesson() {
	FILE* in = fopen("default_settings/class.in", "r");
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
				Timer(beg[bg].Hour(), beg[bg].Min(), day, weekbg),
				Timer(fin[ed].Hour(), fin[ed].Min(), day, weeked)
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
		for(int j = 0; j < m; j++)
			tmpStudents[j]->Events()->AddLesson(lessonId);
	}
}

void InitActivity() {
	FILE* in = fopen("default_settings/activity.in", "r");
	int n, m;
	fscanf(in, "%d", &n);
	for(int i = 1; i <= n; i++) {
		int day, bg, ed, type, weekbg, weeked;
		fscanf(in, "%d%d", &weekbg, &weeked);
		fscanf(in, "%d" ,&m);
		fscanf(in, "%d%d%d", &day, &bg, &ed);
		Duration tmpDuration = Duration(
			Timer(beg[bg].Hour(), beg[bg].Min(), day, weekbg),
			Timer(fin[ed].Hour(), fin[ed].Min(), day, weeked)
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
		int activityId = ActivityGroup.AddActivities(tmpActivity);
		for(int j = 0; j < m; j++)
			tmpStudents[j]->Events()->AddActivity(activityId);
	}
}

Vector<String> GetParms(String message, int size) {
	int idx, tot = 0;
	Vector<String> parms;
	String parm;
	for(idx = 1; idx < message.length(); idx++) {
		if(message[idx] == '#') {
			parms.push_back(parm);
			parm.clear();
			if(++tot == size) break;
			continue;
		}
		parm.push_back(message[idx]);
	}
	parms.push_back(parm);
	return parms;
}

unsigned long long GetHash(String s) {
	unsigned long long tmp = 0;
	for(int i = 0; i < s.length(); i++)
		tmp = tmp * 10007 + s[i];
	return tmp;
}

void WriteFile(String path, String file) {
	FILE* out = fopen(path.c_str(), "wb");
	fwrite(file.c_str(), file.length() - 1, 1, out);
	fclose(out);
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
	String a = ToString(233);
	printf("%s\n", a.c_str());
	/*
	while(1) {
		String message = sock.receiveMessage();
		switch (message[0]) {
			case 0x01 : {// 上传课程资料				
				Vector<String> parms = GetParms(message, 3);// lessonname filename file
				Lesson* nowLesson = lessonGroup.GetLesson(parms[0]);
				String savePath = "data/lesson_files/" + nowLesson->Name() + '/' + parms[1];
				unsigned long long tmpHash = GetHash(parms[2]);
				File* file = new File(savePath, tmpHash);
				nowLesson->AddFile(file);
				WriteFile(savePath, parms[2]);
				sock.closeSocket();
				break;
			}
			case 0x02 : {//提交作业
				Vector<String> parms = GetParms(message, 5);//studentId lessonid homeworkid filename file
				Homework_Student* nowHomework = studentGroup.GetStudent(atoi(parms[0].c_str()))->Events()->GetLesson(atoi(parms[1].c_str()))->GetHomework(atoi(parms[2].c_str()));
				String savePath = "data/student_files/" + parms[0] + "/lessons/" + parms[1] + '/' + parms[2] + '/' + to_string(nowHomework->Ver()) + '/' + parms[3];
				nowHomework->Upload(savePath);
				WriteFile(savePath, parms[3]);
				sock.closeSocket();
				break;
			}
			case 0x03 : {
				
				break;
			}
		}
	}*/
}