#include <Servo.h>
#include "Ultrasonic.h"

Servo clawServo;
Servo frontServo;
Servo backServo;
Servo baseServo;

Ultrasonic outSonic(12,13);
int outSonicDist;
Ultrasonic inSonic(10,11);
int inSonicDist;
int inSonicThresh = 4;
int outSonicThresh = 12;
int box_is_out = 0;
int continue_after_value_save = 0;
int prepare_after_box_done = 0;
int box_released_mode = 0;

int inIrSensor = A0;
int outIrSensor = A1;
int inIrSensor_value;
int outIrSensor_value;
int inIrSensor_value_saved;
int outIrSensor_value_saved;

int boxChannel_closedVal = 60;
int boxChannel_openedVal = 10;
int boxGate_closedVal = 180;
int boxGate_openedVal = 60;
int out_box_detected = 0;
long out_box_millis;

Servo boxChannel;
Servo boxGate;

int conveyMotor = 8;
int conveyMotorState = 0;


int boxDetected = 0;

void setup(){
    Serial.begin(9600);
    clawServo.attach(2);
    frontServo.attach(3);
    backServo.attach(4);
    baseServo.attach(5);
    boxChannel.attach(6);
    boxGate.attach(7);

    pinMode(conveyMotor, OUTPUT);
    prepare_box_servo();
}


void readIrSensors(){
    inIrSensor_value = digitalRead(inIrSensor);
    outIrSensor_value = digitalRead(outIrSensor);
}

void inSonicHandler(){
	if (boxDetected == 0){
		if (inSonicDist < inSonicThresh){
            inIrSensor_value_saved = inIrSensor_value;
            boxDetected = 1;
		}
	}
    //Serial.print("Box detected_ _____ ");
    //Serial.println(boxDetected);
}


void readUltrasonics(){
    outSonicDist = outSonic.Ranging(CM);
    inSonicDist = inSonic.Ranging(CM);

    //Serial.print("ConveeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeeYYY ___ ");
    //Serial.println(conveyMotorState);
}


void loop(){
    readUltrasonics();
    readIrSensors();

    if (boxDetected == 1){
        if (outSonicDist < outSonicThresh){
            out_box_detected = 1;
            out_box_millis = millis();
            stop_conveyer();
        }
    }
    if (out_box_detected){
        stop_conveyer();
        Serial.println(out_box_millis);
        if (outSonicDist < outSonicThresh){
            out_box_millis = millis();
        }

        if (out_box_millis + 2000 < millis()){
            if (!(outSonicDist < outSonicThresh)){
                out_box_detected = 0;
                boxDetected = 0;
            }
        }
    }		

    if (boxDetected == 0){
        if (!(inSonicDist < inSonicThresh)){
            stop_conveyer();
            closeGate();
            openChannel();
        }
        inSonicHandler();
    }
    else if (boxDetected == 1 && !out_box_detected){
        openGate();
        closeChannel();
        start_conveyer();
    }

    digitalWrite(conveyMotor, conveyMotorState);
}

void prepare_box_servo(){
    boxChannel.write(boxChannel_openedVal);
    boxGate.write(boxGate_closedVal);
}

void closeGate(){
	boxGate.write(boxGate_closedVal);
}
void openGate(){
	boxGate.write(boxGate_openedVal);
}
void closeChannel(){
	boxChannel.write(boxChannel_closedVal);
}
void openChannel(){
	boxChannel.write(boxChannel_openedVal);
}

void release_box_servo(){
    boxChannel.write(boxChannel_closedVal);
    boxGate.write(boxGate_openedVal);
}

void box_in_ready_handler(){
    if (inSonicDist < inSonicThresh){
        boxChannel.write(boxChannel_closedVal);
    }
    else {
        if (box_released_mode == 1){
            start_conveyer();
        }
    }
}

void korobka_suysh(){
    baseServo.write(90);
    delay(80);
    delay(500);
    frontServo.write(140);
    delay(80);
    backServo.write(150);
    delay(80);
}

void korobka_gys(){
    clawServo.write(45);
    delay(80);
}

void korobka_goyber(){
    clawServo.write(180);
    delay(80);  
}

void yokary_gal(){
    frontServo.write(150);
    delay(80);
    backServo.write(80);
    delay(80);  
}

void sag_owrum(){
  baseServo.write(160);
  delay(80);
}

void cep_owrum(){
  baseServo.write(20);
  delay(80);
}

void stop_conveyer(){
    conveyMotorState = 0;
}

void start_conveyer(){
    conveyMotorState = 1;
}