#include <pigpio.h>
#include <cstdio>
#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>   // For atomic boolean
#include <csignal>  // For signal handling
#include <fstream> // For input file stream

// Default configuration values
float desiredTemp = 40;     // Desired temperature to start the fan (Celsius)
float criticalTemp = 80;    // Critical temperature for full-speed fan or shutdown (Celsius)
int minFanSpeed = 30;       // Minimum fan speed in percentage

constexpr int fanPin = 24;
// Atomic flag to control the loop
std::atomic<bool> keepRunning(true);

using namespace std;
using namespace std::chrono;
using namespace std::this_thread;

float fanSpeed = 100;   // Fan speed initialization
float sum = 0;          // Sum for integral calculation
float pTemp = 15;       // Proportional factor
float iTemp = 0.4;      // Integral factor

// Function to turn off the fan
void fanOFF() {
	gpioWrite(fanPin, 0); // Turn off the fan
}

// Signal handler to catch Ctrl-C and clean up
void signalHandler(int signum) {
	std::cerr << "Interrupt signal (" << signum << ") received." << std::endl;
	keepRunning = false; // Set flag to false to exit loop
}

// Function to retrieve the CPU temperature
float getCPUtemperature() {
	FILE *fp;
	char tempStr[100];
	float temp;

	// Use vcgencmd to get the CPU temperature
	fp = popen("vcgencmd measure_temp", "r");
	if (fp == NULL) {
		perror("Failed to run command");
		exit(1);
	}

	fgets(tempStr, sizeof(tempStr) - 1, fp);
	sscanf(tempStr, "temp=%f", &temp);  // Extract the temperature from the output

	pclose(fp);
	cout << "CPU tem = " << temp << "ºC" << endl;
	return temp;
}

// Function to set the fan speed (PWM)
void setFanSpeed(float speed) {
	int pwmValue = static_cast<int>(speed * 255 / 100); // Convert fanSpeed (0-100%) to PWM value (0-255)

	if (speed == 0) {
		fanOFF(); // Fan off
	} else {
		gpioPWM(fanPin, pwmValue); // Set PWM speed
	}
}

// Function to control the fan speed based on CPU temperature
void handleFan(float actualTemp) {
	actualTemp;
	float diff = actualTemp - desiredTemp;
	sum += diff;

	float pDiff = diff * pTemp;
	float iDiff = sum * iTemp;

	fanSpeed = pDiff + iDiff;

	// Limit fanSpeed between 0 and 100, and ensure minimum operational speed
	if (fanSpeed > 100) fanSpeed = 100;
	if (fanSpeed < minFanSpeed && fanSpeed != 0) fanSpeed = minFanSpeed;

	// Limit sum for the integral part
	if (sum > 100) sum = 100;
	if (sum < -100) sum = -100;

	cerr << "actualTemp: " << actualTemp << ", TempDiff: " << diff << ", pDiff: " << pDiff << ", iDiff: " << iDiff << ", fanSpeed: " << fanSpeed << endl;

	setFanSpeed(fanSpeed);
}

// Function to handle critical temperatures
void handleCriticalTemp(float actualTemp) {
	if (actualTemp >= criticalTemp) {
		cerr << "Critical temperature reached:" << actualTemp << "ºC" << endl;
		setFanSpeed(100);  // Run fan at full speed
		// Optional: Shutdown the system to prevent damage
		// system("sudo shutdown -h now");
	}
}

// Function to load configuration from a file
void loadConfig() {
	const char* configPaths[] = { "./cpu_fan_control.conf", "/etc/cpu_fan_control.conf" };  // Array of paths to try
	bool configLoaded = false;

	for (const char* path : configPaths) {
		std::ifstream configFile(path);
		if (configFile) {
			std::cerr << "Loading configuration from " << path << std::endl;
			std::string line;
			while (std::getline(configFile, line)) {
				size_t delimiterPos = line.find('=');
				if (delimiterPos == std::string::npos) {
					continue;  // Skip invalid lines
				}

				std::string key = line.substr(0, delimiterPos);
				std::string value = line.substr(delimiterPos + 1);

				if (key == "desiredTemp") {
					desiredTemp = atof(value.c_str());
				} else if (key == "criticalTemp") {
					criticalTemp = atof(value.c_str());
				} else if (key == "minFanSpeed") {
					minFanSpeed = atoi(value.c_str());
				}
			}

			configFile.close();
			configLoaded = true;
			break;  // Stop once a valid config file is found
		}
	}

	if (!configLoaded) {
		std::cerr << "No config file found, using default values." << std::endl;
	}
}

int main(int argc, char const *argv[]) {

	// Load configuration from file or use defaults
	loadConfig();

	// WiringPi Setup
	if (gpioInitialise() < 0) {
		cerr << "WiringPi setup failed!" << endl;
		return 1;
	}

	// Register signal and signal handler
	std::signal(SIGINT, signalHandler);  // Catch Ctrl-C (SIGINT) /!\ NEED TO BE CALLED AFTER gpioInitialise()!

	// Set GPIO pin mode for the fan
	gpioSetMode(fanPin, PI_OUTPUT);

	cout << "Fan connected to GPIO pin" << fanPin << endl;

	float actualTemp = 0.0f;
	while (keepRunning) {
		actualTemp = getCPUtemperature();

		if (actualTemp > (desiredTemp + 2)) {
			handleFan(actualTemp);  // Turn fan on or increase speed
		} else if (actualTemp < (desiredTemp - 2)) {
			fanOFF();     // Turn fan off
		}

		handleCriticalTemp(actualTemp);  // Check for critical temperature

		sleep_for(milliseconds(5000));  // Event-driven periodic sleep
	}

	// Clean up and turn off the fan
	fanOFF();  // Turn fan off
	std::cout << "Fan turned off." << std::endl;

	gpioTerminate();  // Clean up the pigpio library
	return 0;
}