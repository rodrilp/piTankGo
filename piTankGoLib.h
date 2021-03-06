
#ifndef _PITANKGOLIB_H_
#define _PITANKGOLIB_H_

#include <stdio.h>

#include "fsm.h"
#include "tmr.h"

#define CLK_MS 10

// ATENCION: Valores a modificar por el alumno
// INTERVALO DE GUARDA ANTI-REBOTES
#define	DEBOUNCE_TIME	500
// DURACION DISPARO IR
#define SHOOTING_PERIOD 20000

// CLAVES PARA MUTEX
// ATENCION: Valores a modificar por el alumno
#define	SYSTEM_FLAGS_KEY	1
#define	PLAYER_FLAGS_KEY	2
#define	STD_IO_BUFFER_KEY	3

// Distribucion de pines GPIO empleada para el enlace IR
// ATENCION: Valores a modificar por el alumno
#define	IR_TX_PIN		24 // PIN 20 salidas
#define	IR_RX_PIN		20 // PIN 14 entradas

// Distribucion de pines GPIO empleada para la reproducción de efectos
// ATENCION: Valores a modificar por el alumno
#define PLAYER_PWM_PIN 	23 //pin 19 salidas

// Distribucion de pines GPIO empleada para el control de los servos
// ATENCION: Valores a modificar por el alumno
#define	SERVO_VERTICAL_PIN		18 // pin 17 salidas
#define	SERVO_HORIZONTAL_PIN	22 // pin 18 salidas

// FLAGS FSM CONTROL DE JUEGO Y TORRETA
// ATENCION: Valores a modificar por el alumno
#define FLAG_SYSTEM_START 		0x01
#define FLAG_JOYSTICK_UP 		0x02
#define FLAG_JOYSTICK_DOWN 		0x04
#define FLAG_JOYSTICK_LEFT		0x08
#define FLAG_JOYSTICK_RIGHT		0x10
#define FLAG_TRIGGER_BUTTON		0x20
#define FLAG_SHOOT_TIMEOUT		0x40
#define FLAG_TARGET_DONE		0x80
#define FLAG_SYSTEM_END			0x100

// FLAGS FSM REPRODUCCION DE EFECTOS DE SONIDO
// ATENCION: Valores a modificar por el alumno
#define FLAG_START_DISPARO 		0x01
#define FLAG_START_IMPACTO 		0x02
#define FLAG_PLAYER_END			0x04
#define FLAG_NOTA_TIMEOUT		0x08

enum interruption_sources {
	TARGET_ISR,
	TECLADO_FILA_1,
	TECLADO_FILA_2,
	TECLADO_FILA_3,
	TECLADO_FILA_4,
};

extern int flags_system;
extern int flags_player;

extern int frecuenciaDespacito[];
extern int tiempoDespacito[];
extern int frecuenciaGOT[];
extern int tiempoGOT[];
extern int frecuenciaTetris[];
extern int tiempoTetris[];
extern int frecuenciaStarwars[];
extern int tiempoStarwars[];

extern int frecuenciasDisparo[];
extern int tiemposDisparo[];
extern int frecuenciasImpacto[];
extern int tiemposImpacto[];

#endif /* _PITANKGOLIB_H_ */
