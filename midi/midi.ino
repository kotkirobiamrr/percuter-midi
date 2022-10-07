// written in arduino ide, programmed by arduino isp on attiny 2313

// pin order for arduino, these are not phisical pins numbers
// (see: https://github.com/SpenceKonde/ATTinyCore/blob/master/avr/extras/ATtiny_x313.md )


#define S_WAITING 0
#define S_WAIT_FOR_SECOND_BYTE 1
#define S_WAIT_FOR_THIRD_BYTE 2
#define S_DONTCARE 3
#define S_FINISH 4
#define S_TEST 255

#define midi Serial



const byte triggerPin[8] = {9,10,11,12,13,14,15,16};  //pins connected to triggers
const byte midichPin[4] = {4,5,6,7};  //pins used to select midi channel

byte midich;

byte commandByte;
byte noteByte;
byte velocityByte;

byte times[8];
bool timesx;
byte x;

byte state;

byte prevstate =  9;
void setup(){


  midi.begin(31250);

  midich = 0;

  for (x = 0; x < 8; x++)
  {
    pinMode(triggerPin[x], OUTPUT);
    pinMode(midichPin[x%4], INPUT_PULLUP);  //only one loop with strange commands - for memory saving
    midich = midich << 1 | !digitalRead(midichPin[x%4]);  //set midi channel (two times)
    digitalWrite(triggerPin[x], 0);
    times[x] = 255;
  }
  midich &= 15; //set unnecessary bits to 0, so we have actually value from 0 to 15, our midi channel

  state = S_WAITING;

}


void loop(){

  timesx = !timesx;

  if (timesx)
  {
  //count time to set trigger pulse to 0
    for (x = 0; x < 8; x++)
    {
        times[x]--;
        if (times[x] == 0)
        {
          digitalWrite (triggerPin[x], 0);
          times[x] = 255;
        }
      }
  }

////////////////////////////////////////////////////////
  
  if (midi.available())
  {
    if (midi.peek() == 248)
    {
      midi.read();
    }
    else
    {
      switch (state)
      {
        case S_WAITING:
         
          commandByte = midi.read();
          if ((commandByte & 15) == midich)
          {
            switch (commandByte & 240)
            {
              case 144:
              case 128:
                state = S_WAIT_FOR_SECOND_BYTE;
                break;
              default:
                state = S_DONTCARE;
            }
          }
          else
          {
            state = S_DONTCARE;
          }
          break;
  
        case S_WAIT_FOR_SECOND_BYTE:
          noteByte = midi.read() & 7;
          state = S_WAIT_FOR_THIRD_BYTE;
          break;
  
        case S_WAIT_FOR_THIRD_BYTE:
          velocityByte = midi.read();
  
          switch (commandByte & 240)
          {
            case 144: //note on
              if (velocityByte > 4) //velocity values less than 4 are not played
              {
                digitalWrite (triggerPin[noteByte], 1);
                times[noteByte] = velocityByte / 2;
                // the less velocity value, the shorter pulse that results in quieter sound
              }
              else
              {
                digitalWrite (triggerPin[noteByte], 0);
              }
              break;
            case 128: //note off
              digitalWrite (triggerPin[noteByte], 0);
              break;
          };
            state = S_FINISH;
          break;
  
        case S_FINISH:
          if (midi.peek() < 128)
            state = S_WAIT_FOR_SECOND_BYTE;
          else
            state = S_WAITING;
          break;
        case S_DONTCARE:
          if (midi.peek() < 128)
            midi.read();
          else
            state = S_WAITING;
          break;
      };
    }
  }

}
