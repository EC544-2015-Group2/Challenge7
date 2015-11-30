#include "SharpIR.h"

const int PIN_IR[] = {A0, A1};
#define model 20150

//Data Declaration
float IR_distance[2];

//Sensor Declaration
SharpIR sharpIR[] = {SharpIR(PIN_IR[0], model), SharpIR(PIN_IR[1], model)};


void setup(){


}

void loop() {

}

void IRdistance() {
	for(int i=0; i<2; i++) IR_distance[i] = sharpIR[i].distance();
}
