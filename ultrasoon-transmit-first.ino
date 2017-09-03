//pin en led aangepast https://www.pjrc.com/teensy/td_libs_VirtualWire.html

#include <VirtualWire.h>
#include <NewPing.h>

byte afstand;

const int led_pin = 13;
const int transmit_pin = 12;
//const int receive_pin = 2;
//const int transmit_en_pin = 3;

#define TRIGGER_PIN  10  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 220 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

void setup()
{
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.
  
  // Initialise the IO and ISR
  vw_set_tx_pin(transmit_pin);
  //vw_set_rx_pin(receive_pin);
  //vw_set_ptt_pin(transmit_en_pin);
  //vw_set_ptt_inverted(true); // Required for DR3100
  vw_setup(2000);  // Bits per sec

}

byte count = 1;

void loop()
{
  //ultasoon sensor code
  delay(50);                     // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings.
  if (sonar.ping_cm()>1)
    {Serial.print("Ping: ");
    Serial.print(sonar.ping_cm()); // Send ping, get distance in cm and print result (0 = outside set distance range)
    Serial.println(" cm");
    
    if (sonar.ping_cm()>100)
      {
      char msg[7] = {'S','t','o','p','!',' ','#'};
      afstand = (byte) sonar.ping_cm();
      Serial.print (afstand);
      msg[6] = afstand;
      digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
      vw_send((uint8_t *)msg, 7);
      vw_wait_tx(); // Wait until the whole message is gone
      digitalWrite(led_pin, LOW);
      delay(500);
      }

    if (sonar.ping_cm()<50 && sonar.ping_cm()>2 )
      {
      char msg[7] = {'S','t','a','r','t',' ','#'};
      afstand = (byte) sonar.ping_cm();
      Serial.print (afstand);
      msg[6] = afstand;
      digitalWrite(led_pin, HIGH); // Flash a light to show transmitting
      vw_send((uint8_t *)msg, 7);
      vw_wait_tx(); // Wait until the whole message is gone
      digitalWrite(led_pin, LOW);
      delay(500);
      }

    }  

}

