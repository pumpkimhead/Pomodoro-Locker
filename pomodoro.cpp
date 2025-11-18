// Biblioteca DIsplay e Servo
#include <LiquidCrystal.h>
#include <Servo.h>

// Variaveis para manter a contagem do tempo e do numero de sessoes de foco
int seconds = 0;
int minutes = 0;
int count = 0;

// Variaveis para determinar o tempo de casa sessao do pomodoro
const int study_minutes = 1; // 25 minutos
const int short_break_minutes = 1; // 5 minutos
const int long_break_minutes = 1; //  15 minutos
const int repeats = 2;

// Variavel usada para os intervalos do pomodoro
int break_duration;

// Declaracao dos pins usados nos hardwares
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
const int led1 = 6;
const int led2 = 7;
const int button = 8;
const int servoPin = 9; // Pin para o micro servo
const int resetButton = 10; // Variavel Botao de Reset
const int buzzerPin = 13; // Pin para o buzzer

// Objeto Servo
Servo myservo;

// Funcoes para os Beeps do Buzzer - Passivo

// Som PowerUp do Mario Bros
void playPowerUpSound() {
  // Nota 1: C5 (523 Hz) por 50ms
  tone(buzzerPin, 523, 50);
  delay(100);
  // Nota 2: E5 (659 Hz) por 50ms
  tone(buzzerPin, 659, 50);
  delay(100);
  // Nota 3: G5 (784 Hz) por 50ms
  tone(buzzerPin, 784, 50);
  delay(100);
  // Nota 4: C6 (1047 Hz) por 50ms, mais alta
  tone(buzzerPin, 1047, 50);
  delay(100);
  // Garantir que a nota pare de tocar
  noTone(buzzerPin);
}


void playMarioVictorySound() {
  // E5 (659 Hz)
  tone(buzzerPin, 659, 100);
  delay(120);
  
  // E5 (659 Hz)
  tone(buzzerPin, 659, 100);
  delay(120);

  // E5 (659 Hz)
  tone(buzzerPin, 659, 100);
  delay(120);
  
  // C5 (523 Hz)
  tone(buzzerPin, 523, 100);
  delay(120);

  // E5 (659 Hz)
  tone(buzzerPin, 659, 100);
  delay(120);
  
  // G5 (784 Hz)
  tone(buzzerPin, 784, 100);
  delay(300);

  // C6 (1046 Hz), o pico da melodia
  tone(buzzerPin, 1046, 200);
  delay(500);

  // Parar de tocar qualquer nota
  noTone(buzzerPin);
}

void playMarioDeathSound() {
  // Frequências das notas musicais
  int melody[] = {
    622, 587, 554, 523, 494, 466, 440, // D#5, D5, C#5, C5, B4, A#4, A4
    466, 440, 415, 392, 370, 349 // A#4, A4, G#4, G4, F#4, F4
  };
  
  // Duração de cada nota em milissegundos
  int noteDurations[] = {
    100, 100, 100, 100, 100, 100, 100,
    100, 100, 100, 100, 100, 100
  };
  
  for (int thisNote = 0; thisNote < 13; thisNote++) {
    tone(buzzerPin, melody[thisNote], noteDurations[thisNote]);
    delay(noteDurations[thisNote] + 20); // Pequena pausa entre as notas
  }
  
  noTone(buzzerPin);
}

void setup() // Setup dos componentes e inicializacao do pomodoro timer.
{
  lcd.begin(16, 2); // Numero de colunas e linhas para o LCD
  // Definicao das portas para os leds, saidas e a entrada para o botao.
  pinMode(led1,OUTPUT);
  pinMode(led2,OUTPUT);
  pinMode(button,INPUT);
  
  // Botao Reset
  pinMode(resetButton, INPUT_PULLUP); // Apertar e manter pressionado para ativar
  
  // Servo
  myservo.attach(servoPin);
    
}

void loop()
{
  // Mensagem inicial
  lcd.clear();
  lcd.print("Aperte o botao");
  lcd.setCursor(0,1);
  lcd.print("para iniciar!");
  
  // Servo posicao inicial
  myservo.write(0);
  
  // Espera ate que o botao seja pressionado
  while(digitalRead(button)==LOW){
  }
  
  delay(200);

  count = 0;  // define a contagem para zero
   
  while(count<repeats){ // Loop que alterna entre foco e intervalo

    // Mensagem e LEDs para o mudo de foco
    lcd.clear();
    lcd.print("Foco!");
    digitalWrite(led1,HIGH);
    digitalWrite(led2,LOW);
    lcd.setCursor(0, 1);

    // Ativa o servo no inicio do modo foco
    myservo.write(180); // Movimenta o servo para a posicao desejada

    // Reseta os minutos e os segundos
    seconds = 0;
    minutes = 0;
    
    
    // Efeito som do PowerUp do MarioBros
	playMarioVictorySound();

    // Contagem do timer e display durante o periodo de foco
    while(minutes<study_minutes){ // Contagem ate que o tempo limite de foco seja atingido
      seconds = 0;
      // print o valor do timer no formato mm:ss  
      while(seconds<60){
        lcd.setCursor(0, 1);
        if(minutes<10){  // se os minutos forem menores que 10, adicionar um 0 no display
          lcd.print("0");
        }
        lcd.print(minutes);
        lcd.print(":");
        if(seconds<10){  // se os segundos forem menosres de 10, adicionar um 0 no display
          lcd.print("0");
        }
        lcd.print(seconds);
        // esperar 1 segundo para entao encrementar o contador de segundos
        delay(1000);
        seconds++;
        if (digitalRead(resetButton) == LOW) {
          playMarioDeathSound();
          return;  // Botao do reset e verificado a cada segundo
        }
      }
      // incrementar o minuto apos o contador de segundos chegar em 60
      minutes++;
  
    }
    
    // Repetir o processo para o tempo de intervalo
    lcd.clear();
    lcd.setCursor(0, 0);
    digitalWrite(led1,LOW);
    digitalWrite(led2,HIGH);

    // Desabilitar o servo no inicio do intervalo
    myservo.write(0); // Movimentar o servo para a posicao de inicio
    
    if(count==(repeats-1)){ // fazer uma pausa de intervalo grande na ultima repeticao
      break_duration = long_break_minutes;
      lcd.print("Descanso Longo!");
      playPowerUpSound(); // Som para avisar descanso
    }
    else{  // Descanso curto
      break_duration = short_break_minutes;
      lcd.print("Descanso Curto!");
      playPowerUpSound(); // Som para avisar descanso
    }

    lcd.setCursor(0, 1);
    seconds = 0;
    minutes = 0;

    while(minutes<break_duration){
      seconds = 0;
      while(seconds<60){
        lcd.setCursor(0, 1);
        if(minutes<10){
          lcd.print("0");
        }
        lcd.print(minutes);
        lcd.print(":");
        if(seconds<10){
          lcd.print("0");
        }
        lcd.print(seconds);
        delay(1000);
        seconds++;
        if (digitalRead(resetButton) == LOW) {
          playMarioDeathSound(); 
          return;  // Botao do reset e verificado a cada segundo
        }
      }
      minutes++;
    }
    count++;  // incrementar o numero de sessoes de estudos

    
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Ciclo Completo!");
    lcd.setCursor(0, 1);
    lcd.print("Bom trabalho!");

    digitalWrite(led1,LOW);
    digitalWrite(led2,LOW);
    myservo.write(0);
    
    playMarioVictorySound();
  }
}

// Esse projeto foi inspirado no projeto do Science Buddies
// para acessar: https://www.sciencebuddies.org/science-fair-projects/project-ideas/Elec_p099/electricity-electronics/pomodoro-study-clock
