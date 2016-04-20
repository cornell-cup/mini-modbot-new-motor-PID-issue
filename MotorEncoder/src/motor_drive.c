/*
 * motor_drive.c
 *
 *  Created on: Nov 4, 2015
 *      Author: Cornell Cup
 */

#include "GLOBAL_DEFINES.h"
#include "motor_drive.h"
#include "motor_encoder.h"

#ifndef NULL
#define NULL   ((void *) 0)
#endif

mraa_pwm_context PWM_MOTOR_LEFT = NULL;
mraa_gpio_context DIRECTION_MOTOR_LEFT = NULL;
mraa_gpio_context BRAKE_MOTOR_LEFT = NULL;
int pwm_pin;

mraa_pwm_context PWM_MOTOR_RIGHT = NULL;
mraa_gpio_context DIRECTION_MOTOR_RIGHT = NULL;
mraa_gpio_context BRAKE_MOTOR_RIGHT = NULL;
int pwm_pin2;

int oldDirection_left = -1;
int oldDirection_right = -1;


int motorLeft_drive(float duty){
	/* Set speed between minimum and maximum (set in motor_drive.h)
	 * These are in float -> eg 0.3 = 30% duty
	 *
	 */
	mraa_result_t result;

	if (duty > MAX_DUTY){
		duty = MAX_DUTY;
	}

	if (duty < MIN_DUTY){
		duty = MIN_DUTY;
	}

	result = mraa_pwm_write(PWM_MOTOR_LEFT, duty);

	if (result == MRAA_SUCCESS){
		return 0;
	}
	else{
		return -1;
	}


}

int motorRight_drive(float duty){
	/* Set speed between minimum and maximum (set in motor_drive.h)
	 * These are in float -> eg 0.3 = 30% duty
	 *
	 */
	mraa_result_t result;

	if (duty > MAX_DUTY){
		duty = MAX_DUTY;
	}

	if (duty < MIN_DUTY){
		duty = MIN_DUTY;
	}

	result = mraa_pwm_write(PWM_MOTOR_RIGHT, duty);

	if (result == MRAA_SUCCESS){
		return 0;
	}
	else{
		return -1;
	}
}

int motorLeft_direction(int direction){
	/* Configure motor driver for use with specified direction
	 * direction = either MOTOR_CW or MOTOR_CCW
	 * return value = 0 if direction setting was successful
	 * 				= -1 if it failed
	 */

	/* MOTOR+ = OUT1, MOTOR- = OUT2
	 * CCW OPERATION:
	 * 		CONTROL SIGNALS:
	 * 			OUT1 = VS, OUT2 = GND
	 * 			IN1 = 1, IN2 = 0
	 * 		PWM = IN1
	 * 		DIRECTION = IN2 = 0
	 * 	CW OPERATION:
	 * 		CONTROL SIGNALS:
	 * 			OUT1 = GND, OUT2 = VS
	 * 			IN1 = 0, IN2 = 1
	 *		PWM = IN2
	 *		DIRECTION = IN1 = 0
	 *
	 */

	if (direction == oldDirection_left){
		// direction already set, don't do anything
		return 0;
	}

	oldDirection_left = direction;

	// if pwm channel was initialized for a direction, close it to enable it to be
	// initialized to a different pin; similar for direction gpio pin
	if (PWM_MOTOR_LEFT != NULL){
		mraa_pwm_close(PWM_MOTOR_LEFT);
		PWM_MOTOR_LEFT = NULL;
	}

	if (DIRECTION_MOTOR_LEFT != NULL){
		mraa_gpio_close(DIRECTION_MOTOR_LEFT);
		DIRECTION_MOTOR_LEFT = NULL;
	}

	if (direction == MOTOR_CCW){
		PWM_MOTOR_LEFT = mraa_pwm_init(MOTOR_LEFT_1_PIN);
		DIRECTION_MOTOR_LEFT = mraa_gpio_init(MOTOR_LEFT_2_PIN);
		pwm_pin = MOTOR_LEFT_1_PIN;
	}
	else if (direction == MOTOR_CW){
		PWM_MOTOR_LEFT = mraa_pwm_init(MOTOR_LEFT_2_PIN);
		DIRECTION_MOTOR_LEFT = mraa_gpio_init(MOTOR_LEFT_1_PIN);
		pwm_pin = MOTOR_LEFT_2_PIN;
	}

	if (PWM_MOTOR_LEFT == NULL || DIRECTION_MOTOR_LEFT == NULL){
		fprintf(stderr, "Could not initialize left motor, MRAA error!\n");
		return -1;
	}

	// configure pwm settings
	mraa_pwm_period_us(PWM_MOTOR_LEFT, PERIOD_US);
	mraa_pwm_enable(PWM_MOTOR_LEFT, 1);

	// this sets pwm to work in correct direction
	mraa_gpio_dir(DIRECTION_MOTOR_LEFT, MRAA_GPIO_OUT);
	mraa_gpio_write(DIRECTION_MOTOR_LEFT, 0);

	return 0;

}

int motorRight_direction(int direction){
	/* Configure motor driver for use with specified direction
	 * direction = either MOTOR_CW or MOTOR_CCW
	 * return value = 0 if direction setting was successful
	 * 				= -1 if it failed
	 */

	/* MOTOR+ = OUT1, MOTOR- = OUT2
	 * CCW OPERATION:
	 * 		CONTROL SIGNALS:
	 * 			OUT1 = VS, OUT2 = GND
	 * 			IN1 = 1, IN2 = 0
	 * 		PWM = IN1
	 * 		DIRECTION = IN2 = 0
	 * 	CW OPERATION:
	 * 		CONTROL SIGNALS:
	 * 			OUT1 = GND, OUT2 = VS
	 * 			IN1 = 0, IN2 = 1
	 *		PWM = IN2
	 *		DIRECTION = IN1 = 0
	 *
	 */

	if (direction == oldDirection_right){
		// direction already set, don't do anything
		return 0;
	}

	oldDirection_right = direction;

	// if pwm channel was initialized for a direction, close it to enable it to be
	// initialized to a different pin; similar for direction gpio pin
	if (PWM_MOTOR_RIGHT != NULL){
		mraa_pwm_close(PWM_MOTOR_RIGHT);
		PWM_MOTOR_RIGHT = NULL;
	}

	if (DIRECTION_MOTOR_RIGHT != NULL){
		mraa_gpio_close(DIRECTION_MOTOR_RIGHT);
		DIRECTION_MOTOR_RIGHT = NULL;
	}

	if (direction == MOTOR_CCW){
		PWM_MOTOR_RIGHT = mraa_pwm_init(MOTOR_RIGHT_1_PIN);
		DIRECTION_MOTOR_RIGHT = mraa_gpio_init(MOTOR_RIGHT_2_PIN);
		pwm_pin2 = MOTOR_RIGHT_1_PIN;
	}
	else if (direction == MOTOR_CW){
		PWM_MOTOR_RIGHT = mraa_pwm_init(MOTOR_RIGHT_2_PIN);
		DIRECTION_MOTOR_RIGHT = mraa_gpio_init(MOTOR_RIGHT_1_PIN);
		pwm_pin2 = MOTOR_RIGHT_2_PIN;
	}

	if (PWM_MOTOR_RIGHT == NULL || DIRECTION_MOTOR_RIGHT == NULL){
		fprintf(stderr, "Could not initialize right motor, MRAA error!\n");
		return -1;
	}

	// configure pwm settings
	mraa_pwm_period_us(PWM_MOTOR_RIGHT, PERIOD_US);
	mraa_pwm_enable(PWM_MOTOR_RIGHT, 1);

	// this sets pwm to work in correct direction
	mraa_gpio_dir(DIRECTION_MOTOR_RIGHT, MRAA_GPIO_OUT);
	mraa_gpio_write(DIRECTION_MOTOR_RIGHT, 0);

	return 0;

}

int motorLeft_brake(void){
	// Free PWM so that pin can be used as GPIO
	if (PWM_MOTOR_LEFT != NULL){
		mraa_pwm_close(PWM_MOTOR_LEFT);
		PWM_MOTOR_LEFT = NULL;
	}

	oldDirection_left = -1;

	// Electrical brake when:
	// IN1 = 0, IN2 = 0,
	// or
	// IN1 = 1, IN2 = 1
	// Since direction pin is always set to 0 in motorLeft_direction(),
	// set brake pin to also be 0

	BRAKE_MOTOR_LEFT = mraa_gpio_init(pwm_pin);
	if (BRAKE_MOTOR_LEFT == NULL){
		return -1;
	}

	mraa_gpio_write(BRAKE_MOTOR_LEFT, 0);
	mraa_gpio_close(BRAKE_MOTOR_LEFT);

	return 0;
}

int motorRight_brake(void){
	// Free PWM so that pin can be used as GPIO
	if (PWM_MOTOR_RIGHT != NULL){
		mraa_pwm_close(PWM_MOTOR_RIGHT);
		PWM_MOTOR_RIGHT = NULL;
	}

	oldDirection_right = -1;

	// Electrical brake when:
	// IN1 = 0, IN2 = 0,
	// or
	// IN1 = 1, IN2 = 1
	// Since direction pin is always set to 0 in motorLeft_direction(),
	// set brake pin to also be 0

	BRAKE_MOTOR_RIGHT = mraa_gpio_init(pwm_pin2);
	if (BRAKE_MOTOR_RIGHT == NULL){
		return -1;
	}

	mraa_gpio_write(BRAKE_MOTOR_RIGHT, 0);
	mraa_gpio_close(BRAKE_MOTOR_RIGHT);

	return 0;
}

/*
 * Calculates PID error term for left motor
 */
float motorLeft_PID(void){
	// Set constants here
	static float Accumulator = 0;
	float PID = 0;
	static float error = 0;
	float prevError = 0;

	// Calculate the PID
	prevError = error;
	error = getRPM_L() - TARGET_SPEED_L;

	if((error < 0 && prevError > 0) || (error > 0 && prevError <0)){
		Accumulator = 0;
	}

	if(ITerm*Accumulator < 1000){
		Accumulator += error;  // accumulator is sum of errors
	}
	PID = -error*PTerm;     // start with proportional gain
	PID += ITerm*Accumulator; // add integral gain and error accumulation
	PID += DTerm*(error-prevError); // differential gain comes next

	return PID;
}

/*
 * Calculates PID error term for right motor
 */
float motorRight_PID(void){
	// Set constants here
	static float Accumulator = 0;
	float PID = 0;
	static float error = 0;
	float prevError = 0;

	// Calculate the PID
	prevError = error;
	error = getRPM_R() - TARGET_SPEED_R;

	if((error < 0 && prevError > 0) || (error > 0 && prevError <0)){
		Accumulator = 0;
	}

	PID = -error*PTerm;     // start with proportional gain
	Accumulator += error;  // accumulator is sum of errors
	PID += ITerm*Accumulator; // add integral gain and error accumulation
	PID += DTerm*(error-prevError); // differential gain comes next
	return PID;
}




