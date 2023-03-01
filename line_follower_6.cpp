
int left_most_Sensor = A4;
int leftSensor = A3;
int left_close_Sensor = 9;
int midSensor = A2;
int right_close_Sensor = 8;
int rightSensor = A1;
int right_most_Sensor = A0;

int rightSensor_state;
int midSensor_state;
int leftSensor_state;
int left_most_Sensor_state;
int right_most_Sensor_state;
int left_close_Sensor_state;
int right_close_Sensor_state;

int lastTurn = 1;
String thisTurn = "forward";

int super_slowSpeed = 60;
int slowSpeed = 100;
int midSpeed = 150;
int maxSpeed = 230;

// Motors
#define sag1 3
#define sag2 2
#define cep1 4
#define cep2 7
#define right_en 6
#define left_en 5

int sag1_state = 0;
int sag2_state = 0;
int cep1_state = 0;
int cep2_state = 0;
// Motors

void setup() {
    Serial.begin(9600);
    pinMode(sag1, OUTPUT);
    pinMode(sag2, OUTPUT);
    pinMode(cep1, OUTPUT);
    pinMode(cep2, OUTPUT);
    pinMode(right_en, OUTPUT);
    pinMode(left_en, OUTPUT);


    pinMode(left_most_Sensor, INPUT);
    pinMode(leftSensor, INPUT);
    pinMode(left_close_Sensor, INPUT);
    pinMode(midSensor, INPUT);
    pinMode(right_close_Sensor, INPUT);
    pinMode(rightSensor, INPUT);
    pinMode(right_most_Sensor, INPUT);

    analogWrite(right_en, slowSpeed);
    analogWrite(left_en, slowSpeed);
}

void loop(){
	updateColorValues();
	//calculate_and_run();
	calculate_and_run2();
	//forward();

	digitalWrite(sag1,sag1_state);
	digitalWrite(sag2,sag2_state);
	digitalWrite(cep1,cep1_state);
	digitalWrite(cep2,cep2_state);
}

void updateColorValues(){
    left_most_Sensor_state = !digitalRead(left_most_Sensor);
    leftSensor_state = !digitalRead(leftSensor);
    left_close_Sensor_state = digitalRead(left_close_Sensor);
    midSensor_state = !digitalRead(midSensor);
    right_close_Sensor_state = digitalRead(right_close_Sensor);
    rightSensor_state = !digitalRead(rightSensor);
    right_most_Sensor_state = !digitalRead(right_most_Sensor);

    Serial.print(left_most_Sensor_state);
    Serial.print("    -------    ");
    Serial.print(leftSensor_state);
    Serial.print("    -------    ");
    Serial.print(left_close_Sensor_state);
    Serial.print("    -------    ");
    Serial.print(midSensor_state);
    Serial.print("    -------    ");
    Serial.print(right_close_Sensor_state);
    Serial.print("    -------    ");
    Serial.print(rightSensor_state);
    Serial.print("    -------    ");
    Serial.println(right_most_Sensor_state);
}

// 0 0 0 0 1
// right()

// 0 0 0 1 1
// right_slow()

// 0 0 0 1 0
// right_slow() + mid_speed_left

// 0 0 1 0 0
// forward()

// 0 1 0 0 0
// left_slow() + mid_speed_right

// 1 1 0 0 0
// left_slow()

// 1 0 0 0 0
// left()
void calculate_and_run2(){
	if(left_close_Sensor_state || midSensor || right_close_Sensor_state){
		forward();
		if(left_close_Sensor_state){
			thisTurn = "forward_right";
			analogWrite(right_en, midSpeed);
			analogWrite(left_en, super_slowSpeed);
		}
		if(right_close_Sensor_state){
			thisTurn = "forward_left";
			analogWrite(right_en, super_slowSpeed);
			analogWrite(left_en, midSpeed);
		}
		else {
			thisTurn = "forward";
			analogWrite(right_en, midSpeed);
			analogWrite(left_en, midSpeed);
		}
	}

	if(
		leftSensor_state && 
		!midSensor_state &&
		!rightSensor_state
	){
		thisTurn = "right_slow";
		left_slow();
		analogWrite(right_en, slowSpeed);
		analogWrite(left_en, slowSpeed);
	}

	if(
		!leftSensor_state && 
		!midSensor_state &&
		rightSensor_state
	){
		thisTurn = "left_slow";
		right_slow();
		analogWrite(right_en, slowSpeed);
		analogWrite(left_en, slowSpeed);
	}



	if(
		leftSensor_state && 
		midSensor_state &&
		!rightSensor_state
	){
		thisTurn = "right";
		right();
		analogWrite(right_en, slowSpeed);
		analogWrite(left_en, slowSpeed);
	}

	if(
		!leftSensor_state && 
		midSensor_state &&
		rightSensor_state
	){
		thisTurn = "left";
		left();
		analogWrite(right_en, slowSpeed);
		analogWrite(left_en, slowSpeed);
	}

}

void calculate_and_run(){
    if(
        left_most_Sensor_state == 0 && 
        leftSensor_state == 0 && 
        midSensor_state == 0 &&
        rightSensor_state == 0 && 
        right_most_Sensor_state == 1){
			// Serial.println("+++++++++++++++++++++++   left");
            left();
            analogWrite(right_en, slowSpeed);
            analogWrite(left_en, slowSpeed);
    }
    
    if(
        left_most_Sensor_state == 0 && 
        leftSensor_state == 0 && 
        midSensor_state == 0 &&
        rightSensor_state == 1 && 
        right_most_Sensor_state == 1){
			// Serial.println("+++++++++++++++++++++++   left_slow");
            left_slow();
            analogWrite(right_en, slowSpeed);
            analogWrite(left_en, midSpeed);
    }
    
    if(
        left_most_Sensor_state == 0 && 
        leftSensor_state == 0 && 
        midSensor_state == 1 &&
        rightSensor_state == 1 && 
        (right_most_Sensor_state == 0 || right_most_Sensor_state == 1)){
			// Serial.println("+++++++++++++++++++++++   leftleft");
            left_slow();
            analogWrite(right_en, slowSpeed);
            analogWrite(left_en, slowSpeed);
    }
    
    if(
        left_most_Sensor_state == 0 && 
        leftSensor_state == 0 && 
        midSensor_state == 0 &&
        rightSensor_state == 1 && 
        right_most_Sensor_state == 0){
			// Serial.println("+++++++++++++++++++++++   left_slow + speed");
            forward();
            analogWrite(right_en, super_slowSpeed);
            analogWrite(left_en, slowSpeed);
    }

    if(
        left_most_Sensor_state == 0 && 
        leftSensor_state == 0 && 
        midSensor_state == 1 &&
        rightSensor_state == 0 && 
        right_most_Sensor_state == 0){
			// Serial.println("+++++++++++++++++++++++   forward");
            forward();
            analogWrite(right_en, midSpeed);
            analogWrite(left_en, midSpeed);
    }

     if(
        left_most_Sensor_state == 0 && 
        leftSensor_state == 1 && 
        midSensor_state == 0 &&
        rightSensor_state == 0 && 
        right_most_Sensor_state == 0){
			// Serial.println("+++++++++++++++++++++++   right_slow");
            forward();
            analogWrite(right_en, slowSpeed);
            analogWrite(left_en, super_slowSpeed);
    }

    if(
        (left_most_Sensor_state == 0 || left_most_Sensor_state == 1) && 
        leftSensor_state == 1 && 
        midSensor_state == 1 &&
        rightSensor_state == 0 && 
        right_most_Sensor_state == 0){
			// Serial.println("+++++++++++++++++++++++   rightright");
            right_slow();
            analogWrite(right_en, slowSpeed);
            analogWrite(left_en, slowSpeed);
    }

     if(
        left_most_Sensor_state == 1 && 
        leftSensor_state == 1 && 
        midSensor_state == 0 &&
        rightSensor_state == 0 && 
        right_most_Sensor_state == 0){
			// Serial.println("+++++++++++++++++++++++   right");
            right_slow();
            analogWrite(right_en, midSpeed);
            analogWrite(left_en, slowSpeed);
    }

     if(
        left_most_Sensor_state == 1 && 
        leftSensor_state == 0 && 
        midSensor_state == 0 &&
        rightSensor_state == 0 && 
        right_most_Sensor_state == 0){
			// Serial.println("+++++++++++++++++++++++   right");
            right();
            analogWrite(right_en, slowSpeed);
            analogWrite(left_en, slowSpeed);
    }



    if (
        left_most_Sensor_state == 0 && 
        leftSensor_state == 0 && 
        rightSensor_state == 0 &&
        right_most_Sensor_state == 0 &&  
        midSensor_state == 0){

        if (lastTurn == 2){
            right();
            analogWrite(right_en, slowSpeed);
            analogWrite(left_en, slowSpeed);
        }

        if (lastTurn == 0){
            left();
            analogWrite(right_en, slowSpeed);
            analogWrite(left_en, slowSpeed);
        }
    }
}

void forward(){
    sag1_state = 0;
    sag2_state = 1;
    cep1_state = 1;
    cep2_state = 0;
}

void right(){
    sag1_state = 1;
    sag2_state = 0;
    cep1_state = 1;
    cep2_state = 0;
}

void left(){
    sag1_state = 0;
    sag2_state = 1;
    cep1_state = 0;
    cep2_state = 1;
}
void right_slow(){
    sag1_state = 0;
    sag2_state = 0;
    cep1_state = 1;
    cep2_state = 0;
}

void left_slow(){
    sag1_state = 0;
    sag2_state = 1;
    cep1_state = 0;
    cep2_state = 0;
}