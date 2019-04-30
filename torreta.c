
#include "torreta.h"


//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------


void InicializaTorreta (TipoTorreta *p_torreta) {

	//flags_juego = 1;
	p_torreta->servo_x.incremento = SERVO_INCREMENTO;
	p_torreta->servo_x.minimo 	= SERVO_MINIMO;
	p_torreta->servo_x.maximo 	= SERVO_MAXIMO;

	p_torreta->servo_x.inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_torreta->posicion.x	= p_torreta->servo_x.inicio;


	if(p_torreta->posicion.x > p_torreta->servo_x.maximo) {
		p_torreta->posicion.x = p_torreta->servo_x.maximo;
	}

	if(p_torreta->posicion.x < p_torreta->servo_x.minimo){
		p_torreta->posicion.x = p_torreta->servo_x.minimo;
	}


	p_torreta->servo_y.incremento = SERVO_INCREMENTO;
	p_torreta->servo_y.minimo 	= SERVO_MINIMO;
	p_torreta->servo_y.maximo 	= SERVO_MAXIMO;

	p_torreta->servo_y.inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_torreta->posicion.y = p_torreta->servo_y.inicio;

	if(p_torreta->posicion.y > p_torreta->servo_y.maximo){
		p_torreta->posicion.y = p_torreta->servo_y.maximo;
	}

	if(p_torreta->posicion.y < p_torreta->servo_y.minimo){
		p_torreta->posicion.y = p_torreta->servo_y.minimo;
	}

	printf("[POSICION INICIO x]=[%d]\n", p_torreta->servo_x.inicio);
	printf("[POSICION INICIO y]=[%d]\n", p_torreta->servo_y.inicio);
	printf("[POSICION x]=[%d]\n", p_torreta->posicion.x);
	printf("[POSICION y]=[%d]\n", p_torreta->posicion.y);
	fflush(stdout);


	pinMode(SERVO_VERTICAL_PIN, OUTPUT);
	softPwmCreate (SERVO_VERTICAL_PIN, p_torreta->servo_x.inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;

	pinMode(SERVO_HORIZONTAL_PIN, OUTPUT);
	softPwmCreate (SERVO_HORIZONTAL_PIN, p_torreta->servo_y.inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;

	pinMode (IR_TX_PIN, OUTPUT);
	digitalWrite(IR_TX_PIN,LOW);

	pinMode (IR_RX_PIN, INPUT);
	pullUpDnControl(IR_RX_PIN, PUD_DOWN);
	wiringPiISR (IR_RX_PIN, INT_EDGE_RISING, impacto_isr);


	p_torreta->timerDisparo = tmr_new(timer_duracion_disparo_isr);
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaComienzo (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_SYSTEM_START);
	piUnlock(SYSTEM_FLAGS_KEY);
	return result;
}

int CompruebaJoystickUp (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_UP);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaJoystickDown (fsm_t* fsm_player){
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_DOWN);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaJoystickLeft (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_LEFT);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaJoystickRight (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_JOYSTICK_RIGHT);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaTimeoutDisparo (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_SHOOT_TIMEOUT);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaImpacto (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_TARGET_DONE);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaTriggerButton (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_TRIGGER_BUTTON);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

int CompruebaFinalJuego (fsm_t* this) {
	int result = 0;
	piLock(SYSTEM_FLAGS_KEY);
	result = (flags_juego & FLAG_SYSTEM_END);
	piUnlock(SYSTEM_FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ComienzaSistema (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_juego &= (~FLAG_SYSTEM_START);

	InicializaTorreta (p_torreta);

	printf("Comienza el juego");
	fflush(stdout);
}

void MueveTorretaArriba (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_juego &= (~FLAG_JOYSTICK_UP);

	if(p_torreta->posicion.y + p_torreta->servo_y.incremento <= p_torreta->servo_y.maximo){
		p_torreta->posicion.y = p_torreta->posicion.y + p_torreta->servo_y.incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.y);

		printf("[SERVO ARRIBA][POSICION]=[%d]\n", p_torreta->posicion.y);
		fflush(stdout);
	}

}

void MueveTorretaAbajo (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_juego &= (~FLAG_JOYSTICK_DOWN);

	if(p_torreta->posicion.y - p_torreta->servo_y.incremento >= p_torreta->servo_x.minimo){
		p_torreta-> posicion.y = p_torreta->posicion.y - p_torreta->servo_y.incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.y);

		printf("[SERVO ABAJO][POSICION]=[%d]\n", p_torreta->posicion.y);
		fflush(stdout);
	}
}

void MueveTorretaIzquierda (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_juego &= (~FLAG_JOYSTICK_LEFT);

	if(p_torreta->posicion.x + p_torreta->servo_x.incremento <= p_torreta->servo_x.maximo){
		p_torreta->posicion.x = p_torreta->posicion.x + p_torreta->servo_x.incremento;

		softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.x);

		printf("[SERVO IZQUIERDA][POSICION]=[%d]\n", p_torreta->posicion.x);
		fflush(stdout);
	}
}

void MueveTorretaDerecha (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_juego &= (~FLAG_JOYSTICK_RIGHT);

	if(p_torreta->posicion.x - p_torreta->servo_x.incremento >= p_torreta->servo_x.minimo){
		p_torreta-> posicion.x = p_torreta->posicion.x - p_torreta->servo_x.incremento;

		softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.x);

		printf("[SERVO DERECHA][POSICION]=[%d]\n", p_torreta->posicion.x);
		fflush(stdout);
	}
}

void DisparoIR (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_juego &= (~FLAG_TRIGGER_BUTTON);

	printf("[POSICION DE LA TORRETA]=[%d, %d]\n", p_torreta->posicion.x, p_torreta->posicion.y);
	fflush(stdout);

	digitalWrite(IR_TX_PIN,HIGH);
	printf("DISPARO IR");
	fflush(stdout);

	piLock(PLAYER_FLAGS_KEY);
	flags_player |= FLAG_START_DISPARO;
	piUnlock(PLAYER_FLAGS_KEY);


	tmr_startms(p_torreta->timerDisparo,SHOOTING_PERIOD);
}

void FinalDisparoIR (fsm_t* this) {

	flags_juego &= (~FLAG_SHOOT_TIMEOUT);

	digitalWrite(IR_TX_PIN, LOW);

	printf("FIN DISPARO IR");
	fflush(stdout);
}

void ImpactoDetectado (fsm_t* this) {

	flags_juego &= (~FLAG_TARGET_DONE);

	printf("IMPACTO DETECTADO");
	fflush(stdout);

	piLock(PLAYER_FLAGS_KEY);
	flags_player |= FLAG_START_IMPACTO;
	piUnlock(PLAYER_FLAGS_KEY);

}

void FinalizaJuego (fsm_t* this) {
	printf("FIN DEL JUEGO");
	fflush(stdout);
}

void timer_duracion_disparo_isr (union sigval value){
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego |= FLAG_SHOOT_TIMEOUT;
	piUnlock(SYSTEM_FLAGS_KEY);
}

void impacto_isr(void){
	piLock(SYSTEM_FLAGS_KEY);
	flags_juego |= FLAG_TARGET_DONE;
	piUnlock(SYSTEM_FLAGS_KEY);
}
