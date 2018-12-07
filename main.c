#include "./drivers/inc/vga.h"
#include "./drivers/inc/ISRs.h"
#include "./drivers/inc/LEDs.h"
#include "./drivers/inc/audio.h"
#include "./drivers/inc/HPS_TIM.h"
#include "./drivers/inc/int_setup.h"
#include "./drivers/inc/wavetable.h"
#include "./drivers/inc/pushbuttons.h"
#include "./drivers/inc/ps2_keyboard.h"
#include "./drivers/inc/HEX_displays.h"
#include "./drivers/inc/slider_switches.h"

int calculateSignal(float frequency, int t, int amplitude){

	int signal = 0;
	float indexCalc = frequency*t;
	int intIndexCalc = (int)indexCalc;
	float floatIndexCalc = indexCalc - intIndexCalc;	//remaining

	int index = intIndexCalc%48000;			//get the index		
			
	//check if the index is an entier or a floating point number
	if(floatIndexCalc != 0){
		if(index < 47999){
			signal = amplitude*((1-floatIndexCalc)*sine[index]+(floatIndexCalc)*sine[index+1]);
		}
		if(index == 47999){
			signal = amplitude*((1-floatIndexCalc)*sine[47999]+(floatIndexCalc)*sine[0]);
		}
	}
	//index is an int
	else{
		signal = amplitude*sine[index];
	}
	return signal;
}

int main() {
	/*
	int_setup(1, (int[]) {199});	

	HPS_TIM_config_t hps_tim;
	hps_tim.tim = TIM0;
	hps_tim.timeout = 20;
	hps_tim.LD_en = 1;
	hps_tim.INT_en = 1;
	hps_tim.enable = 1;
	
	HPS_TIM_config_ASM(&hps_tim);
	
	float frequency = 100;
	int amplitude = 100;

	int played = 0;
	int t = 0;	
	int signal;

	while(1) {
		
		//if the interruption happened
		if(hps_tim0_int_flag){
			hps_tim0_int_flag = 0;
			//calculate the signal
			signal = calculateSignal(frequency,t,amplitude);
			//play the wave 
			played = audio_write_data_ASM(signal, signal);
			//if the wave has played
			if(played == 1){
				t++;			//increase the time
				t = t%48000;	//loop if max value is reached
			}	
		}	
	}
*/

	int validity;
	char data;
	char *c;
	int played;	

	float fA = 130.813;
	float fS = 146.832;
	float fD = 164.814;
	float fF = 174.614;
	float fJ = 195.998;
	float fK = 220.000;
	float fL = 246.942;
	float fSemi = 261.626;
	
	int amplitude = 5;
	int t = 0;
	float signal = 0;
	
	int keyA = 0;
	int keyS = 0;
	int keyD = 0;
	int keyF = 0;
	int keyJ = 0;
	int keyK = 0;
	int keyL = 0;
	int keySemi = 0;
	int stopFlag = 0;

	int signalA = 0;
	int signalS = 0;
	int signalD = 0;
	int signalF = 0;
	int signalJ = 0;
	int signalK = 0;
	int signalL = 0;
	int signalSemi = 0;

	int_setup(1, (int[]) {199});	
	double previous[320] = { 0 };
	
	HPS_TIM_config_t hps_tim;
	hps_tim.tim = TIM0;
	hps_tim.timeout = 20;
	hps_tim.LD_en = 1;
	hps_tim.INT_en = 1;
	hps_tim.enable = 1;
	
	HPS_TIM_config_ASM(&hps_tim);
	
	while(1){

		c = &data;
		validity = read_ps2_data_ASM(c);

		//check the validity of the input
		if (validity){

			if(data == 0x1C)//A
			{
				if(stopFlag == 1){
					keyA = 0;
					stopFlag = 0;
				}
				else{
					keyA = 1;
					
				}
			}
			
			else if(data == 0x1B)//S
			{
				if(stopFlag == 1){
					keyS = 0;
					stopFlag = 0;
				}
				else{
					keyS = 1;
				}
			}	
			else if(data == 0x23)//D
			{
				if(stopFlag == 1){
					keyD = 0;
					stopFlag = 0;
				}
				else{
					keyD = 1;
				}	
			}
			else if(data == 0x2B)//F
			{
				if(stopFlag == 1){
					keyF = 0;
					stopFlag = 0;
				}
				else{
					keyF = 1;
				}	
			}
			else if(data == 0x3B)//J
			{
				if(stopFlag == 1){
					keyJ = 0;
					stopFlag = 0;
				}
				else{
					keyJ = 1;
				}	
			}
			else if(data == 0x42)//K
			{
				if(stopFlag == 1){
					keyK = 0;
					stopFlag = 0;
				}
				else{
					keyK = 1;
				}
			}
			else if(data == 0x4B)//L
			{
				if(stopFlag == 1){
					keyL = 0;
					stopFlag = 0;
				}
				else{
					keyL = 1;
				}
			}
			else if(data == 0x4C)//;
			{
				if(stopFlag == 1){
					keySemi = 0;
					stopFlag = 0;
				}
				else{
					keySemi = 1;
				}
			}
			//if a key is released
			if(data == 0xF0)
			{
				stopFlag = 1;
			}
			//if the user wants to increase the amplitude
			if(data == 0x4D){
				if(amplitude == 10){
				}
				else{
					amplitude = amplitude+1;
				}
			}	
			//if the user wants to decrease the amplitude
			if(data == 0x44){
				if(amplitude == 1){
				}
				else{
					amplitude = amplitude-1;
				}
			}	
		}
		if(keyA == 1){
			signalA = calculateSignal(fA,t,amplitude);
		}
		if(keyS == 1){
			signalS = calculateSignal(fS,t,amplitude);
		}
		if(keyD == 1){
			signalD = calculateSignal(fD,t,amplitude);
		}
		if(keyF == 1){
			signalF = calculateSignal(fF,t,amplitude);
		}
		if(keyJ == 1){
			signalJ = calculateSignal(fJ,t,amplitude);
		}
		if(keyK == 1){
			signalK = calculateSignal(fK,t,amplitude);
		}
		if(keyL == 1){
			signalL = calculateSignal(fL,t,amplitude);
		}
		if(keySemi == 1){
			signalSemi = calculateSignal(fSemi,t,amplitude);
		}
		signal = (keyA * signalA) + (keyS * signalS)
						 + (keyD * signalD) + (keyF * signalF) 
							+ (keyJ * signalJ) + (keyK * signalK)
								 + (keyL * signalL) + (keySemi * signalSemi);
		
		if(hps_tim0_int_flag){
			hps_tim0_int_flag = 0;
			played = audio_write_data_ASM(signal, signal);	
			if(played == 1){
				t++;			//increase the time
				if(t == 48000){
					t = 0;	//loop if max value is reached
				}
			}
		}

		int drawIndex = 0;
		double nextToDraw = 0;

			// To reduce the number of drawing operations

			if((t%10 == 0)){
				drawIndex = (t/10)%320;//x
				//clear 
				VGA_draw_point_ASM(drawIndex, previous[drawIndex], 0);
				nextToDraw = 120 + signal/2500000;//y
				//add to history
				previous[drawIndex] = nextToDraw;
				//draw 
				VGA_draw_point_ASM(drawIndex, nextToDraw, 900);		
			}
	}
	return 0;
}
