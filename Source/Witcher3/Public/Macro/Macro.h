#pragma once
class FDoOnce
{
public:
	FORCEINLINE void Reset() { Used = false; };
	FORCEINLINE bool Execute() { if (!Used) { Used = true; return Used; } else return false; };
private:
	bool Used = false;
};
#define BEGAVIOR_MONITORING(Condition,DoOnceOnTrue,DoOnceOnFalse,OnStartFunction,OnEndFunction,OnTrueFunction,OnFalseFunction)\
if (Condition)\
{if((DoOnceOnTrue)->Execute())\
{OnStartFunction();(DoOnceOnFalse)->Reset();}\
OnTrueFunction();}\
else\
{if((DoOnceOnFalse)->Execute())\
{OnEndFunction();(DoOnceOnTrue)->Reset();}\
OnFalseFunction();\
}
#define DELAY(TimerManager,User,TimerHandle,DelayTime,Function)\
if(DelayTime>0)\
TimerManager.SetTimer(TimerHandle, User,Function, DelayTime, false);\
else\
{TimerManager.SetTimerForNextTick(User, Function);\
}