#include <LiquidCrystal.h>
#include <Servo.h>

//Configuracao personalizada
#define tempoReposicaoAgua 25000	//tempo em milésimos
#define tempoRemocaoAgua 3000	//tempo em milésimos
#define tempoAdicaoAgua 3000	//tempo em milésimos
#define limiteMinimoRacao 400	//quantidade em gramas
#define limiteMaximoRacao 900	//quantidade em gramas
#define temperaturaMinima 30	//temperatura em grau Celsius
#define temperaturaMaxima 40	//temperatura em grau Celsius

//Declação do display;
LiquidCrystal lcd(12, 11, 7, 6, 5,4);

//Declarando os servos
Servo servo_Comida;
Servo servo_rmvAgua;
Servo servo_addAgua;

//Variáveis
unsigned long timer_Agua = millis();
unsigned long timer_luz = millis();
unsigned long timer_buzzer = millis();
volatile byte est_botaoComida = 0;
volatile byte est_botaoAgua = 0;
volatile byte repondo_agua = 0;
volatile byte repondo_comida = 0;
float temperatura = 0;
float tensao = 0;

//Setup
void setup() {
	//Inicializa o LCD
	lcd.begin(16,2);
	lcd.clear();

	//Relé
	pinMode(13, OUTPUT);

  	//Servos
	servo_Comida.attach(8);
	servo_rmvAgua.attach(9);
	servo_addAgua.attach(10);

	//Botoes de interrupcao
	pinMode (2, INPUT);
	attachInterrupt(digitalPinToInterrupt(2), troca_estComida, RISING);
	pinMode (3, INPUT);
	attachInterrupt(digitalPinToInterrupt(3), troca_estAgua, RISING);
	
	//Balanca
	pinMode (A3, INPUT);

	//Buzzer
	pinMode (A4, OUTPUT);
	
	//"Ventilador"
	pinMode (A5, OUTPUT);

	//Interruptor  
  	pinMode(A0,INPUT);
	
	//Inicializa os servos
	servo_addAgua.write(0);
	servo_rmvAgua.write(0);
	servo_Comida.write(0);
}

float calcTemp(){
  tensao  = analogRead(A1)*5;
  tensao = tensao/1024;
  temperatura = (tensao - 0.5)*100;// converte a leitura do sensor em C
  if(temperatura >45) digitalWrite(A5, HIGH);
  return temperatura;
}

float calcLumi(){
	return analogRead(A2)/10;
}

//Mostra as informações dos sensores no LCD, em tempo real
void showInfos(){ 
  lcd.setCursor(6,0);
  lcd.print(calcTemp());
  lcd.setCursor(7,1);
  float racao = analogRead(A3);
  lcd.print(racao/1000);
}

//Temporizador que aciona as trocas automaticas
void temporizador_agua(){
  	if((millis() - timer_Agua) > tempoReposicaoAgua){
		if(!repondo_agua) troca_agua();
		timer_Agua = millis();
	}	
}

//Troca de agua
void troca_agua(){
	//Feedback LCD
	lcd.clear();
	lcd.setCursor(1,0);
	lcd.print("Repondo a");
	lcd.setCursor(1,1);
  	lcd.print("agua...");

	//Obs.: tempos de acionamento ficticios, apenas para teste
	repondo_agua = !repondo_agua;
	servo_rmvAgua.write(90);
	delay(tempoRemocaoAgua);
	servo_rmvAgua.write(0);
	delay(1000);
	servo_addAgua.write(90);
	delay(tempoAdicaoAgua);
	servo_addAgua.write(0);
	delay(1000);
	repondo_agua = !repondo_agua;
	lcd.clear();
}

//Estado do botão de troca de comida
void troca_estComida(){
	if(!repondo_comida) est_botaoComida = !est_botaoComida;
}

//Estado do botão de troca de agua
void troca_estAgua(){
	if(!repondo_agua) est_botaoAgua = !est_botaoAgua;
}

//Balanca
void balanca(){
	//Quando o compartimento de comida estiver com 900g ou mais de racao, finaliza a reposicao
	if(analogRead(A3) >= limiteMaximoRacao && repondo_comida){
		lcd.clear();
		servo_Comida.write(0);
		repondo_comida = !repondo_comida;
		est_botaoComida = 0;
	}
	//Caso o compartimento esteja em nivel baixo, aciona a reposicao
	else if (analogRead(A3) < limiteMinimoRacao && !repondo_comida){
		//Feedback LCD
		lcd.clear();
		servo_Comida.write(90);
		repondo_comida = !repondo_comida;
		lcd.setCursor(1,0);
		lcd.print("Repondo a");
		lcd.setCursor(1,1);
		lcd.print("racao...");
	}
	//Enquanto estiver repondo a racao, mantem o dispenser aberto
	else if (repondo_comida){
		servo_Comida.write(90);
	}
}

void timer_lamps(){ //faz as verificacoes periodicas
	if((millis() - timer_luz) >= 1000){// se as luzes ficaram 2s apagadas
		if(calcLumi() <= 2) digitalWrite(13, HIGH); //acende as luzes
		else digitalWrite(13, LOW);
		timer_luz = millis();
	}
}

void buzzer(bool v){
	if((millis() - timer_buzzer) > 500 && v){
		digitalWrite(A4, HIGH);
		timer_buzzer = millis();
	}
	else digitalWrite(A4, LOW);
}

void emergencia(){
	if(calcTemp() > temperaturaMaxima){
		lcd.setCursor(15,0);
		lcd.print("!");
		buzzer(true);
		digitalWrite(13, LOW);
		digitalWrite(A5, HIGH);
	}
	else if(calcTemp() < temperaturaMinima){
		lcd.setCursor(15,0);
		lcd.print("!");
		buzzer(true);
		digitalWrite(A5, LOW);
		digitalWrite(13, HIGH);
	}
	else {
		lcd.setCursor(15,0);
		lcd.print(" ");
		buzzer(false);
		digitalWrite(A5, LOW);
	}
}

void troca_comida(){
	servo_Comida.write(90);
	repondo_comida = !repondo_comida;
	troca_estComida();
}

void print_testando(String texto){
	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Testando");
	lcd.setCursor(0,1);
  	lcd.print(texto);
}

//Demora cerca de 15 segundos
void tester_componentes(){
	print_testando("lampadas...");
	digitalWrite(13, HIGH);
	delay(2000);
	digitalWrite(13, LOW);

	print_testando("alarme...");
	buzzer(true);
	delay(2000);
	buzzer(false);

	print_testando("ventilador...");
	digitalWrite(A5, HIGH);
	delay(2000);
	digitalWrite(A5, LOW);

	print_testando("reposicoes...");
	servo_rmvAgua.write(90);
	servo_addAgua.write(90);
	servo_Comida.write(90);
	delay(1000);
	servo_rmvAgua.write(0);
	servo_addAgua.write(0);
	servo_Comida.write(0);

	print_testando("sensores...");
  	delay(2000);
	lcd.clear();
	lcd.print("Temp:");
	lcd.setCursor(0,1);
	lcd.print("Racao:");
	lcd.setCursor(11,1);
	lcd.print("kg");
	showInfos();
	delay(3000);

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print("Testando sensor");
	lcd.setCursor(0,1);
	lcd.print("de luz:");
	lcd.setCursor(8,1);
	lcd.print(calcLumi());
  	delay(2000);

	lcd.clear();
	lcd.setCursor(0,0);
  	lcd.print("Teste finalizado");
  	delay(2000);
  	lcd.clear();
}

//Programa
void loop() {
	if(!repondo_comida && !repondo_agua){
		lcd.setCursor(0,0);
		lcd.print("Temp:");
		lcd.setCursor(0,1);
		lcd.print("Racao:");
		lcd.setCursor(11,1);
		lcd.print("kg");
  		showInfos();
	}
  
	if(est_botaoComida && !repondo_comida){
		lcd.clear();
		lcd.setCursor(1,0);
		lcd.print("Repondo a");
		lcd.setCursor(1,1);
		lcd.print("racao...");
		troca_comida();
	}
	
	if(est_botaoAgua && !repondo_agua) {
		troca_agua();
		troca_estAgua();
		timer_Agua = millis();
	}

	if(analogRead(A0) > 1000){ //Se o interruptor de testes for acionado
		tester_componentes();
	}
	Serial.println(est_botaoComida);
	temporizador_agua();
	timer_lamps();
	balanca();
	emergencia();
}