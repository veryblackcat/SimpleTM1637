
#include <inttypes.h>
#include <Arduino.h>
#include <SimpleTM1637.h>

SimpleTM1637::SimpleTM1637(uint8_t clk, uint8_t dio)
{
	SimpleTM1637::clk = clk;
	SimpleTM1637::dio = dio;
	
	delay = TM1637_DELAY;
	brightness = 2;

    pinMode(clk, INPUT);
    pinMode(dio,INPUT);
	digitalWrite(clk, LOW);
	digitalWrite(dio, LOW);
}

void SimpleTM1637::setBrightness(uint8_t displayBrightness)
{
	if(displayBrightness > 7) displayBrightness = 7;
	brightness = displayBrightness | 0x08;
}

void SimpleTM1637::display(const uint8_t segments[])
{
	// Data command setting
	// Write data to display register, Automatic address adding, Normal mode
	start();
	sendByte(TM1637_DATA_COMMAND);
	stop();

	// Address command setting GRID1 (COH) - first digit (left)
	start();
	sendByte(TM1637_ADDR_COMMAND);

	// Send byte
	for (uint8_t i=0; i < 4; i++) sendByte(segments[i]);
	stop();

	// Display and control command setting - brightness, on/off
	start();
	sendByte(TM1637_CTRL_COMMAND | brightness);
	stop();
}

void SimpleTM1637::start()
{
  TM1637_DIO_LOW;
  delayMicroseconds(delay);
}

void SimpleTM1637::stop()
{
	TM1637_DIO_LOW;
	delayMicroseconds(delay);
	TM1637_CLK_HIGH;
	delayMicroseconds(delay);
	TM1637_DIO_HIGH;
	delayMicroseconds(delay);
}

uint8_t SimpleTM1637::sendByte(uint8_t data)
{
  for(uint8_t i = 0; i < 8; i++) {
	TM1637_CLK_LOW;
	delayMicroseconds(delay);

    if (data & 0x01)
      TM1637_DIO_HIGH;
    else
      TM1637_DIO_LOW;

	TM1637_CLK_HIGH;
	delayMicroseconds(delay);
	data >>= 1;
  }

  // ACK
  TM1637_DIO_HIGH;
  TM1637_CLK_LOW;
  delayMicroseconds(delay);
  TM1637_CLK_HIGH;
  delayMicroseconds(delay);
  uint8_t ack = digitalRead(dio);
  TM1637_CLK_LOW;
  delayMicroseconds(delay);

  return ack;
}


