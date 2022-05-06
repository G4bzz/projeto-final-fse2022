// Variavel para armazenar por quantos segundos a lampada
//  esteve apagada
unsigned long timer_luz = millis();

// Setup
void setup(){
  pinMode(8, OUTPUT); //Rele
  
  pinMode(2, INPUT); //Push button
  attachInterrupt(digitalPinToInterrupt(2), acende, RISING);
}

//Variavel da luz e função de controle
volatile byte luz = 0;
void acende(){
  luz = !luz;
}

void timer_lamps(){ //faz as verificacoes periodicas
  if((millis() - timer_luz) >= 2000){// se as luzes ficaram 2s apagadas
    digitalWrite(8, HIGH); //acende as luzes
    delay(2000); //permanece aceso por 2s
  	timer_luz = millis();
  }
  else digitalWrite(8, LOW);
}

//Programa
void loop(){
  //o botao funciona de maneira independente, e possui precedencia,
  // ou seja, ao ser pressionado, ignora o timer
  if(luz == 1) digitalWrite(8, HIGH);
  else digitalWrite(8, LOW);
  timer_lamps();
}