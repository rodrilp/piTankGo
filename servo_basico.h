/*
 * servo_basico.h
 *
 *  Created on: 11 de enero de 2019
 *      Author: FFM
 */

#ifndef _SERVO_BASICO_H_
#define _SERVO_BASICO_H_

#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <softPwm.h>

#include "fsm.h"
#include "kbhit.h" // Para poder detectar teclas pulsadas sin bloqueo y leer las teclas pulsadas

#define CLK_MS 				1

#define FLAG_SERVO_LEFT		0x01
#define FLAG_SERVO_RIGHT	0x02

//#define	SERVO_PIN			13
#define	SERVO_PIN			18

// https://github.com/WiringPi/WiringPi/blob/master/wiringPi/softPwm.c
//  The PWM Frequency is derived from the "pulse time" below. Essentially,
//	the frequency is a function of the range and this pulse time.
//	The total period will be range * pulse time in µS, so a pulse time
//	of 100 and a range of 100 gives a period of 100 * 100 = 10,000 µS
//	which is a frequency of 100Hz.

// PWM SW: el SO provoca que cuanto más alta sea la frecuencia de la señal PWM de control menos estable resulta esta (i.e. cimbreo del servo)
// Interesa una frecuencia para la señal de control relativamente baja para minimizar el cimbreo
// El problema es que tampoco puede ser demasiado baja o de lo contrario el control del servo se resiente y su respuesta es peor
// (más lenta ante variaciones rápidas del ciclo de trabajo de la señal de control)
#define SERVO_PWM_RANGE		400 // 100 * 400 = 40,000 µS = 40 ms // 25 Hz

#define SERVO_INCREMENTO	1
#define SERVO_MINIMO		9
#define SERVO_MAXIMO		22

// Posibles estados de las FSMs
enum fsm_state {
	WAIT_KEY
};

typedef struct {
	int inicio; // Valor correspondiente a la posicion inicial del servo
	int incremento; // Cuantía en que se incrementa el valor de la posicion con cada movimiento del servo
	int minimo; // Valor mínimo correspondiente a la posicion del servo
	int maximo; // Valor maximo correspondiente a la posicion del servo
	int posicion; // Posicion actual del servo
} TipoServo;

//------------------------------------------------------
// FUNCIONES DE CONFIGURACION/INICIALIZACION
//------------------------------------------------------
int InicializaServo (TipoServo *p_servo);

int CompruebaIzquierda (fsm_t* this);
int CompruebaDerecha (fsm_t* this);
void MueveServoIzquierda (fsm_t* this);
void MueveServoDerecha (fsm_t* this);

#endif /* _SERVO_BASICO_H_ */
