///*
// * Author: Syed Tahmid Mahbub <sm893@cornell.edu>
// */
//



#include "GLOBAL_DEFINES.h"

#include "motor_drive.h"
#include "motor_encoder.h"
#include "pid_controller.h"

int main()
{

	TARGET_SPEED_L = 100;

	float duty = 0.06;
	float PID;
	motorLeft_direction(MOTOR_CW);
	motorLeft_drive(duty);
//	motorRight_direction(MOTOR_CW);
//	motorRight_drive(duty);
	sleep(1);
	initEncoder();

	//prev_duty, if PID is negative, subtract 5% off duty

	motor_pid_t leftmotor;
	leftmotor.Kp = PTerm;
	leftmotor.Ki = ITerm;
	leftmotor.Kd = DTerm;
	leftmotor.accumulator = 0;
	leftmotor.dt = 1;
	leftmotor.error = 0;

	while(1){
		usleep(10000);
		PID = motor_pid(&leftmotor, TARGET_SPEED_L, getRPM_L());
		duty = duty + PID/1000.0;
		if(duty <= 0) duty = 0.0;
		if(duty > 0.95) duty = 0.95;
		motorLeft_drive(duty);
	}

	motorLeft_brake();

	return MRAA_SUCCESS;
}

