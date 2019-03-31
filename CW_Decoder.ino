
#include "Keyboard.h"
unsigned long signal_len,t1,t2,t0;   //time for which button is pressed
int inputPin = 9;                 //input pin for push button
int ledPin = 4;                   //outpu pin for LED
int sp;
String code = "";                 //string in which one alphabet is stored

void setup()
{
  Serial.begin(9600);
  pinMode(inputPin, INPUT_PULLUP); //internal pullup resistor is used to simplify the circuit
  pinMode(ledPin,OUTPUT);
  Keyboard.begin();
  Keyboard.releaseAll();
}

void loop()
{
NextDotDash:
  t0 = millis();
  while (digitalRead(inputPin) == HIGH) {}
  if (millis()-t0>2000) {
        Keyboard.press(' ');
        Keyboard.releaseAll();
    }
  t1 = millis();                            //time at button press
  digitalWrite(ledPin, HIGH);               //LED on while button pressed
  while (digitalRead(inputPin) == LOW) {}
  t2 = millis();                            //time at button release
  digitalWrite(ledPin, LOW);                //LED off on button release
  signal_len = t2 - t1;                     //time for which button is pressed
  if (signal_len > 20)                      //to account for switch debouncing
  {
    code += readio();                       //function to read dot or dash
  }
  while ((millis() - t2) < 500)           //if time between button press greater than 0.5sec, skip loop and go to next alphabet
  {     
    if (digitalRead(inputPin) == LOW)
    {
      goto NextDotDash;
    }
  }
  convertor();                          //function to decipher code into alphabet
}

char readio()
{
  if (signal_len < 200 && signal_len > 20)
  {
    return '.';                        //if button press less than 0.6sec, it is a dot
  }
  else //if (signal_len >= 300)
  {
    return '-';                        //if button press more than 0.6sec, it is a dash
  }
}

void convertor()
{
  static String letters[] = {".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---", "-.-", ".-..", "--", "-.", "---", ".--.", "--.-",
                             ".-.", "...", "-", "..-", "...-", ".--", "-..-", "-.--", "--..",
                             "-----",".----","..---","...--","....-",".....","-....","--...","---..","----.",
                             "E"};
  int i = 0; 
  char c;
  if (code == ".-.-.-")
  {
    Serial.print(".");        //for break
  }
  else
  {
    while (letters[i] != "E")  //loop for comparing input code with letters array
    {
      if (letters[i] == code)
      {
        if (i>25)
          c = '0'+(i-26);
        else
          c = 'A'+i;
        Keyboard.press(char(c));
        Serial.print(char(c));
        Keyboard.releaseAll();
        break;
      }
      i++;
    }
    if (letters[i] == "E")
    {
      Serial.println("");  //if input code doesn't match any letter, error
    }
  }
  code = "";                            //reset code to blank string
}
