const int _leds[] = {A4,A3,A0,10};
const int _buttons[] = {A5,A2,13,9};
const int _startButton = 8;
const int _readyLed = A1;
const int _rptButton = 7;
const int _rptLed = 12;
const int _speaker = 6;
const int _scoreClk = 2;
const int _scoreRst = 4;
const int _tones[] = {330,195,392,262};
String _sequence = "";
bool _allowRepeatSeq;

void setup()
{
  for(int i=0; i<4; i++)
  {
    pinMode(_leds[i], OUTPUT);
    pinMode(_buttons[i], INPUT_PULLUP);
  }
  pinMode(_startButton, INPUT_PULLUP);
  pinMode(_readyLed, OUTPUT);
  pinMode(_rptButton, INPUT_PULLUP);
  pinMode(_rptLed, OUTPUT);
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

  // loop until user loses game
  while(continueGame)
  {
    updateSequence();
    seqLen++;
    showSequence();
    digitalWrite(_rptLed, HIGH);
    _allowRepeatSeq = true;
    
    String response = "";    
    for(int i=0; i<seqLen; i++)
    {
      response += getButtonPress();
      digitalWrite(_rptLed, LOW);
      if(_sequence.startsWith(response))
      {
        score++;
        updateScore();
        scoreIsMilestone(score);
      }
      else
      {
        continueGame = false;        
        // Game Lost
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
      else
      {
        // no button pressed yet, so see if the user can and wants to repeat the sequence
        if(_allowRepeatSeq && (digitalRead(_rptButton) == LOW))
        {
          showSequence();
        }        
      }
    }
  }
  _allowRepeatSeq = false;
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

void scoreIsMilestone(int score)
{
  
 if(score == 55 || score == 120 || score == 210 || 
    score == 325 || score == 465 || score == 630 || 
    score == 820 || score == 990)
  {
    playWinningSound();
  }
}



// Code below is for the winning sound - "Ode To Joy"
// Taken from https://github.com/robsoncouto/arduino-songs   
// Many thanks Robson Couto

/* 
  Ode to Joy - Beethoven's Symphony No. 9 
  Connect a piezo buzzer or speaker to pin 11 or select a new pin.
  More songs available at https://github.com/robsoncouto/arduino-songs                                            
                                              
                                              Robson Couto, 2019
*/
#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST 0

// change this to make the song slower or faster
int tempo=150; 

// change this to whichever pin you want to use
int buzzer = A1;

// notes of the moledy followed by the duration.
// a 4 means a quarter note, 8 an eighteenth , 16 sixteenth, so on
// !!negative numbers are used to represent dotted notes,
// so -4 means a dotted quarter note, that is, a quarter plus an eighteenth!!
int melody[] = {  
  NOTE_E4,4,  NOTE_E4,4,  NOTE_F4,4,  NOTE_G4,4,//1
  NOTE_G4,4,  NOTE_F4,4,  NOTE_E4,4,  NOTE_D4,4,
  NOTE_C4,4,  NOTE_C4,4,  NOTE_D4,4,  NOTE_E4,4,
  NOTE_E4,-4, NOTE_D4,8,  NOTE_D4,2,

  NOTE_E4,4,  NOTE_E4,4,  NOTE_F4,4,  NOTE_G4,4,//4
  NOTE_G4,4,  NOTE_F4,4,  NOTE_E4,4,  NOTE_D4,4,
  NOTE_C4,4,  NOTE_C4,4,  NOTE_D4,4,  NOTE_E4,4,
  NOTE_D4,-4,  NOTE_C4,8,  NOTE_C4,2,

  NOTE_D4,4,  NOTE_D4,4,  NOTE_E4,4,  NOTE_C4,4,//8
  NOTE_D4,4,  NOTE_E4,8,  NOTE_F4,8,  NOTE_E4,4, NOTE_C4,4,
  NOTE_D4,4,  NOTE_E4,8,  NOTE_F4,8,  NOTE_E4,4, NOTE_D4,4,
  NOTE_C4,4,  NOTE_D4,4,  NOTE_G3,2,

  NOTE_E4,4,  NOTE_E4,4,  NOTE_F4,4,  NOTE_G4,4,//12
  NOTE_G4,4,  NOTE_F4,4,  NOTE_E4,4,  NOTE_D4,4,
  NOTE_C4,4,  NOTE_C4,4,  NOTE_D4,4,  NOTE_E4,4,
  NOTE_D4,-4,  NOTE_C4,8,  NOTE_C4,2  
};

// sizeof gives the number of bytes, each int value is composed of two bytes (16 bits)
// there are two values per note (pitch and duration), so for each note there are four bytes
int notes=sizeof(melody)/sizeof(melody[0])/2; 

// this calculates the duration of a whole note in ms (60s/tempo)*4 beats
int wholenote = (60000 * 4) / tempo;

int divider = 0, noteDuration = 0;

void playWinningSound() {
  // MEH - added code to iterate over the leds as each of the notes are played
  int ledDir = 1;
  int ledIdx = 0;

  // iterate over the notes of the melody. 
  // Remember, the array is twice the number of notes (notes + durations)
  for (int thisNote = 0; thisNote < notes * 2; thisNote = thisNote + 2) {

    // calculates the duration of each note
    divider = melody[thisNote + 1];
    if (divider > 0) {
      // regular note, just proceed
      noteDuration = (wholenote) / divider;
    } else if (divider < 0) {
      // dotted notes are represented with negative durations!!
      noteDuration = (wholenote) / abs(divider);
      noteDuration *= 1.5; // increases the duration in half for dotted notes
    }

    // we only play the note for 90% of the duration, leaving 10% as a pause
    tone(_speaker, melody[thisNote], noteDuration*0.95);

    // MEH - light the correct LED
    digitalWrite(_leds[ledIdx], HIGH);

    // Wait for the specief duration before playing the next note.
    delay(noteDuration);

    // MEH - unlight the correct LED
    digitalWrite(_leds[ledIdx], LOW);
    // MEH - determine next led
    ledIdx += ledDir;
    if(ledIdx == -1 || ledIdx == 4)
    {
      ledDir = -ledDir;
      ledIdx = ledIdx + (ledDir * 2);
    }
    
    // stop the waveform generation before the next note.
    noTone(buzzer);
  } 
}
