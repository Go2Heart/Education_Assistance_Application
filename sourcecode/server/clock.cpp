#include "clock.h"
#include "graph.h"
#include "global.h"
#include "customObject.h"
#include "timeHeap.h"

void* func(void* p) {
    while(1) {
        usleep(100000 * (1 / timeTracker.Ratio()));
        timeTracker.addTimer();
        graph.UpdGraph(timeTracker.NowTimer());
        //timeTracker.Print();
    }
}

Clock::Clock() {
    timer = Timer(8, 0, 1, 1);
    ratio = 1;
    pthread_create(&thread, NULL, func, NULL);
}

void Clock::addTimer() {
    timer = timer + Timer(0, 1);
    while(timeHeap.Size() && timeHeap.Top().t <= timer) {
        ClockTimer tmp = timeHeap.Top(); timeHeap.Pop();
        Alarm* alarm = alarmGroup.FromId(tmp.originId);
        if(alarm->frequency == Alarm::EVERYDAY) {
            timeHeap.Push(ClockTimer(tmp.t + Timer(0, 0, 1), alarm->id));
        } else if(alarm->frequency == Alarm::WEEKLY) {
            timeHeap.Push(ClockTimer(tmp.t + Timer(0, 0, 0, 1), alarm->id));
        }
        String nowMessage = "   闹钟：" + ToString_Time(alarm->t.hour) + ":" + ToString_Time(alarm->t.minute) + "  内容: " + alarm->desc + "  地点： " + alarm->place;
        studentGroup.GetStudent(alarm->belongId)->TriggerMessage(nowMessage);
    }
    if(timer.week == 21) {
        timeHeap.clear();
        timer.week -= 20;
        for(int i = 0; i < alarmGroup.Size(); i++) {
            Alarm* nowAlarm = alarmGroup.FromId(i);
            timeHeap.Push(ClockTimer(nowAlarm->t, nowAlarm->belongId));
        }
    }
}