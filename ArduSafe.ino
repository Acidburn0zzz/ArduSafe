/*
 *  Unlicenced
 */

#include <Keypad.h>
#include <Password.h>

const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
	{'1', '2', '3'},
	{'4', '5', '6'},
	{'7', '8', '9'},
	{'*', '0', '#'}
};

byte rowPins[ROWS] = { 13, 4, 6, 8 };
byte colPins[COLS] = { 2, 5, 7 };

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

const int green = 9;
const int red = 10;
const int orange = 11;
const int buzzer = 3;
const int solenoid = 12;

Password password = Password("4321");

int failCount = 0;

void setup(void)
{
	pinMode(green, OUTPUT);	// Green LED (tick)
	pinMode(red, OUTPUT);	// Red LED ('X')
	pinMode(orange, OUTPUT);	// Orange LED ('C')
	pinMode(buzzer, OUTPUT);	// Piezo buzzer
	pinMode(solenoid, OUTPUT);	// Lock solenoid

	keypad.addEventListener(keypadEvent);
}

void loop()
{
	while (failCount < 3) {
		keypad.getKey();
	}

	delay(10000);
	failCount = 0;
}

void keypadEvent(KeypadEvent keyPress)
{
	switch (keypad.getState()) {
	case PRESSED:
		switch (keyPress) {

		case '*':
			clearPassword();
			break;
		case '#':
			checkPassword();
			break;
		default:
			beep();
			password.append(keyPress);
		}
	}
}

void checkPassword()
{
	if (password.evaluate()) {
		password.reset();
		failCount = 0;
		success();
	} else {
		password.reset();
		failCount++;
		error();
	}
}

void clearPassword()
{
	password.reset();

	digitalWrite(red, HIGH);

	tone(buzzer, 440, 100);
	delay(150);
	tone(buzzer, 220, 100);
	delay(150);

	digitalWrite(red, LOW);
}

void beep()
{
	digitalWrite(orange, HIGH);

	tone(buzzer, 440, 200);
	delay(200);

	digitalWrite(orange, LOW);
}

void success()
{
	digitalWrite(green, HIGH);
	digitalWrite(solenoid, HIGH);

	tone(buzzer, 880, 1000);
	delay(1000);

	digitalWrite(green, LOW);
	digitalWrite(solenoid, LOW);
}

void error()
{
	digitalWrite(red, HIGH);

	for (int i = 0; i <= 2; i++) {
		tone(buzzer, 784, 100);
		delay(150);
	}

	digitalWrite(red, LOW);
}
