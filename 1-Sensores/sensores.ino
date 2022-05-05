//Variáveis
float temperatura = 0;
float tensao = 0;
float lumi = 0;

//Setup
void setup() {
   Serial.begin(9600); //Inicializa o serial 
}

//Programa
void loop() {
  tensao  = analogRead(A2)*5;
  tensao = tensao/1024;
  temperatura = (tensao - 0.5)*100;// converte a leitura do sensor em ºC
  Serial.print("Temperatura: ");
  Serial.println(temperatura);
  
  lumi  = analogRead(A1); //leitura do sensor
  Serial.print("Luminosidade: ");
  Serial.println(lumi);
  Serial.println();
  delay(1000);
}