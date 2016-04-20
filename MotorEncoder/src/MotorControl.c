///*
// * Author: Syed Tahmid Mahbub <sm893@cornell.edu>
// */
//



#include "GLOBAL_DEFINES.h"

#include "motor_drive.h"
#include "motor_encoder.h"
#include <signal.h>
/*
 */
volatile sig_atomic_t RUNNING = 1;
void keyboardinterrupt(int sig){
	RUNNING = 0;
}
int main()
{
	signal(SIGTERM, keyboardinterrupt);

	TARGET_SPEED_L = 100;

	float RPM;
	float usec;
	float duty = 0.06;
	float prev_duty;
	float PID;
	motorLeft_direction(MOTOR_CW);
	motorLeft_drive(duty);
	motorRight_direction(MOTOR_CW);
	motorRight_drive(duty);
	sleep(1);
	initEncoder();
	int i=0;

	//prev_duty, if PID is negative, subtract 5% off duty

	while(RUNNING){
		usleep(20000);
		PID = motorLeft_PID();
		duty = PID/1000.0;
		if(duty <=0) duty =0.01;
		if(duty >0.95) duty =0.95;
		motorLeft_drive(duty);
		motorRight_drive(duty);

		if(++i ==1) {
			RPM = getRPM_L();
			usec = getusecPerCount_L();
			fprintf(stdout, "PID: %.1f   RPM: %.1f   duty: %.3f   usec: %.1f\n", (float)PID, (float)RPM, duty, usec);
			i=0;
		}
	}

	motorLeft_brake();
	motorRight_brake();

	return MRAA_SUCCESS;
}

