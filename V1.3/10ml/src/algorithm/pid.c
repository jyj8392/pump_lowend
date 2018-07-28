#include "include.h"

struct PID Cnt_PID;
double dError = 0;
double Error = 0;

double PID_Calc(struct PID *pp, double NextPoint)
{

	Error = pp->SetPoint - NextPoint;           //P
	pp->SumError += Error;                      //I
	dError = pp->LastError - pp->PrevError;     //D
	pp->PrevError = pp->LastError;
	pp->LastError = Error;
	
	if (abs(NextPoint - pp->SetPoint) < pp->DeadZone) {
		Error = 0;
		pp->SumError = 0;
		dError = 0;
	}
	
	return (pp->Kp * Error              	    //P
			+   pp->Ki * pp->SumError           //I
			+   pp->Kd * dError                 //D
			);
}

void PID_Init(double kp, double ki, double kd, u32 deadzone, double setpoint)
{	
	dError = 0;
	Error = 0;
    memset(&Cnt_PID, 0, sizeof(struct PID));
	Cnt_PID.Kp = kp;
	Cnt_PID.Ki = ki;
	Cnt_PID.Kd = kd;
	Cnt_PID.DeadZone = deadzone;
	Cnt_PID.SetPoint = setpoint;
}
