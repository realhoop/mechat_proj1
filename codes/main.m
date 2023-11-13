rpi = raspi('192.168.1.100', 'pi', 'raspberry');
encoderPin = configureDigitalPin(rpi, 18, 'input');
motorPin = configureDigitalPin(rpi, 12, 'PWM');

desiredPosition = 1000;  % Replace with your desired position
Kp = 1; Ki = 1; Kd = 1;  % Replace with your PID constants

pidController = pid(Kp, Ki, Kd);

lastPosition = 0;
while true
    currentPosition = readDigitalPin(rpi, encoderPin);
    error = desiredPosition - currentPosition;
    controlSignal = pidController(error);
    
    % Limit control signal to between 0 and 1
    controlSignal = max(0, min(1, controlSignal));
    
    writePWMVoltage(rpi, motorPin, controlSignal);
    
    pause(0.01);  % Delay to avoid overwhelming the Raspberry Pi
end