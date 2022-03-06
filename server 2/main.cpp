#include <cstdio>
#include <iostream>
#include <string>
#include "basicClass.h"
#include "customObject.h"
#include "identity.h"
#include "connect.h"

Timer beg[20];
Timer fin[20];

string GetStr(FILE *fin) {
	char tmp[100];
	fscanf(fin, "%s", tmp);
	string trans = tmp;
	return trans;
}

void initilizeStudent() {
	FILE *in = fopen("student.in", "r");
	int n;
	fscanf(in, "%d", &n);
	for(int i = 1; i <= n; i++) {
		char name[40];
		fscanf(in, "%s", name);
		students.AddStudent(new Student(name));
	}
}

void initializeTime() {
	FILE* in = fopen("time.in", "r");
	int n;
	fscanf(in, "%d", &n);
	for(int i = 1; i <= n; i++) {
		int beg_hour, beg_min, end_hour, end_min;
		fscanf(in, "%d%d%d%d", &beg_hour, &beg_min, &end_hour, &end_min);
		beg[i] = Timer(beg_hour, beg_min);
		fin[i] = Timer(end_hour, end_min);
	}
}

void initiliazeLesson() {
	FILE* in = fopen("class.in", "r");
	int n, m;
	fscanf(in, "%d", &n);
	for(int i = 1; i <= n; i++) {
		fscanf(in, "%d", &m);
		vector<Duration> tmp_durations;
		for(int j = 1; j <= m; j++) {
			int day, bg, ed;
			fscanf(in, "%d%d%d", &day, &bg, &ed);
			Duration tmp_duration = Duration(
				Timer(beg[bg].Hour(), beg[bg].Min(), day),
				Timer(fin[ed].Hour(), fin[ed].Min(), day)
			);
			tmp_durations.push_back(tmp_duration);
		}
		string classPlace = GetStr(in), examPlace = GetStr(in), teacher = GetStr(in), name = GetStr(in), QQnumber = GetStr(in);
		Lesson* tmpLesson = new Lesson(classPlace, examPlace, teacher, name, QQnumber, tmp_durations);
		int lessonId = lessonGroup.AddLesson(tmpLesson);
		fscanf(in, "%d", &m);
		for(int j = 1; j <= m ; j++) {
			char name[40];
			fscanf(in, "%s", name);
			students.GetStudent(name)->Events()->AddLesson(lessonId);
		}
	}
}

int main() {
	/*
	initializeTime();
	initilizeStudent();
	initiliazeLesson();*/
	string tmp = sock.receiveMessage();
	cout<<tmp;
	sock.closeSocket();
	puts("now here");
	tmp = sock.receiveMessage();
	cout<<tmp;
	sock.closeSocket();
}