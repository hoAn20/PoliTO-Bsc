/*
	Write a sketch to control the rotational speed of a DC motor using a PWM (Pulse-Width Modulation)
	capable pin. Starting from stationary condition the sketch controls the motor following user's
	input ('+' to increase speed, '-' to decrease speed) on a 10 steps scale between 0 and 255.

	DC-motor datasheet: https://wiki.dfrobot.com/Fan_Module_DFR0332
*/

const int CTRL_PIN = 11;
const float MAX_SPEED = 255;
const float MIN_SPEED = 0;
const float N_STEP = 10;

volatile float ctrl = 0;
volatile float step;

void setup()
{
	Serial.begin(9600);
	while (!Serial);
	Serial.println("Insert + to increase or - to decrease speed.")

	pinMode(CTRL_PIN, OUTPUT);
	analogWrite(CTRL_PIN, (int)ctrl);

	step = MAX_SPEED / N_STEP;
}

void loop()
{
	if (Serial.available() > 0)
	{
		volatile int byteRead = Serial.read();
		//Serial.read();

		if (byteRead == '+')
		{
			if (ctrl == MAX_SPEED)
			{
				Serial.println("Already at max speed.");
			}
			else
			{
				ctrl += step;
				analogWrite(CTRL_PIN, (int)ctrl);  // while pin 11 is digital it is also pwm
						// capable, using analogWrite for finer modulations
				Serial.print("(+) New speed: ");
				Serial.println(ctrl);
			}
		}
		else if (byteRead == '-')
		{
			if (ctrl == MIN_SPEED)
			{
				Serial.println("Already at minimum speed.");
			}
			else
			{
				ctrl -= step;
				analogWrite(CTRL_PIN, (int)ctrl);
				Serial.print("(-) New speed: ");
				Serial.println(ctrl);
			}
		}
		else
		{
			Serial.println("Incorrect char.");
		}
	}
}
