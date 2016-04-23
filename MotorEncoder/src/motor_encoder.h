/*
 * motor_encoder.h
 *
 *  Created on: Nov 20, 2015
 *      Author: Claire
 */

#ifndef MOTOR_ENCODER_H_
#define MOTOR_ENCODER_H_

#define CPR 48.0
#define gear_ratio 20.4

int initEncoder();
float getusecPerCount_L();
float getusecPerCount_R();

float RPM_L;
float RPM_R;
float usecPerCount_L;
float usecPerCount_R;

float getRPM_L(void);
float getRPM_R(void);

#endif /* MOTOR_ENCODER_H_ */
