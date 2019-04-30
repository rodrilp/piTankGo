
#include "player.h"
#include <string.h>
#include <wiringPi.h>
#include <softTone.h>



//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

//------------------------------------------------------
// void InicializaEfecto (TipoEfecto *p_efecto)
//
// Descripcion: inicializa los parametros del objeto efecto.
// Ejemplo de uso:
//
// ...
//
// TipoEfecto efecto_demo;
//
// if ( InicializaEfecto (&efecto_demo, "DISPARO", frecuenciasDisparo, tiemposDisparo, 16) < 0 ) {
// 		printf("\n[ERROR!!!][InicializaEfecto]\n");
// 		fflush(stdout);
// 		}
//
// ...
//
//------------------------------------------------------
int InicializaEfecto (TipoEfecto *p_efecto, char *nombre, int *array_frecuencias, int *array_duraciones, int num_notas) {

	int i=0;
	strcpy(p_efecto->nombre, nombre);

	for(i=0; i<num_notas; i++){
		p_efecto->duraciones[i]=array_duraciones[i];
		p_efecto->frecuencias[i]=array_frecuencias[i];
	}

	p_efecto->num_notas=num_notas;

	return p_efecto->num_notas;
}

// Procedimiento de inicializacion del objeto especifico
// Nota: parte inicialización común a InicializaPlayDisparo y InicializaPlayImpacto
void InicializaPlayer (TipoPlayer *p_player) {

	InicializaEfecto(&(p_player->efecto_disparo),"Disparo", frecuenciaGOT, tiempoGOT,518);
	InicializaEfecto(&(p_player->efecto_impacto),"Impacto", frecuenciasImpacto, tiemposImpacto,32);

	p_player->p_efecto = &(p_player->efecto_disparo);

	p_player->posicion_nota_actual=0;
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];

	piLock(STD_IO_BUFFER_KEY);
	printf("\n[PLAYER][InicializaPlayer][NOTA 1][FREC %d][DURA %d]\n", p_player->frecuencia_nota_actual,p_player->duracion_nota_actual);
	piUnlock(STD_IO_BUFFER_KEY);

	pinMode(PLAYER_PWM_PIN, OUTPUT);
	//softPwmCreate(PLAYER_PWM_PIN);
	digitalWrite(PLAYER_PWM_PIN,0);

	p_player->timer = tmr_new(timer_player_duracion_nota_actual_isr);

}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaStartDisparo (fsm_t* this) {
	int result = 0;

	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_DISPARO);
	piUnlock(PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaStartImpacto (fsm_t* this) {
	int result = 0;

	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_START_IMPACTO);
	piUnlock (PLAYER_FLAGS_KEY);


	return result;
}

int CompruebaNuevaNota (fsm_t* this){
	int result = 0;

	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_PLAYER_END);
	piUnlock (PLAYER_FLAGS_KEY);

	return !result;
}

int CompruebaNotaTimeout (fsm_t* this) {
	int result = 0;

	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_NOTA_TIMEOUT);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

int CompruebaFinalEfecto (fsm_t* this) {
	int result = 0;

	piLock(PLAYER_FLAGS_KEY);
	result = (flags_player & FLAG_PLAYER_END);
	piUnlock (PLAYER_FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void InicializaPlayDisparo (fsm_t* this) {
	TipoPlayer *p_player = (TipoPlayer *) this->user_data;

	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_DISPARO;
	piUnlock (PLAYER_FLAGS_KEY);


	piLock (STD_IO_BUFFER_KEY);
	printf("Inicia Disparo\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

	p_player->p_efecto = & (p_player->efecto_disparo);

	p_player->posicion_nota_actual=0;
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];

	softToneWrite(PLAYER_PWM_PIN, p_player->frecuencia_nota_actual);
	tmr_startms(p_player->timer,p_player->duracion_nota_actual);

}

void InicializaPlayImpacto (fsm_t* this) {
	TipoPlayer *p_player = (TipoPlayer *) this->user_data;

	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_START_IMPACTO;
	flags_player &= ~FLAG_START_DISPARO;
	piUnlock (PLAYER_FLAGS_KEY);

	piLock (STD_IO_BUFFER_KEY);
	printf("Inicia impacto\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

	p_player->p_efecto = & (p_player->efecto_impacto);

	p_player->posicion_nota_actual=0;
	p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
	p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];



	softToneWrite(PLAYER_PWM_PIN, p_player->frecuencia_nota_actual);
	tmr_startms(p_player->timer,p_player->duracion_nota_actual);


}

void ComienzaNuevaNota (fsm_t* this) {
	TipoPlayer *p_player = (TipoPlayer*)this->user_data;

	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	piUnlock (PLAYER_FLAGS_KEY);

	softToneWrite(PLAYER_PWM_PIN, p_player->frecuencia_nota_actual);
	tmr_startms(p_player->timer,p_player->duracion_nota_actual);


	piLock (STD_IO_BUFFER_KEY);
	printf("[Player][ComienzaNuevaNota][NOTA %d] [FREC %d] [DURA %d]\n",p_player->posicion_nota_actual +1, p_player->frecuencia_nota_actual, p_player->duracion_nota_actual);
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);

}

void ActualizaPlayer (fsm_t* this) {
	TipoPlayer *p_player = (TipoPlayer*)this->user_data;

	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_NOTA_TIMEOUT;
	piUnlock (PLAYER_FLAGS_KEY);

	p_player->posicion_nota_actual++;

	if(p_player->posicion_nota_actual >= (p_player->p_efecto->num_notas )){

		piLock (PLAYER_FLAGS_KEY);
		flags_player |= FLAG_PLAYER_END;
		piUnlock (PLAYER_FLAGS_KEY);

	}
	else{
		piLock (STD_IO_BUFFER_KEY);
		printf("[Player][ActualizaPlayer][NUEVA NOTA (%d DE %d)]\n", p_player ->posicion_nota_actual, (p_player->p_efecto->num_notas ));
		fflush(stdout);
		piUnlock(STD_IO_BUFFER_KEY);

		p_player->frecuencia_nota_actual = p_player->p_efecto->frecuencias[p_player->posicion_nota_actual];
		p_player->duracion_nota_actual = p_player->p_efecto->duraciones[p_player->posicion_nota_actual];
	}


}

void FinalEfecto (fsm_t* this) {
	piLock (PLAYER_FLAGS_KEY);
	flags_player &= ~FLAG_PLAYER_END;
	//flags_player &= ~FLAG_START_DISPARO;
	piUnlock (PLAYER_FLAGS_KEY);

	softToneWrite(PLAYER_PWM_PIN,0);

	piLock (STD_IO_BUFFER_KEY);
	printf("SE HAN REPRODUCIDO TODAS LAS NOTAS\n");
	printf("Final Efecto\n");
	fflush(stdout);
	piUnlock (STD_IO_BUFFER_KEY);


}

//------------------------------------------------------
// PROCEDIMIENTOS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_player_duracion_nota_actual_isr (union sigval value) {
	piLock(PLAYER_FLAGS_KEY);
	flags_player |= FLAG_NOTA_TIMEOUT;
	piUnlock(PLAYER_FLAGS_KEY);
}
