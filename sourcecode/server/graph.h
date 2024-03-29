#ifndef GRAPH_H
#define GRAPH_H

//#include <string>
#include "data_structure/string.h"
#include "data_structure/vector.h"
#include "data_structure/heap.h"
#include "data_structure/rbtree.h"
//#include <vector>
#include "basicClass.h"
#include <cstdlib>
#include <cstring>
#include <queue>
using namespace std;

extern int humanSpeed, bicycleSpeed, busCost, shuttleCost; 
extern Vector<Timer> bus, shuttle;

class Point {
private:
	String name;
	int id, belong = -1;
public:
	Point(String name, int id) :
		name(name),
		id(id)
	{}
	Point() {}
	String Name() { return name; }
	int Id() { return id; }
	int Bel() { return belong; }
	void SetBel(int x) { belong  = x; }
};

class Edge {
private:
	int to, next;
	int dis, type;
	int originId;
	double spdRatio = 1;
public:
	Edge(int to, int next, int dis, int type, int origin) :
		to(to),
		next(next),
		dis(dis),
		type(type),
		originId(origin)
	{}
	Edge() {}
	void setSpdRatio(double ratio) { spdRatio = ratio; }
	int Dis() { return dis; }
	int To() { return to; }
	int Nxt() { return next; }
	int Type() { return type; }
	int Ori() { return originId; }
	double Ratio() { return spdRatio; }
};

struct Result {
// type: 0: x -> y 1: y -> x 2: point 3: start 4: end 5: crossTime
// tool: 对于校区内的路:0: foot 1: bicycle
// tool：对于校区间的路:0: bus 1: shuttle
	int type, tool;
	int id;
	Result() {}
	Result(int type, int tool, int id) :
		type(type),
		tool(tool),
		id(id)
	{}
};

struct ResPackage {
	Timer timeCost;
	Vector<Result> v;
	ResPackage(Timer t, Vector<Result> v) :
		timeCost(t),
		v(v)
	{}
	ResPackage() {}
	ResPackage operator + (const ResPackage &x) {
		ResPackage tmp;
		tmp.timeCost = timeCost + x.timeCost;
		for(int i = 0; i < v.size(); i++) tmp.v.push_back(v[i]);
		for(int i = 0; i < x.v.size(); i++) tmp.v.push_back(x.v[i]);
		return tmp;
	}
};

class Graph {
private:
	int head[105];
	int rt[2];
	int col[105];
	int dis[105], sour[105];
	bool vis[105];
	Timer lastTime;
	RbTree<String, int> nameSort;
public:
	int num = 1, n, m;
	int crossEdgeId, crossTime, crossType;
	Point p[105];
	Edge e[1005];
	enum { INF = 1 << 30 };
	void AddEdge(int x, int y, int dis, int type, int ori) {
		++num; e[num] = Edge(y, head[x], dis, type, ori); head[x] = num;
		++num; e[num] = Edge(x, head[y], dis, type, ori); head[y] = num;
	}
	int GetRandDis() { return 25 + rand()%10; }
	void GetColor(int id, int c);
	void GetCross(Timer t);
	int GetPointId(String s) {
		return nameSort.Find(s)[0];
	}
	void Dij(int x, int mode);
	ResPackage QueryDis(int s, int t, int mode);
	ResPackage QueryDis(Vector<int> v, int mode) {
		ResPackage tmp;
		for(int i = 1; i < v.size(); i++) {
			tmp = tmp + QueryDis(v[i - 1], v[i], mode);
		}
		return tmp;
	}
	void UpdGraph(Timer t);
	void Init();
	Point GetPoint(int id) { return p[id]; }
};

struct DisStruct {
	int val, id;
	friend bool operator<(DisStruct x, DisStruct y) { return x.val < y.val; }
	DisStruct() {}
	DisStruct(int val, int id) : val(val), id(id) {}
};

#endif
