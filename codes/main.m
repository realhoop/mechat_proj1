rpi = raspi('192.168.1.100', 'pi', 'raspberry');
encoderPin = configureDigitalPin(rpi, 18, 'input');
motorPin = configureDigitalPin(rpi, 12, 'PWM');

desiredPosition = 1000;  % Replace with your desired position
Kp = 1; Ki = 1; Kd = 1;  % Replace with your PID constants

pidController = pid(Kp, Ki, Kd);

% Prepare figure for plotting
hFig = figure('Name', 'Motor Position');
hPlot = plot(nan, nan);
ylabel('Position');
xlabel('Time (s)');
grid on;

% Variables for time and position
posData = [];
timeData = [];
startTime = tic;  % Start a timer for the x-axis of the plot

while ishandle(hFig)  % Run until the plot window is closed
    currentPosition = readDigitalPin(rpi, encoderPin);
    error = desiredPosition - currentPosition;
    controlSignal = pidController(error);

    % Limit control signal to between 0 and 1
    controlSignal = max(0, min(1, controlSignal));

    writePWMVoltage(rpi, motorPin, controlSignal);
    
    % Store data for plotting
    timeData(end+1) = toc(startTime);
    posData(end+1) = currentPosition;
    
    % Update plot
    set(hPlot, 'XData', timeData, 'YData', posData);
    xlim([max(0, timeData(end)-10), timeData(end)+0.1]);  % Show only the last 10 seconds of data

    pause(0.01);  % Delay to avoid overwhelming the Raspberry Pi
end
