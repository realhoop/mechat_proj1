#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <math.h>

#define LOOPTIME 10 //millisec

#define ENCODERA 23 // encoder A pin no.
#define ENCODERB 18 // encoder B pin no.
#define ENC2REDGEAR 216 // gear ratio of motor

#define MOTOR1 13 // motor 1 pin no.
#define MOTOR2 5 // motor 2 pin no.
#define PWM1 6 // pwm 1 pin no.
#define PWM2 19 // pwm 2 pin no.

#define PGAIN 100 // 
#define IGAIN 1
#define DGAIN 2

#define SIGNALTIME 15000 // millisec, 15 seconds

int encA;
int encB;
int encoderPosition = 0;
float redGearPosition = 0;
float referencePosition = 5;
float errorPosition = 0;
float errorPositionBefore = 0;
unsigned int startTime;
unsigned int checkTime;
unsigned int checkTimeBefore;
float Kp,Ki,Kd;
float ITAE;


void funcEncoderA()
{
	
	encA = digitalRead(ENCODERA);
	encB = digitalRead(ENCODERB);
	if (encA == HIGH)
	{
		if (encB == LOW)
			encoderPosition++;
		else
			encoderPosition--;
	}
	else
	{
		if (encB == LOW)
			encoderPosition--;
		else
			encoderPosition++;
	}
	redGearPosition = (float)encoderPosition / ENC2REDGEAR;
	printf("funcENCODERA() A : %d B : %d encPos : %d gearPos : %f\n" , encA, encB, encoderPosition, redGearPosition);	
}

void funcEncoderB()
{
	
	encA = digitalRead(ENCODERA);
	encB = digitalRead(ENCODERB);
	if (encB == HIGH)
	{
		if (encA == LOW)
			encoderPosition--;
		else
			encoderPosition++;
	}
	else
	{
		if (encA == LOW)
			encoderPosition++;
		else
			encoderPosition--;
	}
	redGearPosition = (float)encoderPosition / ENC2REDGEAR;
	printf("funcENCODERB() A : %d B : %d encPos : %d gearPos : %f\n" , encA, encB, encoderPosition, redGearPosition);	
}



int main()
{
    printf("enter Kp: ");
    scanf("%f", &Kp);
    printf("enter Ki: ");
    scanf("%f", &Ki);
    printf("enter Kd: ");
    scanf("%f", &Kd);
    //printf("enter Reference Position: ");
    //scanf("%f", &referencePosition);
    
	//encoderPosition = 0;
	//redGearPosition = 0;
	//errorPosition = 0;
	//errorPositionBefore = 0;
	ITAE = 0;
	
	wiringPiSetupGpio();
	startTime = millis();
	checkTimeBefore = millis();
	softPwmCreate(PWM1, 0, 255);
    softPwmCreate(PWM2, 0, 255);
	pinMode(ENCODERA, INPUT);
	pinMode(ENCODERB, INPUT);
	wiringPiISR(ENCODERA, INT_EDGE_BOTH, funcEncoderA);
	wiringPiISR(ENCODERB, INT_EDGE_BOTH, funcEncoderB);
    digitalWrite(MOTOR1, HIGH);
    digitalWrite(MOTOR2, HIGH);

	
	while(checkTime - startTime <= SIGNALTIME)
	{
		checkTime = millis();
		
		if(checkTime-checkTimeBefore >= LOOPTIME) //LOOPTIME = 10
		{
            errorPosition = referencePosition - redGearPosition;
            float de = errorPosition - errorPositionBefore;
            float dt = checkTime - checkTimeBefore;
            float control = (float)Kp*errorPosition + (float)Ki*de*dt + (float)Kd*de/dt;
			if (errorPosition > 0)
			{
				printf("control: %f\n", control);
				softPwmWrite(PWM1, control);
				//softPwmWrite(PWM2, 0);
			}
			else
			{
				printf("control: %f\n", control);
				//softPwmWrite(PWM1, 0);
				softPwmWrite(PWM2, -1*control*100);

				
			}
			
			printf("referencePosition: %f | redGearPosition : %f | errorPosition: %f\n", referencePosition, redGearPosition, errorPosition);
			checkTimeBefore = checkTime;
            errorPositionBefore = errorPosition;
            ITAE += fabs(errorPosition)*checkTime*dt;
		}
		
	}
	printf("ITAE Score: %f", ITAE);
	
	
	return 0;
}
