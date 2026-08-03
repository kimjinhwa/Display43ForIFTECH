
#include <Arduino.h>
#include <unity.h>

#define GPIO17 17  /* 17 */
#define GPIO18 18  /* 18 */
#define RX 19  /* 18 */
#define TX 20  /* 17*/

//HardwareSerial mySerial(1);

uint16_t  TESTPORT  ; 
void setUp(void)
{
  // set stuff up here
}

void tearDown(void)
{
  // clean stuff up here
}


void test_state_high(void)
{
  digitalWrite(TESTPORT,HIGH);
  TEST_ASSERT_EQUAL(HIGH, digitalRead(TESTPORT));
}
void test_state_low(void)
{
  digitalWrite(TESTPORT,LOW);
  TEST_ASSERT_EQUAL(LOW, digitalRead(TESTPORT));
}
void test_serial_WriteAndRead(void){
  int c=0x55;
  c = Serial2.write(c);
  delay(1);
  if(Serial2.available())
    c = Serial2.read();
  delay(1);
  TEST_ASSERT_EQUAL(0x55, c);
}

void setup()
{
  // NOTE!!! Wait for >2 secs
  // if board doesn't support software reset via Serial.DTR/RTS
  // pinMode(TX,OUTPUT | PULLUP);
  // pinMode(RX,OUTPUT);
  pinMode(GPIO17,OUTPUT);
  pinMode(GPIO18,OUTPUT);
  Serial2.begin(57600,SERIAL_8N1,RX, TX);
  delay(1000);

  UNITY_BEGIN(); // IMPORTANT LINE!
  delay(10);
}

uint8_t i = 0;
uint8_t max_blinks = 100;

void loop()
{
  if (i < max_blinks)
  {
    //RUN_TEST(test_serial_WriteAndRead);
    TESTPORT = GPIO17;
    RUN_TEST(test_state_high);
    delay(1000);
    RUN_TEST(test_state_low);
    delay(1000);
    // delay(500);
    TESTPORT = GPIO18;
    RUN_TEST(test_state_high);
    delay(1000);
    RUN_TEST(test_state_low);
    delay(1000);
    i++;
  }
  else if (i == max_blinks)
  {
    UNITY_END(); // stop unit testing
  }
}