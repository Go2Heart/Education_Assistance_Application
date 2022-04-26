//
// Created by Go2Heart on 2022/3/24.
//

#ifndef GRAPH_IMPLEMENT_H
#define GRAPH_IMPLEMENT_H

#include "graph_view.h"

using namespace std;

class VexInfo{
public:
    enum { INF = 2147483647 };
    MyGraphicsVexItem *gVex;
    VexInfo *strtVexInfo = nullptr;
    int preVexID = -1;
    int distance = INF;

    VexInfo(MyGraphicsVexItem *vex) : gVex(vex){}
};

class AbstractGraph{
protected:
    int type;
    int vexNum = 0;

public:
    enum { UDG = 1, DG = 2 };

    AbstractGraph(int _type = DG) : type(_type){}
    virtual ~AbstractGraph() = 0;
    /* Insert */
    virtual void AddVex(MyGraphicsVexItem *gvex) = 0;
    virtual void AddVex(VexInfo *info) = 0;
    virtual void AddArc(MyGraphicsLineItem *garc, int weight = 1) = 0;

    /* Delete */
    virtual void DelVex(MyGraphicsVexItem *gvex) = 0;
    virtual void DelVex(int vexID) = 0;
    virtual void DelArc(MyGraphicsLineItem *garc) = 0;
    virtual void DelArc(int sVexID, int eVexID) = 0;

    /* Find */
    virtual int GetIdOf(MyGraphicsVexItem *gvex) = 0;
    virtual VexInfo* GetInfoOf(int id) = 0;
    virtual VexInfo* GetInfoOf(MyGraphicsVexItem *gvex) = 0;

    /* Modify */
    //virtual void SetText(MyGraphicsVexItem *gvex);
    virtual void SetWeight(MyGraphicsLineItem *garc, int weight) = 0;

    /* Other Function */
    virtual void ClearVisit() = 0;
    virtual void ResetDistance() = 0;

    virtual int Type() const = 0;
};

class ALVex;
class ALArc;
class ALGraph;

/* Classes of ALGraph */

class ALVex {
public:
    bool visited = false;

    VexInfo *info = nullptr;
    ALArc *firstArc = nullptr;

    ALVex(MyGraphicsVexItem *vex){info = new VexInfo(vex);}
    ALVex(VexInfo *_info){info = _info;}
    bool equalTo(const ALVex &v){return info == v.info;}
    void visit();
    void access(const QString &hint = "");
};

class ALArc{
public:
    MyGraphicsLineItem *gArc;
    int weight = 1;
    int eVexID;
    ALArc *nextArc = nullptr;

    ALArc(MyGraphicsLineItem *garc, int eVex, ALArc *next = nullptr) : gArc(garc), eVexID(eVex), nextArc(next){}
    void visit();
    void access();
};

class ALGraph : public AbstractGraph{
private:
    vector<ALVex> vexList;

public:
    ALGraph(int _type = DG) : AbstractGraph(_type){}
    ~ALGraph();
    /* Insert */
    void AddVex(MyGraphicsVexItem *gvex);
    void AddVex(VexInfo *info);
    void AddArc(MyGraphicsLineItem *garc, int weight = 1);

    /* Delete */
    void DelVex(MyGraphicsVexItem *gvex);
    void DelVex(int vexID);
    void DelArc(MyGraphicsLineItem *garc);
    void DelArc(int sVexID, int eVexID);

    /* Find */
    int GetIdOf(MyGraphicsVexItem *gvex);
    VexInfo* GetInfoOf(int id){return vexList[id].info;}
    VexInfo* GetInfoOf(MyGraphicsVexItem *gvex){return vexList[GetIdOf(gvex)].info;}
    ALArc* FindArc(int sID, int eID);

    /* Modify */
    void SetWeight(MyGraphicsLineItem *garc, int weight);
    void ConvertType(int _type);

    /* Other Function */
    void ClearVisit();
    void ResetDistance();

    int Type() const { return type; }
};

#endif // GRAPH_IMPLEMENT_H

