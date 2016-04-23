///*
// * Author: Syed Tahmid Mahbub <sm893@cornell.edu>
// */
//



#include "GLOBAL_DEFINES.h"

#include "motor_drive.h"
#include "motor_encoder.h"
#include "pid_controller.h"
#include <signal.h>
#include <pthread.h>

volatile sig_atomic_t RUNNING = 1;
void keyboardinterrupt(int sig){
	RUNNING = 0;
}

void initialize_pid_struct(motor_pid_t* pid){
	pid->Kp = PTerm;
	pid->Ki = ITerm;
	pid->Kd = DTerm;
	pid->accumulator = 0;
	pid->dt = 1;
	pid->error = 0;
}

void* get_dur_mcu(void* args){

	int fid = open("/dev/ttymcu0", O_RDONLY);

	uint8_t buf[4];
	uint32_t duration;
	float RPM;

	while (1){
		read(fid, buf, 4);
		duration = buf[0] + (buf[1] << 8) + (buf[2] << 16) + (buf[3] << 24);
		RPM = 1000000.0 * 60.0 * (4.0/duration)/(CPR * gear_ratio);
		printf("duration = %u, rpm = %.1f\n", duration, RPM);
		sleep(1);
	}
	return 0;
}

int main()
{

	signal(SIGINT, keyboardinterrupt);

	TARGET_SPEED_L = 135;
	TARGET_SPEED_R = 135;

	float dutyL = 0.06;
	float dutyR = 0.06;
	float PID;
	motorLeft_direction(MOTOR_CW);
	motorRight_direction(MOTOR_CW);
	motorLeft_drive(dutyL);
	motorRight_drive(dutyR);
	sleep(1);
//	initEncoder();

	pthread_t mcu_comm;
	pthread_create(&mcu_comm, NULL, get_dur_mcu, NULL);

	motor_pid_t leftmotor, rightmotor;
	initialize_pid_struct(&leftmotor);
	initialize_pid_struct(&rightmotor);

	while(RUNNING){
		usleep(10000);

		PID = motor_pid(&leftmotor, TARGET_SPEED_L, getRPM_L());
//		dutyL = dutyL + PID/1000.0;
		dutyL = 0.33;
		motorLeft_drive(dutyL);

//		PID = motor_pid(&rightmotor, TARGET_SPEED_R, getRPM_R());
//		dutyR = dutyR + PID/1000.0;
//		motorRight_drive(dutyR);
	}

	motorLeft_brake();
	motorRight_brake();

	return MRAA_SUCCESS;
}

