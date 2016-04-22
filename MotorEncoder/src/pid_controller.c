
#include "pid_controller.h"

float motor_pid(pid_t* pid, int target_speed, int motor_speed){
	float preverror = pid->error;
	pid->error = target_speed - motor_speed;
	pid->accumulator += error;

	// check for zero crossing
	if ((pid->error > 0 && preverror < 0) || (pid->error < 0 && preverror > 0))
		pid->accumulator = 0;

	// check for integral wind up
	if (pid->accumulator > PID_MAX) pid->accumulator = PID_MAX;
	if (pid->accumulator < PID_MIN) pid->accumulator = PID_MIN;

	float p,i,d, c;
	p = pid->Kp * pid->error;
	i = pid->Ki * pid->accumulator;
	d = pid->Kd * (pid->error - preverror)/pid->dt;
	c = p+i+d;

	// bound pid
	if (c > PID_MAX) c = PID_MAX;
	if (c < PID_MIN) c = PID_MIN;

	return c;
}
