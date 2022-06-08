#include "graph.h"

int humanSpeed = 1, bicycleSpeed = 2, busCost = 1800, shuttleCost = 3600;
Vector<Timer> bus, shuttle;
Heap pq;

void Graph::GetColor(int id, int c) {
	p[id].SetBel(c);
	for(int i = head[id]; i; i = e[i].Nxt()) if(p[e[i].To()].Bel() == -1) GetColor(e[i].To(), c);
}

void Graph::UpdGraph(Timer t) {
	for(int i = 0; i < m; i++) e[i].setSpdRatio((rand() % 4000 + 3000) / 10000.0);
	lastTime = t;
}

void Graph::Dij(int x, int mode) {
	for(int i = 0; i < n; i++) if(p[i].Bel() == p[x].Bel()) dis[i] = INF, vis[i] = false;
	dis[x] = 0; sour[x] = x;
	//priority_queue<Node> pq;
	//pq.push(Node(0, x));
	pq.Push(0, x);
	while(pq.Size()) {
		HeapNode tmp;
		pq.Pop(tmp);
		if(vis[tmp.Id]) continue;
		vis[tmp.Id] = true;
		for(int i = head[tmp.Id]; i; i = e[i].Nxt()) {
			int curDis = dis[tmp.Id];
			switch (mode) {
				case 1 :
					curDis += e[i].Dis();
					break;
				case 2 :
					curDis += e[i].Dis() / (humanSpeed * e[i].Ratio());
					break;
				case 3 :
					curDis += e[i].Dis() / ((e[i].Type() ? bicycleSpeed : humanSpeed) * e[i].Ratio());
					break;
			}
			if(dis[e[i].To()] > curDis) {
				dis[e[i].To()] = curDis;
				sour[e[i].To()] = tmp.Id;
				pq.Push(curDis, e[i].To());
			}
		}
	}
}

void Graph::GetCross(Timer t) {
	crossTime = INF; crossType = -1;
	for(int i = 0; i < bus.size(); i++) if(t.HMLessEqual(bus[i])) {
		crossTime = ToInt(bus[i] - t) + busCost;
		crossType = 0;
		break;
	}
	for(int i = 0; i < shuttle.size(); i++) if(t.HMLessEqual(shuttle[i])) {
		if(crossTime > ToInt(shuttle[i] - t) + shuttleCost) {
			crossTime = ToInt(shuttle[i] - t) + shuttleCost;
			crossType = 1;
		}
		break;
	}
}

ResPackage Graph::QueryDis(int s, int t, int mode) {
	Vector<Result> res;
	Timer totTime;
	res.push_back(Result(3, 0, 0));
	if(p[s].Bel() != p[t].Bel()) {
		Dij(s, mode);
		Dij(rt[p[t].Bel()], mode);
		GetCross(lastTime + ToTimer(dis[p[s].Bel()]));

		Vector<Result> tmpRes;
		int now = t;
		while(now != rt[p[t].Bel()]) {
			tmpRes.push_back(Result(2, 0, now));
			int x = sour[now];
			for(int i = head[x]; i; i = e[i].Nxt())
				if(e[i].To() == now)
					tmpRes.push_back(Result(i % 2, mode == 3 && e[i].Type(), (i >> 1) /*- 1*/));
			now = x;
		}
		tmpRes.push_back(Result(2, 0, rt[p[t].Bel()]));
		tmpRes.push_back(Result(p[t].Bel() ^ 1, crossType, crossEdgeId));
		now = rt[p[s].Bel()];
		while(now != s) {
			tmpRes.push_back(Result(2, 0, now));
			int x = sour[now];
			for(int i = head[x]; i; i = e[i].Nxt())
				if(e[i].To() == now)
					tmpRes.push_back(Result(i % 2, mode == 3 && e[i].Type(), (i >> 1)));
			now = x;
		}
		tmpRes.push_back(Result(2, 0, s));
		for(int i = tmpRes.size() - 1; i >= 0; i--) res.push_back(tmpRes[i]);
		totTime = ToTimer(dis[p[s].Bel()] + dis[t] + crossTime);
	} else {
		Dij(s, mode);
		Vector<Result> tmpRes;
		int now = t;
		while(now != s) {
			//printf("%d\n", now);
			tmpRes.push_back(Result(2, 0, now));
			int x = sour[now];
			for(int i = head[x]; i; i = e[i].Nxt())
				if(e[i].To() == now)
					tmpRes.push_back(Result(i % 2, mode == 3 && e[i].Type(), (i >> 1)));
			now = x;
		}
		tmpRes.push_back(Result(2, 0, s));
		for(int i = tmpRes.size() - 1; i >= 0; i--) res.push_back(tmpRes[i]);
		totTime = ToTimer(dis[t]);
	}
	res.push_back(Result(4, 0, 0));
	return ResPackage(totTime, res);
}

void Graph::Init() {
	puts("graph init begin.");
	FILE* in = fopen("/home/ubuntu/Codefield/EduServer/server 2/default_settings/graph.in", "r");
	fscanf(in, "%d %d", &n, &m);
	for(int i = 0; i < n; i++) {
		char name[45];
		fscanf(in, "%s", name);
		p[i] = Point(name, i);
		printf("%s\n",name);
	}
	puts("graph name init end.");
	for(int i = 0; i < m; i++) {
		int x, y, type;
		fscanf(in, "%d%d%d", &x, &y, &type);
		//printf("%d %d %d\n",x,y,type);
		if(type != 3) AddEdge(x, y, GetRandDis(), type == 0);
		else {
			rt[0] = x;
			rt[1] = y;
			crossEdgeId = i;
		}
		//printf("%d %d %d\n",x,y,type);
	}
	puts("graph read complete.");
	GetColor(rt[0], 0);
	GetColor(rt[1], 1);
	fclose(in);

	in = fopen("/home/ubuntu/Codefield/EduServer/server 2/default_settings/traffic.in", "r");
	int x, y, z;
	fscanf(in, "%d", &x);
	for(int i = 0; i < x; i++)
	{
		int y, z;
		fscanf(in, "%d %d", &y, &z);
		bus.push_back(Timer(y, z));
	}
	fscanf(in, "%d", &x);
	for(int i = 0; i < x; i++)
	{
		int y, z;
		fscanf(in, "%d %d", &y, &z);
		shuttle.push_back(Timer(y, z));
	}
		
	fclose(in);
	puts("graph init complete.");
}