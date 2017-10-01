
/* morse.h
 * Library for morse code decripting
 * WIP
 * 
 */
#ifndef LENGTH_LONG
const int PROGMEM LENGTH_LONG = 750; // time, in ms
#endif
#ifndef LENGTH_SHORT
const int PROGMEM LENGTH_SHORT = 250;
#endif 
#ifndef SENS
const int PROGMEM SENS = 50;
#endif
#ifndef RESET_TIME
const int PROGMEM RESET_TIME = 7000; // note: adjust depending on your pro level
#endif
#ifndef MAX_NEXT_TIME
const int PROGMEM MAX_NEXT_TIME = 700;
#endif 

int morseChar[5] = {0, 0, 0, 0, 0};



/*  1 = short pulse
 *  2 = long pulse
 *  0 = not defined
 */
const int PROGMEM NUM_CHARS = 36; // change this when modifying the character arrays
const int PROGMEM morseDict[36][5] =
{ { 1, 2, 0, 0, 0 } , // a
  { 2, 1, 1, 1, 0 } ,
  { 2, 1, 2, 1, 0 } ,
  { 2, 1, 1, 0, 0 } ,
  { 1, 0, 0, 0, 0 } ,
  { 1, 1, 2, 1, 0 } ,
  { 2, 2, 1, 0, 0 } ,
  { 1, 1, 1, 1, 0 } ,
  { 1, 1, 0, 0, 0 } ,
  { 1, 2, 2, 2, 0 } ,
  { 2, 1, 2, 0, 0 } ,
  { 1, 2, 1, 1, 0 } ,
  { 2, 2, 0, 0, 0 } ,
  { 2, 1, 0, 0, 0 } ,
  { 2, 2, 2, 0, 0 } ,
  { 1, 2, 2, 1, 0 } ,
  { 2, 2, 1, 2, 0 } ,
  { 1, 2, 1, 0, 0 } ,
  { 1, 1, 1, 0, 0 } ,
  { 2, 0, 0, 0, 0 } ,
  { 1, 1, 2, 0, 0 } ,
  { 1, 1, 1, 2, 0 } ,
  { 1, 2, 2, 0, 0 } ,
  { 2, 1, 1, 2, 0 } ,
  { 2, 1, 2, 2, 0 } ,
  { 2, 2, 1, 1, 0 } , // z
  { 2, 2, 2, 2, 2 } , // 0
  { 1, 2, 2, 2, 2 } ,
  { 1, 1, 2, 2, 2 } ,
  { 1, 1, 1, 2, 2 } ,
  { 1, 1, 1, 1, 2 } ,
  { 1, 1, 1, 1, 1 } ,
  { 2, 1, 1, 1, 1 } ,
  { 2, 2, 1, 1, 1 } ,
  { 2, 2, 2, 1, 1 } ,
  { 2, 2, 2, 2, 1 } // 9
} ;

// 0-25 : a-z
// 26-35 : 0-9

const char PROGMEM alphanumDict[36] = // this is ugly as fuck, but works
{ 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z', '0', '1', '2', '3', '4', '5', '6', '7', '8', '9' } ;

//initial declaration of functions
char readMorseChar(const int inputPin);
char convertMorseChar(int morseChar[]);
bool compareMorseChars(int morseChar[], int morseDict[]);

char readMorseChar(const int inputPin) {
  int morseChar[5] = { 0 };
  int timeHigh;
  int timeLow;
  int timeStart;
  int timeRisingEdge;
  int timeFallingEdge;
  int input;
  int i = 0;
  
  
  do { // loop for a full character (word)
    timeStart = millis();
    // loop to check for rising edge
    do { 
      input = digitalRead(inputPin);
      // this if tries to avoid the program hanging in one of these listener loops
      if( (millis() - timeStart) > RESET_TIME ) {
        return NULL;
      }
    } while(input == LOW);
    
    // note: Im trusting this operation is faster than the user pressing and releasing the button. If not, bad stuff will happen
    timeRisingEdge = millis();
    
    do { // loop for a bit (char)

      // loop to check for falling edge
      do { 
        input = digitalRead(inputPin);
      } while(input == HIGH);
      
      timeFallingEdge = millis();

      timeHigh = timeFallingEdge - timeRisingEdge;
      if( timeHigh > (LENGTH_SHORT - SENS) && timeHigh < (LENGTH_SHORT + SENS) ) {
        morseChar[i] = 1;
        i++;
      } else if ( timeHigh > (LENGTH_LONG - SENS) && timeHigh < (LENGTH_LONG + SENS) ) {
        morseChar[i] = 2;
        i++;
      }

      // loop to check for rising edge
      do { 
        input = digitalRead(inputPin);
        // this if tries to avoid the program hanging in one of these listener loops
        if( (millis() - timeStart) > RESET_TIME ) {
          return NULL;
        }
      } while(input == LOW);

      timeRisingEdge = millis();

      timeLow = timeRisingEdge - timeFallingEdge;
      
    } while(timeLow < MAX_NEXT_TIME);
    
  } while(timeLow < MAX_NEXT_TIME && i < 5);
  
  return convertMorseChar(morseChar);
}

char convertMorseChar(int morseChar[]){
  
  int i = 0;
  bool found = false;
  char foundChar = NULL;
  
  while(i<NUM_CHARS && found == false) {
    if(compareMorseChars(morseChar, morseDict[i]) == true){
      found = true;
      foundChar = alphanumDict[i];
    } else {
      i++;
    }
  }
  return foundChar;
}

bool compareMorseChars(int morseChar[], int morseDict[]) {
  if(morseChar[0] == morseDict[0] && morseChar[1] == morseDict[1] && morseChar[2] == morseDict[2] && morseChar[3] == morseDict[3] && morseChar[4] == morseDict[4]) {
    return true;    
  } else {
    return false;
  }
}
