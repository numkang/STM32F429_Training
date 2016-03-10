#include "PID.h"

float Kp = 2.2, Ki = 0.05, Kd = 0.05;
float setpoint = 0.0;
float dt = 1.0/(150.0*2.0);
float error, errorI = 0.0, errorD, previous_error = 0.0;
float output = 0.0;
char text[100];

void PID_control(float ang[3]){
	error = setpoint - ang[1];

	errorI += error*dt;
	if(errorI > 800) errorI = 800;
	else if(errorI < -800) errorI = -800;

	//errorD = (error - previous_error)/dt;
	errorD = (float)gyroRaw[2]*(-1.0);

	output = Kp*error + Ki*errorI + Kd*errorD;

	previous_error = error;

    TIM1->CCR2 = 1300 - output; //L PA9
    TIM1->CCR3 = 1300 + output; //R PA10
}

void set_Kp_Func(){
	return_val(&Kp);
}

void set_Ki_Func(){
	return_val(&Ki);
}

void set_Kd_Func(){
	return_val(&Kd);
}

void set_setpoint_Func(){
	return_val(&setpoint);
}