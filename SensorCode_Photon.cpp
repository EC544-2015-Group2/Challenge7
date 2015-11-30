#include <SharpIR.h>

#define ir1 A0
#define ir2 A1
#define model 20150

//Data Declaration
float IR_distance[2];

//Sensor Declaration
SharpIR1 SharpIR(ir, model);
SharpIR2 SharpIR(ir, model);


void setup() {

}

void loop() {

}

void IRdistance() {
	IR1_distance = SharpIR1.distance();
	IR2_distance = SharpIR2.distance();
}