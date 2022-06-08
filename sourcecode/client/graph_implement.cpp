//
// Created by Go2Heart on 2022/3/24.
//
#include "graph_implement.h"
#include <QDebug>

AbstractGraph::~AbstractGraph(){}

void ALVex::visit(){
    if(visited)
        return;
    info->gVex->visit(true);
    visited = true;
}

void ALVex::access(const QString &hint){
    info->gVex->access(hint);
}

void ALArc::visit(){
    gArc->visit(true);
}

void ALArc::access(){
    gArc->access();
}

ALGraph::~ALGraph(){
    int i = vexList.size() - 1;
    while(i >= 0)
        DelVex(i--);
    vexList.clear();
}

void ALGraph::AddVex(MyGraphicsVexItem *gvex){
    ALVex newVex(gvex);
    vexList.push_back(newVex);
}

void ALGraph::AddVex(VexInfo *info){
    ALVex newVex(info);
    vexList.push_back(newVex);
}

void ALGraph::AddArc(MyGraphicsLineItem *garc, int weight){
    int strtVex = GetIdOf(garc->stVex());
    int endVex = GetIdOf(garc->edVex());

    ALArc *temp = vexList[strtVex].firstArc;
    ALArc *newArc = new ALArc(garc, endVex, temp);
    newArc->weight = weight;
    vexList[strtVex].firstArc = newArc;

    if(type == UDG){
        temp = vexList[endVex].firstArc;
        newArc = new ALArc(garc, strtVex, temp);
        vexList[endVex].firstArc = newArc;
    }
}

void ALGraph::DelVex(MyGraphicsVexItem *gvex){
    int vexID = GetIdOf(gvex);
    DelVex(vexID);
}

void ALGraph::DelVex(int vexID){
    //Delete out arc
    ALArc *curArc = vexList[vexID].firstArc;
    while(curArc != nullptr){
        ALArc *next = curArc->nextArc;
        delete curArc;
        curArc = next;
    }
    //Delete in arc and adjust arcs
    for(int i = 0; i < vexList.size(); i++){
        if(i == vexID)  continue;
        ALArc *dummyHead = new ALArc(nullptr, 0, vexList[i].firstArc);
        ALArc *preArc = dummyHead;
        while(preArc->nextArc != nullptr){
            if(preArc->nextArc->eVexID == vexID){
                ALArc *next = preArc->nextArc;
                preArc->nextArc = next->nextArc;
                delete next;
                continue;
            }
            if(preArc->nextArc->eVexID > vexID)
                preArc->nextArc->eVexID--;
            preArc = preArc->nextArc;
        }
        vexList[i].firstArc = dummyHead->nextArc;
        delete dummyHead;
    }
    vexList.erase(vexList.begin() + vexID);
}

void ALGraph::DelArc(MyGraphicsLineItem *garc){
    int sVex = GetIdOf(garc->stVex());
    int eVex = GetIdOf(garc->edVex());
    DelArc(sVex, eVex);
}

void ALGraph::DelArc(int sVexID, int eVexID){
    //Delete sVex -> eVex
    if(vexList[sVexID].firstArc != nullptr){
        if(vexList[sVexID].firstArc->eVexID == eVexID){
            ALArc *awaitDel = vexList[sVexID].firstArc;
            vexList[sVexID].firstArc = awaitDel->nextArc;
            delete awaitDel;
        }
        else{
            ALArc *preArc = vexList[sVexID].firstArc;
            while(preArc->nextArc != nullptr && preArc->nextArc->eVexID != eVexID)
                preArc = preArc->nextArc;
            if(preArc->nextArc != nullptr){
                ALArc *awaitDel = preArc->nextArc;
                preArc->nextArc = awaitDel->nextArc;
                delete awaitDel;
            }
        }
    }
    //Delete eVex -> sVex
    if(type == UDG && vexList[eVexID].firstArc != nullptr){
        if(vexList[eVexID].firstArc->eVexID == sVexID){
            ALArc *awaitDel = vexList[eVexID].firstArc;
            vexList[eVexID].firstArc = awaitDel->nextArc;
            delete awaitDel;
        }
        else{
            ALArc *preArc = vexList[eVexID].firstArc;
            while(preArc->nextArc != nullptr && preArc->nextArc->eVexID != sVexID)
                preArc = preArc->nextArc;
            if(preArc->nextArc != nullptr){
                ALArc *awaitDel = preArc->nextArc;
                preArc->nextArc = awaitDel->nextArc;
                delete awaitDel;
            }
        }
    }
}

int ALGraph::GetIdOf(MyGraphicsVexItem *gvex){
    int i = 0;
    while(i < vexList.size() && !vexList[i].info->gVex->equalTo(gvex))
        i++;
    return i == vexList.size() ? -1 : i;
}

ALArc* ALGraph::FindArc(int sID, int eID){
    if(sID < 0 || sID >= vexList.size())
        return nullptr;
    ALArc *p = vexList[sID].firstArc;
    while(p != nullptr){
        if(p->eVexID == eID)
            return p;
        p = p->nextArc;
    }
    return nullptr;
}

void ALGraph::SetWeight(MyGraphicsLineItem *garc, int weight){
    int strtVex = GetIdOf(garc->stVex());
    int endVex = GetIdOf(garc->edVex());
    ALArc *p = vexList[strtVex].firstArc;
    while(p != nullptr){
        if(p->eVexID == endVex){
            p->weight = weight;
            p->gArc->setText(QString::asprintf("%d", weight));
        }
        p = p->nextArc;
    }
    if(type == UDG){
        p = vexList[endVex].firstArc;
        while(p != nullptr){
            if(p->eVexID == strtVex){
                p->weight = weight;
            }
            p = p->nextArc;
        }
    }
}


void ALGraph::ClearVisit(){
    for(int i = 0; i < vexList.size(); i++){
        vexList[i].visited = false;
        vexList[i].info->gVex->visit(false);
    }
}

void ALGraph::ResetDistance(){
    for(int i = 0; i < vexList.size(); i++){
        vexList[i].info->strtVexInfo = nullptr;
        vexList[i].info->distance = VexInfo::INF;
        vexList[i].info->preVexID = -1;
        vexList[i].info->gVex->access("", false);
    }
}
