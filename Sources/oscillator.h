/*
 * oscillator.h
 *
 *  Created on: 06.11.2016
 *      Author: Manuel
 */

#ifndef SOURCES_OSCILLATOR_H_
#define SOURCES_OSCILLATOR_H_

//-- Macro for converting from degrees to radians
#define M_PI 3.14159

#ifndef DEG2RAD
  #define DEG2RAD(g) ((g)*M_PI)/180
#endif


struct Servos{
	//-- Oscillators parameters
	 int _A;  //-- Amplitude (degrees)
	 int _O;  //-- Offset (degrees)
	unsigned int _T;  //-- Period (miliseconds)
	double _phase0;   //-- Phase (radians)

	//-- Internal variables
	int _pos;         //-- Current servo pos
	int _trim;        //-- Calibration offset
	double _phase;    //-- Current phase
	double _inc;      //-- Increment of phase
	double _N;        //-- Number of samples
	unsigned int _TS; //-- sampling period (ms)

	long _previousMillis;
	long _currentMillis;

	//-- Oscillation mode. If true, the servo is stopped
	bool _stop;

	//-- Reverse mode
	bool _rev;
}servo;

extern struct Servos _servo[4];

void SetA( int , int );
void SetO( int , int );
void SetPh(double , int );
void attach(bool , int );
void detach(void);
void SetT(unsigned int , int );
bool next_sample(int );
void SetPosition(int , int );
void refresh(int );


#endif /* SOURCES_OSCILLATOR_H_ */
