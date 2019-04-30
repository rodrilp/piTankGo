
#include "torreta.h"


//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------


void InicializaTorreta (TipoTorreta *p_torreta) {

	flags_juego = 1;


	wiringPiSetupGpio();

	p_torreta->incremento = SERVO_INCREMENTO;
	p_torreta->minimo 	= SERVO_MINIMO;
	p_torreta->maximo 	= SERVO_MAXIMO;

	p_torreta->inicio 	= SERVO_MINIMO + (SERVO_MAXIMO - SERVO_MINIMO)/2;
	p_torreta->posicion.x 	= p_torreta->inicio;
	p_torreta->posicion.y 	= p_torreta->inicio;

	if((p_torreta->posicion.x > p_torreta->maximo) || (p_torreta->posicion.y > p_torreta->maximo)){
		p_torreta->posicion.x = p_torreta->maximo;
		p_torreta->posicion.y = p_torreta->maximo;
	}

	if((p_torreta->posicion.x < p_torreta->minimo) || (p_torreta->posicion.y > p_torreta->minimo)){
		p_torreta->posicion.x = p_torreta->minimo;
		p_torreta->posicion.y = p_torreta->minimo;
	}

	pinMode(SERVO_VERTICAL_PIN, OUTPUT);
	softPwmCreate (SERVO_VERTICAL_PIN, p_torreta->inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;
	softPwmWrite(SERVO_VERTICAL_PIN, p_torreta->posicion.y);

	pinMode(SERVO_HORIZONTAL_PIN, OUTPUT);
	softPwmCreate (SERVO_HORIZONTAL_PIN, p_torreta->inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;
	softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta->posicion.x);

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

	flags_juego =0;

	InicializaTorreta (p_torreta);

	printf("Comienza el juego");
	fflush(stdout);


}

void MueveTorretaArriba (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_juego &= (~FLAG_JOYSTICK_UP);

	if(p_torreta->posicion.y + p_torreta->incremento <= p_torreta->maximo){
		p_torreta-> posicion.y = p_torreta->posicion.y + p_torreta->incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta-> posicion.y);

		printf("[SERVO][POSICION]=[%d]\n", p_torreta->posicion.y);
		fflush(stdout);
	}

}

void MueveTorretaAbajo (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_juego &= (~FLAG_JOYSTICK_DOWN);

	if(p_torreta->posicion.y - p_torreta->incremento >= p_torreta->minimo){
		p_torreta-> posicion.y = p_torreta->posicion.y - p_torreta->incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta-> posicion.y);

		printf("[SERVO][POSICION]=[%d]\n", p_torreta->posicion.y);
		fflush(stdout);
	}
}

void MueveTorretaIzquierda (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_juego &= (~FLAG_JOYSTICK_LEFT);

	if(p_torreta->posicion.x + p_torreta->incremento <= p_torreta->maximo){
		p_torreta-> posicion.x = p_torreta->posicion.x + p_torreta->incremento;

		softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta-> posicion.x);

		printf("[SERVO][POSICION]=[%d]\n", p_torreta->posicion.x);
		fflush(stdout);
	}
}

void MueveTorretaDerecha (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_juego &= (~FLAG_JOYSTICK_RIGHT);

	if(p_torreta->posicion.x + p_torreta->incremento <= p_torreta->maximo){
		p_torreta-> posicion.x = p_torreta->posicion.x + p_torreta->incremento;

		softPwmWrite(SERVO_HORIZONTAL_PIN, p_torreta-> posicion.x);

		printf("[SERVO][POSICION]=[%d]\n", p_torreta->posicion.x);
		fflush(stdout);
	}
}

void DisparoIR (fsm_t* this) {

	//piLock(PLAYER_FLAGS_KEY);
	//flags_player |= FLAG_START_DISPARO;
	//piUnlock(PLAYER_FLAGS_KEY);

	printf("DISPARO IR");
	fflush(stdout);
}

void FinalDisparoIR (fsm_t* this) {
	printf("FIN DISPARO IR");
	fflush(stdout);
}

void ImpactoDetectado (fsm_t* this) {
	printf("IMPACTO DETECTADO");
	fflush(stdout);
}

void FinalizaJuego (fsm_t* this) {
	printf("FIN DEL JUEGO");
	fflush(stdout);
}
