const int _leds[] = {10,13,A1,A3};
const int _buttons[] = {9,12,A0,A2};
const int _startButton = A4;
const int _readyLed = A5;
const int _speaker = 4;
const int _scoreClk = 7;
const int _scoreRst = 8;
const int _tones[] = {330,195,392,262};
String _sequence = "";

void setup()
{
  for(int i=0; i<4; i++)
  {
    pinMode(_leds[i], OUTPUT);
    pinMode(_buttons[i], INPUT_PULLUP);
  }
  pinMode(_startButton, INPUT_PULLUP);
  pinMode(_readyLed, OUTPUT);
  pinMode(_scoreClk, OUTPUT);
  pinMode(_scoreRst, OUTPUT);
  pinMode(_speaker, OUTPUT);
  
  for(int i=0; i<4; i++)
  {
    digitalWrite(_leds[i], LOW);
  }  
  resetScore();
}

void loop()
{
  // Wait for user to press start button
  waitForPlayerStart();
  resetScore();
  
  // initialize game
  int score = 0;
  _sequence = "";
  int seqLen = 0;
  bool continueGame = true;

  // loop until player loses
  while(continueGame)
  {
    updateSequence();
    seqLen++;
    showSequence();
    
    String response = "";    
    for(int i=0; i<seqLen; i++)
    {
    response += getButtonPress();
      if(_sequence.startsWith(response))
      {
        score++;
        updateScore();
        if(scoreIs99(score))
        {
          continueGame = false;
          break;
        }
      }
      else
      {
        continueGame = false;        
        // Gsme Lost
        tone(_speaker, 42, 1500);
        break;
      }
    }
    delay(1000);
  }  
}

void waitForPlayerStart()
{
  digitalWrite(_readyLed, HIGH);
  while(digitalRead(_startButton) == HIGH){}
  delay(150);
  while(digitalRead(_startButton) == LOW){}
  digitalWrite(_readyLed, LOW);
  delay(500);        
}

void showSequence()
{
  int l = _sequence.length();
  int toneLen = getToneLen(l);
  
  for(int i=0; i<l; i++)
  {
    int ledIdx = _sequence[i] - 49;    
    digitalWrite(_leds[ledIdx], HIGH);
    tone(_speaker, _tones[ledIdx], toneLen);
    delay(toneLen);
    digitalWrite(_leds[ledIdx], LOW);
    delay(75);
  }
}

int getToneLen(int sequenceLen)
{  
  if(sequenceLen>11) return 260;
  if(sequenceLen>7)  return 380;
  return 500;
}

void updateSequence()
{
  int randomSeq = 49 + (millis() % 4);
  _sequence += char(randomSeq);
}

char getButtonPress()
{
  char response;
  bool buttonPressed = false;
  while(!buttonPressed)
  {
    for(int i=0; i<4; i++)
    {      
      if(digitalRead(_buttons[i]) == LOW)
      {
        // we have a button press, so record it, and light the led
        response = char(i + 49);
        digitalWrite(_leds[i], HIGH);
        tone(_speaker, _tones[i]);
        // debounce delay
        delay(100);
        // wait for the player to release the button, then turn the led off
        while(digitalRead(_buttons[i]) == LOW){}
        digitalWrite(_leds[i], LOW);
        buttonPressed = true;
        noTone(_speaker);
        break;
      }
    }
  }
  return response;
}

void resetScore()
{
  digitalWrite(_scoreRst, HIGH);
  delay(10);
  digitalWrite(_scoreRst, LOW);
}

void updateScore()
{
  digitalWrite(_scoreClk, HIGH);
  delay(10);
  digitalWrite(_scoreClk, LOW);
}

bool scoreIs99(int score)
{
  if(score == 99)
  {
    for(int j=0; j<3; j++)
    {
      for(int i=0; i<4; i++)
      {
        digitalWrite(_leds[i], HIGH);
        tone(_speaker, _tones[i], 420);
        delay(420);
        digitalWrite(_leds[i], LOW);
      }
    }
    return true;
  }
  return false;
}
