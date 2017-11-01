/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Encoder_Driver_Two_Phase.cpp
 * Author: yasufumi
 * 
 * Created on 2017/10/12, 19:08
 */

#include "Encoder_Driver_Two_Phase.h"

#include <iostream>
#include <wiringPi.h>

using namespace std;

const double DEG2RAD = 3.1415 / 180.0;
const double RAD2DEG = 180.0 / 3.1415;

const int ENC_PULSE = 50;
const double PULSE2DEG_1 = 360.0 / (double)ENC_PULSE / 1.0;
const double PULSE2DEG_2 = 360.0 / (double)ENC_PULSE / 2.0;
const double PULSE2DEG_4 = 360.0 / (double)ENC_PULSE / 4.0;

const double PULSE2RAD_1 = PULSE2DEG_1 * DEG2RAD;
const double PULSE2RAD_2 = PULSE2DEG_2 * DEG2RAD;
const double PULSE2RAD_4 = PULSE2DEG_4 * DEG2RAD;

/*  */
Encoder_Driver_Two_Phase::Encoder_Driver_Two_Phase() {
	GPIO_Port_A = 16;
	GPIO_Port_B = 20;
	Init();
	pinMode(GPIO_Port_A, INPUT);
	pinMode(GPIO_Port_B, INPUT);
}

Encoder_Driver_Two_Phase::Encoder_Driver_Two_Phase(int gpio_A, int gpio_B){
	Init();
	Set_GPIO_PORTS(gpio_A, gpio_B);
}

Encoder_Driver_Two_Phase::Encoder_Driver_Two_Phase(const Encoder_Driver_Two_Phase& orig) {
}

Encoder_Driver_Two_Phase::~Encoder_Driver_Two_Phase() {
}

/*------*/
/* Init */
/*------*/
void Encoder_Driver_Two_Phase::Init(){
	CountMode = 1;
	Counter = 0;
	InitTrig = true;
}

/*----------------*/
/* Set GPIO Ports */
/*----------------*/
void Encoder_Driver_Two_Phase::Set_GPIO_PORTS(int gpio_A, int gpio_B){
	GPIO_Port_A = gpio_A;
	GPIO_Port_B = gpio_B;
	pinMode(GPIO_Port_A, INPUT);
	pinMode(GPIO_Port_B, INPUT);
}

/*----------------*/
/* Set Count Mode */
/*----------------*/
void Encoder_Driver_Two_Phase::Set_CountMode(int c_mode){
	Init();
	CountMode = c_mode;
}

/*-------------*/
/* Read Pulses */
/*-------------*/
void Encoder_Driver_Two_Phase::Read_Pulses(){
	Pulse_A = (bool)digitalRead(GPIO_Port_A);
	Pulse_B = (bool)digitalRead(GPIO_Port_B);
	
	if(InitTrig == true){
		InitTrig = false;
		Bef_Pulse_A = Pulse_A;
		Bef_Pulse_B = Pulse_B;
	}
	
	switch(CountMode){
		case(1):
			PulseCounter_Multiplication_1();
			break;
			
		case(2):
			PulseCounter_Multiplication_2();
			break;
			
		case(4):
			PulseCounter_Multiplication_4();
			break;
			
		default:
			PulseCounter_Multiplication_1();
			break;
	}
	
	Bef_Pulse_A = Pulse_A;
	Bef_Pulse_B = Pulse_B;
	
//	cout << "pulse: " << Pulse_A << " " << Pulse_B << endl;
}

/*-------------*/
/* Count Pulse */
/*-------------*/
void Encoder_Driver_Two_Phase::PulseCounter_Multiplication_1(){
	//up
	if(Bef_Pulse_A == false && Pulse_A == true && Pulse_B == false){
		Counter++;
	}
	//down
	else if(Bef_Pulse_A == true && Pulse_A == false && Pulse_B == false){
		Counter--;
	}
}

/*-------------*/
/* Count Pulse */
/*-------------*/
void Encoder_Driver_Two_Phase::PulseCounter_Multiplication_2(){
	//up
	if(Bef_Pulse_A == false && Pulse_A == true && Pulse_B == false){
		Counter++;
	}
	else if(Bef_Pulse_A == true && Pulse_A == false && Pulse_B == true){
		Counter++;
	}
	//down
	else if(Bef_Pulse_A == false && Pulse_A == true && Pulse_B == true){
		Counter--;
	}
	else if(Bef_Pulse_A == true && Pulse_A == false && Pulse_B == false){
		Counter--;
	}
}

/*-------------*/
/* Count Pulse */
/*-------------*/
void Encoder_Driver_Two_Phase::PulseCounter_Multiplication_4(){
	//up
	if(Bef_Pulse_A == false && Pulse_A == true && Pulse_B == false){
		Counter++;
	}
	else if(Bef_Pulse_A == true && Pulse_A == false && Pulse_B == true){
		Counter++;
	}
	else if(Bef_Pulse_B == false && Pulse_B == true && Pulse_A == true){
		Counter++;
	}
	else if(Bef_Pulse_B == true && Pulse_B == false && Pulse_A == false){
		Counter++;
	}
	//down
	else if(Bef_Pulse_A == false && Pulse_A == true && Pulse_B == true){
		Counter--;
	}
	else if(Bef_Pulse_A == true && Pulse_A == false && Pulse_B == false){
		Counter--;
	}
	else if(Bef_Pulse_B == true && Pulse_B == false && Pulse_A == true){
		Counter--;
	}
	else if(Bef_Pulse_B == false && Pulse_B == true && Pulse_A == false){
		Counter--;
	}
}

/*-----------*/
/* Get Angle */
/*-----------*/
double Encoder_Driver_Two_Phase::Get_Angle(){
	double ans;
	
	switch(CountMode){
		case(1):
			ans = (double)Counter * PULSE2RAD_1;
			break;
			
		case(2):
			ans = (double)Counter * PULSE2RAD_2;
			break;
			
		case(4):
			ans = (double)Counter * PULSE2RAD_4;
			break;
			
		default:
			ans = (double)Counter * PULSE2RAD_1;
			break;
	}
	
	return ans;
}




