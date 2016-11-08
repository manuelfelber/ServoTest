/*
 * Robo_Moves.c
 *
 *  Created on: 25.10.2016
 *      Author: Manuel
 */

#include "PE_Types.h"
#include "WAIT1.h"
#include "Robo.h"
#include "SERVO1.h"
#include "SERVO2.h"
#include "SERVO3.h"
#include "SERVO4.h"
#include "oscillator.h"
#include "TmDt1.h"
#include "Events.h"
#include "math.h"

void initRobo(void){
	Robo_initPos();
}

void Robo_initPos(void){
	SERVO1_MovePos(100,1); //Huft rechts
	SERVO2_MovePos(130,1); //Huft links
	SERVO3_MovePos(125,1);	//Fuss rechts
	SERVO4_MovePos(125,1);	//Fuss links
	while(SERVO1_IsMoving() & SERVO2_IsMoving() & SERVO3_IsMoving() & SERVO4_IsMoving()){}
}


void Robo_StepTime(int steps){

	for(int a=0; a<steps; a++){
		SERVO1_MovePos(170,500);
		SERVO2_MovePos(170,500);
		WAIT1_Waitms(2000);
		SERVO3_MovePos(80,420);
		SERVO4_MovePos(80,420);
		WAIT1_Waitms(2000);//430
		SERVO3_MovePos(180,900);
		SERVO4_MovePos(180,900);
		WAIT1_Waitms(2000);//80
		SERVO1_MovePos(80,500);
		SERVO2_MovePos(80,500);
		WAIT1_Waitms(2000);//830
	}
}

void Robo_StepForward(int steps){
	int i = 0;
	float m = 0.0;
	bool phase = false;

	for(int a=0; a<steps; a++){
		WAIT1_Waitms(5);
		m = (180.0-25.0);
		for(i = 80; i < 170; i++) {
			if(m>80.0 & ~phase){
				m = m-1.1111;
			}
			else{
				phase = true;
				m = m+1.1111;
			}
			WAIT1_Waitms(5);
			SERVO1_SetPos(i); 	//Huft rechts
			SERVO2_SetPos(i); 	//Huft links
			SERVO3_SetPos(m);	//Fuss rechts
			SERVO4_SetPos(m);	//Fuss links
		}
		WAIT1_Waitms(5);
		//m = 80.0;
		for(i = 170; i > 80; i--) {
			m = m+1.11;
			if(m<180.0 & phase){
				m = m+1.1111;
			}
			else{
				phase = false;
				m = m-1.1111;
			}
			WAIT1_Waitms(5);
			SERVO1_SetPos(i); 	//Huft rechts
			SERVO2_SetPos(i); 	//Huft links
			SERVO3_SetPos(m);	//Fuss rechts
			SERVO4_SetPos(m);	//Fuss links
		}
	}
}

void Robo_StepBack(int steps){
	int i = 0;
	float m = 0.0;
	bool phase = false;

	for(int a=0; a<steps; a++){
		WAIT1_Waitms(5);
		m = (180.0-25.0); //todo
	}
}


void walk(float steps, int T, int dir){

  //-- Oscillator parameters for walking
  //-- Hip sevos are in phase
  //-- Feet servos are in phase
  //-- Hip and feet are 90 degrees out of phase
  //--      -90 : Walk forward
  //--       90 : Walk backward
  //-- Feet servos also have the same offset (for tiptoe a little bit)
  int A[4]= {30, 30, 20, 20};
  int O[4] = {0, 0, 4, -4};
  double phase_diff[4] = {0, 0, DEG2RAD(dir * -90), DEG2RAD(dir * -90)};

  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Zowi gait: Turning (left or right)
//--  Parameters:
//--   * Steps: Number of steps
//--   * T: Period
//--   * Dir: Direction: LEFT / RIGHT
//---------------------------------------------------------
void turn(float steps, int T, int dir){

  //-- Same coordination than for walking (see Zowi::walk)
  //-- The Amplitudes of the hip's oscillators are not igual
  //-- When the right hip servo amplitude is higher, the steps taken by
  //--   the right leg are bigger than the left. So, the robot describes an
  //--   left arc
  int A[4]= {30, 30, 20, 20};
  int O[4] = {0, 0, 4, -4};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(-90)};

  if (dir == LEFT) {
    A[0] = 30; //-- Left hip servo
    A[1] = 10; //-- Right hip servo
  }
  else {
    A[0] = 10;
    A[1] = 30;
  }

  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Zowi gait: Shake a leg
//--  Parameters:
//--    steps: Number of shakes
//--    T: Period of one shake
//--    dir: RIGHT=Right leg LEFT=Left leg
//---------------------------------------------------------
void shakeLeg (int steps,int T,int dir){

  //This variable change the amount of shakes
  int numberLegMoves=2;

  //Parameters of all the movements. Default: Right leg
  int shake_leg1[4]={90, 90, 5, 35};//40,90,58,35
  int shake_leg2[4]={90, 90, 5, 120};//90,90,58,120
  int shake_leg3[4]={90, 90, 5, 60};//90,90,58,60
  int homes[4]={90, 90, 90, 90};

  //Changes in the parameters if left leg is chosen
  if(dir==-1)
  {
    shake_leg1[2]=180-35;
    shake_leg1[3]=180-58;
    shake_leg2[2]=180-120;
    shake_leg2[3]=180-58;
    shake_leg3[2]=180-60;
    shake_leg3[3]=180-58;
  }

  //Time of the bend movement. Fixed parameter to avoid falls
  int T2=1000;
  //Time of one shake, constrained in order to avoid movements too fast.
  T=T-T2;
  //T=max(T,200*numberLegMoves);
  if(T > 200*numberLegMoves){
	  T = T;
  }
  else{
	  T = 200*numberLegMoves;
  }

  for (int j=0; j<steps;j++)
  {
  //Bend movement
  moveServos(T2/2,shake_leg1);
  moveServos(T2/2,shake_leg2);

    //Shake movement
    for (int i=0;i<numberLegMoves;i++)
    {
    moveServos(T/(2*numberLegMoves),shake_leg3);
    moveServos(T/(2*numberLegMoves),shake_leg2);
    }
    moveServos(500,homes); //Return to home position
  }

  WAIT1_Waitms(T);
}

//---------------------------------------------------------
//-- Zowi movement: up & down
//--  Parameters:
//--    * steps: Number of jumps
//--    * T: Period
//--    * h: Jump height: SMALL / MEDIUM / BIG
//--              (or a number in degrees 0 - 90)
//---------------------------------------------------------
void updown(float steps, int T, int h){

  //-- Both feet are 180 degrees out of phase
  //-- Feet amplitude and offset are the same
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h, -h};
  double phase_diff[4] = {0, 0, DEG2RAD(-90), DEG2RAD(90)};

  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Zowi movement: swinging side to side
//--  Parameters:
//--     steps: Number of steps
//--     T : Period
//--     h : Amount of swing (from 0 to 50 aprox)
//---------------------------------------------------------
void swing(float steps, int T, int h){

  //-- Both feets are in phase. The offset is half the amplitude
  //-- It causes the robot to swing from side to side
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h/2, -h/2};
  double phase_diff[4] = {0, 0, DEG2RAD(0), DEG2RAD(0)};

  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Zowi movement: swinging side to side without touching the floor with the heel
//--  Parameters:
//--     steps: Number of steps
//--     T : Period
//--     h : Amount of swing (from 0 to 50 aprox)
//---------------------------------------------------------
void tiptoeSwing(float steps, int T, int h){

  //-- Both feets are in phase. The offset is not half the amplitude in order to tiptoe
  //-- It causes the robot to swing from side to side
  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h, -h};
  double phase_diff[4] = {0, 0, 0, 0};

  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);
}

//---------------------------------------------------------
//-- Zowi gait: Jitter
//--  Parameters:
//--    steps: Number of jitters
//--    T: Period of one jitter
//--    h: height (Values between 5 - 25)
//---------------------------------------------------------
void jitter(float steps, int T, int h){

  //-- Both feet are 180 degrees out of phase
  //-- Feet amplitude and offset are the same
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  //-- h is constrained to avoid hit the feets
  //h=min(25,h);
	 if(25 < h) h = 25;

  int A[4]= {h, h, 0, 0};
  int O[4] = {0, 0, 0, 0};
  double phase_diff[4] = {DEG2RAD(-90), DEG2RAD(90), 0, 0};

  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);
}


//---------------------------------------------------------
//-- Zowi gait: Ascending & turn (Jitter while up&down)
//--  Parameters:
//--    steps: Number of bends
//--    T: Period of one bend
//--    h: height (Values between 5 - 15)
//---------------------------------------------------------
void ascendingTurn(float steps, int T, int h){

  //-- Both feet and legs are 180 degrees out of phase
  //-- Initial phase for the right foot is -90, so that it starts
  //--   in one extreme position (not in the middle)
  //-- h is constrained to avoid hit the feets
  //h=min(13,h);
  if(13 < h) h = 13;

  int A[4]= {h, h, h, h};
  int O[4] = {0, 0, h+4, -h+4};
  double phase_diff[4] = {DEG2RAD(-90), DEG2RAD(90), DEG2RAD(-90), DEG2RAD(90)};

  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);
}


//---------------------------------------------------------
//-- Zowi gait: Moonwalker. Zowi moves like Michael Jackson
//--  Parameters:
//--    Steps: Number of steps
//--    T: Period
//--    h: Height. Typical valures between 15 and 40
//--    dir: Direction: LEFT / RIGHT
//---------------------------------------------------------
void moonwalker(float steps, int T, int h, int dir){

  //-- This motion is similar to that of the caterpillar robots: A travelling
  //-- wave moving from one side to another
  //-- The two Zowi's feet are equivalent to a minimal configuration. It is known
  //-- that 2 servos can move like a worm if they are 120 degrees out of phase
  //-- In the example of Zowi, the two feet are mirrored so that we have:
  //--    180 - 120 = 60 degrees. The actual phase difference given to the oscillators
  //--  is 60 degrees.
  //--  Both amplitudes are equal. The offset is half the amplitud plus a little bit of
  //-   offset so that the robot tiptoe lightly

  int A[4]= {0, 0, h, h};
  int O[4] = {0, 0, h/2+2, -h/2 -2};
  int phi = -dir * 90;
  double phase_diff[4] = {0, 0, DEG2RAD(phi), DEG2RAD(-60 * dir + phi)};

  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);
}


//----------------------------------------------------------
//-- Zowi gait: Crusaito. A mixture between moonwalker and walk
//--   Parameters:
//--     steps: Number of steps
//--     T: Period
//--     h: height (Values between 20 - 50)
//--     dir:  Direction: LEFT / RIGHT
//-----------------------------------------------------------
void crusaito(float steps, int T, int h, int dir){

  int A[4]= {25, 25, h, h};
  int O[4] = {0, 0, h/2+ 4, -h/2 - 4};
  double phase_diff[4] = {90, 90, DEG2RAD(0), DEG2RAD(-60 * dir)};

  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);
}


//---------------------------------------------------------
//-- Zowi gait: Flapping
//--  Parameters:
//--    steps: Number of steps
//--    T: Period
//--    h: height (Values between 10 - 30)
//--    dir: direction: FOREWARD, BACKWARD
//---------------------------------------------------------
void flapping(float steps, int T, int h, int dir){

  int A[4]= {12, 12, h, h};
  int O[4] = {0, 0, h - 10, -h + 10};
  double phase_diff[4] = {DEG2RAD(0), DEG2RAD(180), DEG2RAD(-90 * dir), DEG2RAD(90 * dir)};

  //-- Let's oscillate the servos!
  execute(A, O, T, phase_diff, steps);
}


///////////////////////////////////////////////////////////////////
//-- BASIC MOTION FUNCTIONS -------------------------------------//
///////////////////////////////////////////////////////////////////
void execute(int A[4], int O[4], int T, double phase_diff[4], float steps){

	for(int i = 0; i < 4; i++){
		attach(false, i);
	}

	int cycles=(int)steps;

	//-- Execute complete cycles
	if (cycles >= 1)
	for(int i = 0; i < cycles; i++)
		oscillateServos(A,O, T, phase_diff,1);

	//-- Execute the final not complete cycle
	oscillateServos(A,O, T, phase_diff,(float)steps-cycles);
}


void moveServos(int time, int  servo_target[]) {
  int servo_position[4];
  float increment[4];
  unsigned long final_time = 0;
  unsigned long partial_time = 0;

  for (int i = 0; i < 4; i++) servo_position[i] = 90;

  if(time>10){
    for (int i = 0; i < 4; i++) increment[i] = ((servo_target[i]) - servo_position[i]) / (time / 10.0);
    final_time =  cntr + time;

    for (int iteration = 1; cntr < final_time; iteration++) {
      partial_time = cntr + 10;
      float position = 0.0;
      for (int i = 0; i < 4; i++){
		position = (float)(servo_position[i] + (iteration * increment[i]))*1.4166666;
		switch(i){
			case 0: SERVO1_SetPos((byte)position);
			break;
		case 1: SERVO2_SetPos((byte)position);
			break;
			case 2: SERVO3_SetPos((byte)position);
		break;
			case 3: SERVO4_SetPos((byte)position);
			break;
		}
      }
      while (cntr < partial_time); //pause
    }
  }
  else{
    float position = 0.0;
    for (int i = 0; i < 4; i++){
		position = (float)(servo_target[i])*1.4166666;
		switch(i){
			case 0: SERVO1_SetPos((byte)position);
			break;
		case 1: SERVO2_SetPos((byte)position);
			break;
			case 2: SERVO3_SetPos((byte)position);
		break;
			case 3: SERVO4_SetPos((byte)position);
			break;
		}
    }
  }
  for (int i = 0; i < 4; i++) servo_position[i] = servo_target[i];
}



void oscillateServos(int A[4], int O[4], int T, double phase_diff[4], float cycle){
	for (int i=0; i<4; i++) {
		SetO(O[i], i);
		SetA(A[i], i);
		SetT(T, i);
		SetPh(phase_diff[i], i);
	}
	//-- Read current time
	double ref=cntr;
	for (double x=ref; x<=T*cycle+ref; x=cntr){
		for (int i=0; i<4; i++){
		refresh(i);
		}
	}
}
