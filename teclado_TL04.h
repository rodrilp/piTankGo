/*
 * teclado_TL04.h
 *
 *  Created on: 19 de Feb. de 2019
 *      Author: FFM
 */

#ifndef _TECLADO_TL04_H_
#define _TECLADO_TL04_H_

#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <wiringPi.h>

#include "fsm.h"
#include "tmr.h"
#include "piTankGo_1.h"
#include "player.h"


#define GPIO_COL_1 0
#define GPIO_COL_2 1
#define GPIO_COL_3 2
#define GPIO_COL_4 3

#define GPIO_ROW_1 5
#define GPIO_ROW_2 6
#define GPIO_ROW_3 12
#define GPIO_ROW_4 13

//#define CLK_MS 			5
//#define	DEBOUNCE_TIME	200
#define COL_REFRESH_TIME 25

#define	FLAG_KEY			0

#define FLAG_KEY_PRESSED 0x01
#define FLAG_TMR_TIMEOUT 0x02

#define NUM_COLS 4
#define NUM_ROWS 4

enum columns_values {
	COL_1,
	COL_2,
	COL_3,
	COL_4,
};

enum rows_values {
	ROW_1,
	ROW_2,
	ROW_3,
	ROW_4
};

enum column_state {
	KEY_COL_1,
	KEY_COL_2,
	KEY_COL_3,
	KEY_COL_4
};

enum keypad_state {
	KEY_WAITING
};

typedef struct {
	int col;
	int row;
} TipoTecla;

typedef struct {
	int columna_actual;
	tmr_t* tmr_duracion_columna;
	TipoTecla teclaPulsada;
} TipoTeclado;

int CompruebaColumnTimeout (fsm_t* this);
int initialize(TipoTeclado *p_teclado);
int key_pressed (fsm_t* this);

void row_1_isr (void);
void row_2_isr (void);
void row_3_isr (void);
void row_4_isr (void);

void timer_duracion_columna_isr (union sigval value);

void col_1 (fsm_t* this);
void col_2 (fsm_t* this);
void col_3 (fsm_t* this);
void col_4 (fsm_t* this);

void process_key (fsm_t* this);
void delay_until (unsigned int next);
TipoTeclado teclado;


#endif /* _TECLADO_TL04_H_ */
