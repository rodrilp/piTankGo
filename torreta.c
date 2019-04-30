
#include "torreta.h"
#include "servo_basico.h"

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

int flags_torreta = 0;

void InicializaTorreta (TipoTorreta *p_torreta) {

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

	if((p_torreta->posicion < p_torreta->minimo) || (p_torreta->posicion.y > p_torreta->minimo)){
		p_torreta->posicion.x = p_torreta->minimo;
		p_torreta->posicion.y = p_torreta->minimo;
	}

	softPwmCreate (SERVO_PIN, p_torreta->inicio, SERVO_PWM_RANGE); // Internamente ya hace: piHiPri (90) ;
	softPwmWrite(SERVO_PIN, p_torreta->posicion);
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaComienzo (fsm_t* this) {
	int result = 0;
	result = (flags_torreta & FLAG_SYSTEM_START);
	return result;
}

int CompruebaJoystickUp (fsm_t* this) {
	int result = 0;
	result = (flags_torreta & FLAG_JOYSTICK_UP);
	return result;
}

int CompruebaJoystickDown (fsm_t* fsm_player){
	int result = 0;
	result = (flags_torreta & FLAG_JOYSTICK_DOWN);
	return result;
}

int CompruebaJoystickLeft (fsm_t* this) {
	int result = 0;
	result = (flags_torreta & FLAG_JOYSTICK_LEFT);
	return result;
}

int CompruebaJoystickRight (fsm_t* this) {
	int result = 0;
	result = (flags_torreta & FLAG_JOYSTICK_RIGHT);
	return result;
}

int CompruebaTimeoutDisparo (fsm_t* this) {
	int result = 0;
	result = (flags_torreta & FLAG_SHOOT_TIMEOUT);
	return result;
}

int CompruebaImpacto (fsm_t* this) {
	int result = 0;
	result = (flags_torreta & FLAG_TARGET_DONE);
	return result;
}

int CompruebaTriggerButton (fsm_t* this) {
	int result = 0;
	result = (flags_torreta & FLAG_TRIGGER_BUTTON);
	return result;
}

int CompruebaFinalJuego (fsm_t* this) {
	int result = 0;
	result = (flags_torreta & FLAG_SYSTEM_END);
	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ComienzaSistema (fsm_t* this) {
	// A completar por el alumno
	// ...
}

void MueveTorretaArriba (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_torreta &= (~FLAG_JOYSTICK_UP);

	if(p_torreta->posicion.y + p_torreta->incremento <= p_torreta->maximo){
		p_torreta-> posicion.y = p_torreta->posicion.y + p_torreta->incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta-> posicion.y);

		printf("[SERVO][POSICION]=[%d]\n", p_torreta->posicion.y);
		fflush(stdout);
	}

}

void MueveTorretaAbajo (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_torreta &= (~FLAG_JOYSTICK_DOWN);

	if(p_torreta->posicion.y - p_torreta->incremento >= p_torreta->minimo){
		p_torreta-> posicion.y = p_torreta->posicion.y - p_torreta->incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta-> posicion.y);

		printf("[SERVO][POSICION]=[%d]\n", p_torreta->posicion.y);
		fflush(stdout);
	}
}

void MueveTorretaIzquierda (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_torreta &= (~FLAG_JOYSTICK_LEFT);

	if(p_torreta->posicion.x + p_torreta->incremento <= p_torreta->maximo){
		p_torreta-> posicion.x = p_torreta->posicion.x + p_torreta->incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta-> posicion.x);

		printf("[SERVO][POSICION]=[%d]\n", p_torreta->posicion.x);
		fflush(stdout);
	}
}

void MueveTorretaDerecha (fsm_t* this) {
	TipoTorreta *p_torreta = (TipoTorreta * ) this-> user_data;

	flags_torreta &= (~FLAG_JOYSTICK_RIGHT);

	if(p_torreta->posicion.x + p_torreta->incremento <= p_torreta->maximo){
		p_torreta-> posicion.x = p_torreta->posicion.x + p_torreta->incremento;

		softPwmWrite(SERVO_VERTICAL_PIN, p_torreta-> posicion.x);

		printf("[SERVO][POSICION]=[%d]\n", p_torreta->posicion.x);
		fflush(stdout);
	}
}

void DisparoIR (fsm_t* this) {
	// A completar por el alumno
	// ...
}

void FinalDisparoIR (fsm_t* this) {
	// A completar por el alumno
	// ...
}

void ImpactoDetectado (fsm_t* this) {
	// A completar por el alumno
	// ...
}

void FinalizaJuego (fsm_t* this) {
	// A completar por el alumno
	// ...
}
