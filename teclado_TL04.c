#include "teclado_TL04.h"
#include "piTankGo_1.h"
#include "piTankGoLib.h"


TipoTeclado teclado;
int flags = 0;
int debounceTime[NUM_ROWS] = {0,0,0,0}; // Timeout to avoid bouncing after pin event

char tecladoTL04[4][4] = {
	{'1', '2', '3', 'C'},
	{'4', '5', '6', 'D'},
	{'7', '8', '9', 'E'},
	{'A', '0', 'B', 'F'}
};

int CompruebaColumnTimeout (fsm_t* this) {
	int result = 0;
	piLock (FLAG_KEY);
	result = (flags & FLAG_TMR_TIMEOUT);
	piUnlock (FLAG_KEY);
	return result;
}

void row_1_isr (void) {
	// Pin event (key / button event) debouncing procedure
	if (millis () < debounceTime[ROW_1]) {
		debounceTime[ROW_1] = millis () + DEBOUNCE_TIME ;
		return;
	}

	piLock (FLAG_KEY);

	teclado.teclaPulsada.row = ROW_1;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags |= FLAG_KEY_PRESSED;

	piUnlock (FLAG_KEY);

	debounceTime[ROW_1] = millis () + DEBOUNCE_TIME ;
}

void row_2_isr (void) {
	// Pin event (key / button event) debouncing procedure
	if (millis () < debounceTime[ROW_2]) {
		debounceTime[ROW_2] = millis () + DEBOUNCE_TIME ;
		return;
	}

	piLock (FLAG_KEY);

	teclado.teclaPulsada.row = ROW_2;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags |= FLAG_KEY_PRESSED;

	piUnlock (FLAG_KEY);

	debounceTime[ROW_2] = millis () + DEBOUNCE_TIME ;
}

void row_3_isr (void) {
	// Pin event (key / button event) debouncing procedure
	if (millis () < debounceTime[ROW_3]) {
		debounceTime[ROW_3] = millis () + DEBOUNCE_TIME ;
		return;
	}

	piLock (FLAG_KEY);

	teclado.teclaPulsada.row = ROW_3;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags |= FLAG_KEY_PRESSED;

	piUnlock (FLAG_KEY);

	debounceTime[ROW_3] = millis () + DEBOUNCE_TIME ;
}

void row_4_isr (void) {
	// Pin event (key / button event) debouncing procedure
	if (millis () < debounceTime[ROW_4]) {
		debounceTime[ROW_4] = millis () + DEBOUNCE_TIME ;
		return;
	}

	piLock (FLAG_KEY);

	teclado.teclaPulsada.row = ROW_4;
	teclado.teclaPulsada.col = teclado.columna_actual;

	flags |= FLAG_KEY_PRESSED;

	piUnlock (FLAG_KEY);

	debounceTime[ROW_4] = millis () + DEBOUNCE_TIME ;
}

void col_1 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (FLAG_KEY);

	digitalWrite (GPIO_COL_4, LOW);
	digitalWrite (GPIO_COL_1, HIGH);
	digitalWrite (GPIO_COL_2, LOW);
	digitalWrite (GPIO_COL_3, LOW);

	flags &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_1;

	piUnlock (FLAG_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

void col_2 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (FLAG_KEY);

	digitalWrite (GPIO_COL_1, LOW);
	digitalWrite (GPIO_COL_2, HIGH);
	digitalWrite (GPIO_COL_3, LOW);
	digitalWrite (GPIO_COL_4, LOW);

	flags &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_2;

	piUnlock (FLAG_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

void col_3 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (FLAG_KEY);

	digitalWrite (GPIO_COL_2, LOW);
	digitalWrite (GPIO_COL_3, HIGH);
	digitalWrite (GPIO_COL_4, LOW);
	digitalWrite (GPIO_COL_1, LOW);

	flags &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_3;

	piUnlock (FLAG_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

void col_4 (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (FLAG_KEY);

	digitalWrite (GPIO_COL_3, LOW);
	digitalWrite (GPIO_COL_4, HIGH);
	digitalWrite (GPIO_COL_1, LOW);
	digitalWrite (GPIO_COL_2, LOW);

	flags &= (~FLAG_TMR_TIMEOUT);

	p_teclado->columna_actual = COL_4;

	piUnlock (FLAG_KEY);

	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);
}

int key_pressed (fsm_t* this) {
	int result = 0;

	piLock (FLAG_KEY);
	result = (flags & FLAG_KEY_PRESSED);
	piUnlock (FLAG_KEY);

	return result;
}

void process_key (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	piLock (FLAG_KEY);

	flags &= (~FLAG_KEY_PRESSED);

	switch(p_teclado->teclaPulsada.col){
		case COL_1:
		case COL_2:
		case COL_3:
		case COL_4:
			printf("\nKeypress \"%c\"...\n",
					tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]);
			fflush(stdout);

			switch(tecladoTL04[p_teclado->teclaPulsada.row][p_teclado->teclaPulsada.col]){

			case'D':
				piLock(PLAYER_FLAGS_KEY);
				flags_player |= FLAG_START_DISPARO;
				printf("Tecla D pulsada!\n");
				fflush(stdout);
				piUnlock(PLAYER_FLAGS_KEY);
				break;

			case '1':
				piLock(PLAYER_FLAGS_KEY);
				flags_player |= FLAG_START_IMPACTO;
				printf("Tecla 1 pulsada!\n");
				fflush(stdout);
				piUnlock(PLAYER_FLAGS_KEY);
				break;

			case '2':
				piLock(SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_UP;
				printf("Tecla 2 pulsada!\n");
				fflush(stdout);
				piUnlock(SYSTEM_FLAGS_KEY);
				break;

			case '4':
				piLock(SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_LEFT;
				printf("Tecla 4 pulsada!\n");
				fflush(stdout);
				piUnlock(SYSTEM_FLAGS_KEY);
				break;

			case '8':
				piLock(SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_DOWN;
				printf("Tecla 8 pulsada!\n");
				fflush(stdout);
				piUnlock(SYSTEM_FLAGS_KEY);
				break;

			case '6':
				piLock(SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_JOYSTICK_RIGHT;
				printf("Tecla 6 pulsada!\n");
				fflush(stdout);
				piUnlock(SYSTEM_FLAGS_KEY);
				break;

			case '0':
				piLock(SYSTEM_FLAGS_KEY);
				flags_juego |= FLAG_SYSTEM_START;
				printf("Tecla 0 pulsada!\n");
				fflush(stdout);
				piUnlock(SYSTEM_FLAGS_KEY);
				break;
			}
			break;

		default:
			printf("\nERROR!!!! invalid number of column (%d)!!!\n", p_teclado->teclaPulsada.col);
			fflush(stdout);

			p_teclado->teclaPulsada.row = -1;
			p_teclado->teclaPulsada.col = -1;

			break;
	}

	piUnlock (FLAG_KEY);

}

// wait until next_activation (absolute time)
void delay_until (unsigned int next) {
	unsigned int now = millis();
	if (next > now) {
		delay (next - now);
	}
}

void timer_duracion_columna_isr (union sigval value) {
	piLock (FLAG_KEY);
	flags |= FLAG_TMR_TIMEOUT;
	piUnlock (FLAG_KEY);
}

int initialize(TipoTeclado *p_teclado) {
	// Comenzamos excitacion por primera columna
	p_teclado->columna_actual = COL_1;

	// Inicialmente no hay tecla pulsada
	p_teclado->teclaPulsada.col = -1;
	p_teclado->teclaPulsada.row = -1;

	pinMode (GPIO_ROW_1, INPUT);
	pullUpDnControl(GPIO_ROW_1, PUD_DOWN);
	wiringPiISR (GPIO_ROW_1, INT_EDGE_RISING, row_1_isr);

	pinMode (GPIO_ROW_2, INPUT);
	pullUpDnControl(GPIO_ROW_2, PUD_DOWN);
	wiringPiISR (GPIO_ROW_2, INT_EDGE_RISING, row_2_isr);

	pinMode (GPIO_ROW_3, INPUT);
	pullUpDnControl(GPIO_ROW_3, PUD_DOWN);
	wiringPiISR (GPIO_ROW_3, INT_EDGE_RISING, row_3_isr);

	pinMode (GPIO_ROW_4, INPUT);
	pullUpDnControl(GPIO_ROW_4, PUD_DOWN);
	wiringPiISR (GPIO_ROW_4, INT_EDGE_RISING, row_4_isr);

	pinMode (GPIO_COL_1, OUTPUT);
	digitalWrite (GPIO_COL_1, HIGH);

	pinMode (GPIO_COL_2, OUTPUT);
	digitalWrite (GPIO_COL_2, LOW);

	pinMode (GPIO_COL_3, OUTPUT);
	digitalWrite (GPIO_COL_3, LOW);

	pinMode (GPIO_COL_4, OUTPUT);
	digitalWrite (GPIO_COL_4, LOW);

	p_teclado->tmr_duracion_columna = tmr_new (timer_duracion_columna_isr);
	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), COL_REFRESH_TIME);

	printf("\nSystem init complete! keypad ready to process the code!!!\n");
	fflush(stdout);

	return 0;
}

/*int main () {
	unsigned int next;

	fsm_trans_t columns[] = {
		{ KEY_COL_1, CompruebaColumnTimeout, KEY_COL_2, col_2 },
		{ KEY_COL_2, CompruebaColumnTimeout, KEY_COL_3, col_3 },
		{ KEY_COL_3, CompruebaColumnTimeout, KEY_COL_4, col_4 },
		{ KEY_COL_4, CompruebaColumnTimeout, KEY_COL_1, col_1 },
		{-1, NULL, -1, NULL },
	};

	fsm_trans_t keypad[] = {
		{ KEY_WAITING, key_pressed, KEY_WAITING, process_key},
		{-1, NULL, -1, NULL },
	};

	initialize(&teclado);

	fsm_t* columns_fsm = fsm_new (KEY_COL_1, columns, &teclado);
	fsm_t* keypad_fsm = fsm_new (KEY_WAITING, keypad, &teclado);

	next = millis();
	while (1) {
		fsm_fire (columns_fsm);
		fsm_fire (keypad_fsm);

		next += CLK_MS;
		delay_until (next);
	}
	return 0;
}*/
