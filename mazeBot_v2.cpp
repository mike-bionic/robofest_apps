#include "Ultrasonic.h"

Ultrasonic midSonic(12,13);
Ultrasonic rightSonic(10,11);
Ultrasonic leftSonic(4,5);
int rightSonicDist;
int midSonicDist;
int leftSonicDist;
int distThresh = 5;
int frontDistThresh = 10;
int tama_yetdi = 0;
int sag_tarap_yapyk = 0;
int cep_tarap_yapyk = 0;

// motor
#define cep1 3
#define cep2 7
#define sag1 8
#define sag2 2
#define A 6
#define B 9

int sag_1_state = 0;
int sag_2_state = 0;
int cep_1_state = 0;
int cep_2_state = 0;
// motor

void setup(){
    Serial.begin(9600);

    pinMode (cep1, OUTPUT);
    pinMode (cep2, OUTPUT);
    pinMode (sag1, OUTPUT);
    pinMode (sag2, OUTPUT);
    pinMode (A, OUTPUT);
    pinMode (B, OUTPUT);
    analogWrite(A, 90);
    analogWrite(B, 90);
		tama_yetdi = 0;
    //digitalWrite(A,1);
    //digitalWrite(B,1);
}

void loop(){
    rightSonicDist = rightSonic.Ranging(CM);
    midSonicDist = midSonic.Ranging(CM);
    leftSonicDist = leftSonic.Ranging(CM);

		// Ozbashdak
		if (leftSonicDist < distThresh){
			cep_tarap_yapyk = 1;
		} else {
			cep_tarap_yapyk = 0;
		}
		if (rightSonicDist < distThresh){
			sag_tarap_yapyk = 1;
		} else {
			sag_tarap_yapyk = 0;
		}
		//

    if (midSonicDist < frontDistThresh){
			tama_yetdi = 1;
			//Serial.println("Taaaaaaaaaaaammmmmmmmmmmmmmmmmm");
			//Serial.print("sag_tarap_yapyk ________");
			//Serial.print(sag_tarap_yapyk);
			//Serial.print(" _____ cep_tarap_yapyk  ________  ");
			//Serial.println(cep_tarap_yapyk);
			if (sag_tarap_yapyk == 0 && cep_tarap_yapyk == 1){
				Serial.println("______________SAGA____________________");
				saga_owrum();
				motorRun();
			}
			else if (cep_tarap_yapyk == 0 && sag_tarap_yapyk == 1){
				Serial.println("______________CEPE____________________");
				cepe_owrum();
				motorRun();
			}
			else {
				stop();
			}
    }

		//while (!tama_yetdi){

		//}
		else if (midSonicDist > frontDistThresh){
			tama_yetdi = 0;
			if (sag_tarap_yapyk == 0 && cep_tarap_yapyk == 1){
				slow_saga_owrum();
			}
			else if (cep_tarap_yapyk == 0 && sag_tarap_yapyk == 1){
				slow_cepe_owrum();
			}
			else {
				one_yore();
			}
		}
		else {
			one_yore();
		}

    //Serial.print("____ Tama yetdimi : _____ ");
    //Serial.print(tama_yetdi);
    //Serial.print("____ cep_tarap : _____ ");
    //Serial.print(cep_tarap_yapyk);
    //Serial.print("____ sag_tarap : _____ ");
    //Serial.println(sag_tarap_yapyk);

    log();
    motorRun();
}


void motorRun(){
  digitalWrite(sag1, sag_1_state);
  digitalWrite(sag2, sag_2_state);
  digitalWrite(cep1, cep_1_state);
  digitalWrite(cep2, cep_2_state);
}

void one_yore(){
 sag_1_state = 1;
 sag_2_state = 0;
 cep_1_state = 0;
 cep_2_state = 1;
}

void yza_yore(){
 sag_1_state = 0;
 sag_2_state = 1;
 cep_1_state = 1;
 cep_2_state = 0;
}


void cepe_owrum(){
 sag_1_state = 0;
 sag_2_state = 1;
 cep_1_state = 0;
 cep_2_state = 1;
}


void saga_owrum(){
 sag_1_state = 1;
 sag_2_state = 0;
 cep_1_state = 1;
 cep_2_state = 0;
}

void slow_cepe_owrum(){
 sag_1_state = 0;
 sag_2_state = 0;
 cep_1_state = 0;
 cep_2_state = 1;
}


void slow_saga_owrum(){
 sag_1_state = 1;
 sag_2_state = 0;
 cep_1_state = 0;
 cep_2_state = 0;
}


void stop(){
 sag_1_state = 0;
 sag_2_state = 0;
 cep_1_state = 0;
 cep_2_state = 0;
}

void log(){
    Serial.print(rightSonicDist);
    Serial.print("   ");
    Serial.print(midSonicDist);
    Serial.print("   ");
    Serial.println(leftSonicDist);
}

