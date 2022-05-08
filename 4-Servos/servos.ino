#include <Servo.h>

//Declarando os servos
Servo servo_Comida;
Servo servo_rmvAgua;
Servo servo_addAgua;

//Variáveis
unsigned long timer_Agua = millis();
volatile byte est_botaoComida = 0;
volatile byte est_botaoAgua = 0;
volatile byte busy = 0;
volatile byte repondo_comida = 0;

//Setup
void setup(){
	//Servos
	servo_Comida.attach(8);
	servo_rmvAgua.attach(13);
	servo_addAgua.attach(10);

	//Botoes de interrupcao
	pinMode (2, INPUT);
	attachInterrupt(digitalPinToInterrupt(2), troca_estComida, RISING);
	pinMode (3, INPUT);
	attachInterrupt(digitalPinToInterrupt(3), troca_estAgua, RISING);
	
	//Balanca
	pinMode (A3, INPUT);

	//Inicializa os servos
	servo_addAgua.write(0);
	servo_rmvAgua.write(0);
	servo_Comida.write(0);
}

//Temporizador que aciona as trocas automaticas
void temporizador_agua(){
  	if((millis() - timer_Agua) > 3000 && !busy){
		troca_agua();
		timer_Agua = millis();
	}	
}


//Troca de agua
void troca_agua(){
	//Obs.: tempos de acionamento ficticios, apenas para teste
	busy = !busy;
	servo_rmvAgua.write(90);
	delay(2000);
	servo_rmvAgua.write(0);
	delay(1000);
	servo_addAgua.write(90);
	delay(2000);
	servo_addAgua.write(0);
	delay(1000);
	busy = !busy;
}

//Estado do botão de troca de comida
void troca_estComida(){
	est_botaoComida = !est_botaoComida;
}

//Estado do botão de troca de agua
void troca_estAgua(){
	est_botaoAgua = !est_botaoAgua;
}

//Balanca
void balanca(){
	//Quando o compartimento de comida estiver com 900g ou mais de racao, finaliza a reposicao
	if(analogRead(A3) >= 900 && repondo_comida){
		servo_Comida.write(0);
		repondo_comida = !repondo_comida;
	}
	//Caso o compartimento esteja em nivel baixo, aciona a reposicao
	else if (analogRead(A3) < 400 && !repondo_comida){
		servo_Comida.write(90);
		repondo_comida = !repondo_comida;
	}
	//Enquanto estiver repondo a racao, mantem o dispenser aberto
	else if (repondo_comida){
		servo_Comida.write(90);
	}
}

//Programa
void loop(){
	if(est_botaoComida==1 && !repondo_comida) {
		servo_Comida.write(90);
		repondo_comida = !repondo_comida;
		troca_estComida();
	}
	
	if(est_botaoAgua==1) {
		troca_agua();
		troca_estAgua();
	}
	temporizador_agua();
	balanca();
}