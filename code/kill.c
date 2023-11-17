#include <stdio.h>
#include <wiringPi.h>
#include <softPwm.h>
#include <math.h>

int main(){
	wiringPiSetupGpio();
	softPwmCreate(6, 0, 255);
	softPwmCreate(19, 0, 255);
	pinMode(5, OUTPUT);
	pinMode(13, OUTPUT);
	
	while(1){
		digitalWrite(5, HIGH);
		digitalWrite(13, HIGH);
		softPwmWrite(6, 0);
		softPwmWrite(19,0);
	}
	return 0;
}
