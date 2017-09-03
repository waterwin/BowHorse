/*

Standalone Arduino StopWatch

Modified By ESE - 24/7/2017 9-1-2017 mega256 / ultrasoon via virtualwire

*/

// call the necessary libraries

#include <VirtualWire.h>
#include <LiquidCrystal.h>

// variables used in more than 1 function need to be declared here

// these are the pins used on the shield for this sketch
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

const int receive_pin = 11; //44 voor Mega / 11 voor Uno
int lcd_key     = 0;
int adc_key_in  = 0;

char cad[100];
int pos = 0;

unsigned long time;
unsigned long endtime;
unsigned long start, finished, elapsed;
boolean r = false; //om uit de doorteltijd if te ontsnappen

// Variables for button debounce time
long lastButtonPressTime = 0; // the last time the button was pressed or a new command was received
long debounceDelay = 50; // the debounce time; keep this as low as possible

byte buf[VW_MAX_MESSAGE_LEN]; 
byte buflen = VW_MAX_MESSAGE_LEN;

void setup()
{
lcd.begin(16, 2); // initialize the lcd (16 chars, 2 lines)

// a little introduction :)
lcd.setCursor(4, 0); // set the cursor on line 1
lcd.print("Arduino");
lcd.setCursor(3, 1); // set the cursor to 4th character on line 2
lcd.print("StopWatch");
delay(2000); // wait 2 seconds
lcd.clear(); // clear the display
lcd.print("Press select for");
lcd.setCursor(0, 1); // set the cursor to first character on line 2
lcd.print("Start & Stop");
delay(2000);

//also define the mega256 pushbuttons as mock up for the remote laser switches
//  pinMode(22, INPUT);
//  digitalWrite(22, HIGH);
//  pinMode(24, INPUT);
//  digitalWrite(24, HIGH);

// Initialise the IO and ISR voor receive
//vw_set_tx_pin(transmit_pin);
vw_set_rx_pin(receive_pin);
//vw_set_ptt_pin(transmit_en_pin);
//vw_set_ptt_inverted(true); // Required for DR3100lcd.begin(16, 2); 
vw_setup(2000); 
vw_rx_start();
}

void loop()
{
  CheckStartStop(); //could be a switchable variant for timekeeping by hand, based on a boolean listening to a button at start of program
  ReceiveSignal();
  DisplayResult();
}

void CheckStartStop()
{
  //int x = analogRead (0); // assign 'x' to the Arduino's AnalogueInputs (Shield's buttons), calibrate the output per button with the small .ino
  if ((char)buf[2] == 'a' ) // if the object is close to the ultrasoon sensor
  {
    if ((millis() - lastButtonPressTime) > debounceDelay)
    {
        if (r == false)
        {
        start = millis(); // saves start time to calculate the elapsed time
            lcd.clear();
            lcd.setCursor(7, 1); // needed
            lcd.print("Time Past");
        }
        else if (r == true) // knows it is endtime
          {
            lcd.setCursor(7, 1); // needed
            lcd.print("End Time");
          }
        r = !r;
      }
  lastButtonPressTime = millis();
  }
}

void ReceiveSignal()
{

//lcd.setCursor(14,1); // set the LCD cursor position 
//lcd.print("Rx");     // print a simple message on the LCD to show we are receiving

int i;

if( vw_get_message(buf, &buflen) )
{
  if (r == true)
  
  {
  if(pos < 2)
  lcd.setCursor(0, pos);
  else
    {
      pos=0;
      lcd.clear();
    }
 
  lcd.setCursor(0, 1);
  for (i = 0; i < buflen; i++)
    {
      lcd.print((char)buf[i]);
      //lcd.print(buf[i]);
      pos++;
    }
  }
}
/* de ontvangen input om de timer te starten
    if ((char)buf[2] == 'a')
        {
        lcd.setCursor(0, 1);
        lcd.print ("A ");      //hier komt de code om de timer te starten
        lcd.print("Time: ");
        time = millis();
        endtime = time;
        //prints time since program started
        lcd.print(time);
        // wait a second so as not to send massive amounts of data
        delay(1000);
        }

    if ((char)buf[2] == 'o')
        {lcd.setCursor(0, 1);
        lcd.print ("O ");      //hier komt de code om de timer te stoppen
        lcd.print("EndTime: ");
        //prints time since program started
        lcd.print(endtime);
        // wait a second so as not to send massive amounts of data
        delay(1000);
        }
*/
  }



void DisplayResult()
  {
    if (r == true)
      {
        finished = millis(); // saves stop time to calculate the elapsed time
        // declare variables
        float h, m, s, ms;
        unsigned long over;

        // MATH time!!!
        elapsed = finished - start;

        h = int(elapsed / 3600000);
        over = elapsed % 3600000;
        m = int(over / 60000);
        over = over % 60000;
        s = int(over / 1000);
        ms = over % 1000;
        // display the results
        lcd.setCursor(0, 0);   //naar 1e regel over de tekst Press select for
        lcd.print(h, 0); // display variable 'h' - the 0 after it is the number of decimals after a comma (ex: lcd.print(h, 2); would print 0,00
        lcd.print("h "); // and the letter 'h' after it
        lcd.print(m, 0);
        lcd.print("m ");
        lcd.print(s, 0);
        lcd.print("s ");
        if (h < 10)
          {
            lcd.print(ms, 0);
            lcd.print("ms ");
          }
      }
  }
