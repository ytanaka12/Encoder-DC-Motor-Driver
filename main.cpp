/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: yasufumi
 *
 * Created on 2017/09/07, 18:58
 */

#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <math.h>

#include <wiringPi.h>
#include "ServoBlaster.h"
#include "Encoder_Driver_Two_Phase.h"

using namespace std;

const double DEG2RAD = 3.1415 / 180.0;
const double RAD2DEG = 180.0 / 3.1415;

const int BUTTON_INPUT_PORT = 21;

const int ENCORDER_A_PORT = 16;
const int ENCORDER_B_PORT = 20;

/*
 * 
 */
int main(int argc, char** argv) {
	
	cout << "hello world" << endl;
	
	if(wiringPiSetupGpio() == -1){
		return 1;
	}
	
	pinMode(BUTTON_INPUT_PORT, INPUT);
	
	Encoder_Driver_Two_Phase enc(ENCORDER_A_PORT, ENCORDER_B_PORT);
	enc.Set_CountMode(4);
	
	nsServoBlaster::ServoBlaster sb;
	
	int tick = 0;

	//unlock
	sb.SendPulseWidth_us(0, 0);
	sb.SendPulseWidth_us(1, 0);
	sb.SendPulseWidth_us(2, 0);
	usleep(10000);
	
	while(true){
		if(digitalRead(BUTTON_INPUT_PORT) == 1){
			break;
		}

		enc.Read_Pulses();
		double angle = enc.Get_Angle();
		
		//int pwm = 500 * cos((double)tick / 1000.0) + 1500.0;
		int pwm = 2000;
		
		double pGain = 500.0;
		double dGain = 1.0;
		double target = -270.0 * DEG2RAD;
		double output = pGain * (target - angle);
		
		if(0.0 < output){
			output += 1000.0;
			if(3000.0 < output){
				output = 3000.0;
			}
			sb.SendPulseWidth_us(0, 0);
			sb.SendPulseWidth_us(1, 0);
			sb.SendPulseWidth_us(2, (int)output);
		}else{
			output = - output;
			output += 1000.0;
			if(3000.0 < output){
				output = 3000.0;
			}
			sb.SendPulseWidth_us(0, 0);
			sb.SendPulseWidth_us(1, (int)output);
			sb.SendPulseWidth_us(2, 0);
		}

		if(tick%1 == 0){
			cout << "tick = " << tick;
			cout << " / pwm = " << output;
			cout << " angle = " << angle * RAD2DEG / 3.0;
			cout << endl;
		}

		tick++;

		usleep(1000);
	}

	return 0;
}

