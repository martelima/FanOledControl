/*
Autor: Mateus lima
Projeto: Cotrolador de velocidade proporcional a temperatura
Descrição: O seguinte código é utilizado em um microntrolador Arduino Uno R3 e tem como objetivo
           controlar um ventilador proporcional o aquecimento da temperatura de um sensor, este no
           caso, elaboramos o circuito simulado com um LDR(sensor luminoso) pois o sensor mais
           adequado para o contexto real não existia no simulador, recomendamos caso monte o circuito
           na vida real utilizar um NTC(sensor de temperatura), na mesma posição que está o LDR
           aprensentado

*/
// Bibliotecas:
#include <Wire.h>              // Biblioteca utilizada para fazer a comunicação com o I2C
#include <LiquidCrystal_I2C.h> // Biblioteca utilizada para fazer a comunicação com o display 20x4

const int col = 16;  // Serve para definir o numero de colunas do display utilizado
const int lin = 2;   // Serve para definir o numero de linhas do display utilizado
const int ende = 0x20; // Serve para definir o endereço do display.

LiquidCrystal_I2C lcd(ende, col, lin); // Chamada da funcação LiquidCrystal para ser usada com o I2C

const int PIN_MOTOR_FAN = 11; // pino para saída de sinal PWM de controle do motor

const int PIN_SENSOR_TMP = A1;

const float TempSensorSignal100DegreeCelsius = 309;
const float TempSensorSignal0DegreeCelsius = 102;
const int MotorMaxRPM = 8000;
const int AnalogWriteMaxSignal = 255;
;

void setup() // Incia o display
{
  pinMode(PIN_MOTOR_FAN, OUTPUT); // configura pino como saída
  pinMode(PIN_SENSOR_TMP, INPUT);

  lcd.init();      // Serve para iniciar a comunicação com o display já conectado
  lcd.backlight(); // Serve para ligar a luz do display
  lcd.clear();     // Serve para limpar a tela do display
}

void loop()
{
  const int signalSensorTemp = analogRead(PIN_SENSOR_TMP);
  const int signalMotor = signalSensorTemp / 2;

  analogWrite(PIN_MOTOR_FAN, signalMotor);
  printLCD(signalMotor,signalSensorTemp);
}

void printLCD(int signalFan, int signalSensorTemp)
{
 
  const long temperatureCelsius = map(signalSensorTemp,102,309,0,100);
  const int speedRPM = map(signalFan,0, AnalogWriteMaxSignal, 0, MotorMaxRPM);
  
  lcd.clear(); 
  lcd.setCursor(0,0);
  lcd.print("RPM:" );
  lcd.setCursor(4,0);
  lcd.print(speedRPM);
  lcd.setCursor(0,  1);
  lcd.print("TEMP: ");
  lcd.setCursor(5,1);
  lcd.print(temperatureCelsius);
  delay(300);
}
