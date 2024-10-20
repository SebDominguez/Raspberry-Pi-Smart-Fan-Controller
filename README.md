# Raspberry Pi CPU Fan PID Controller

A PID controller for managing a Raspberry Pi CPU fan based on real-time temperature readings. This project efficiently adjusts fan speed to maintain optimal CPU temperatures, enhancing performance and longevity. Features include customizable temperature thresholds and graceful signal handling.

## Features

- **Real-time Temperature Monitoring**: Continuously reads CPU temperature using `vcgencmd`.
- **PID Control Algorithm**: Smoothly adjusts fan speed to maintain desired temperature.
- **Customizable Temperature Thresholds**: Easily modify desired and critical temperature settings.
- **User-friendly Interface**: Provides real-time feedback on CPU temperature and fan status.
- **Signal Handling**: Gracefully turns off the fan and cleans up resources on exit.

## Installation

0. **Clone the repository**:
   ```bash
   git clone https://github.com/SebDominguez/Raspberry-Pi-Smart-Fan-Controller.git
   cd Raspberry-Pi-Smart-Fan-Controller
   ```

1. **Install the necessary library**:
   Ensure you have `pigpio` installed:
   ```bash
   sudo apt update
   sudo apt install pigpio
   ```

2. **Compile the code**:
   Use the provided `Makefile`:
   ```bash
   make
   ```
3. **Copy to `/usr/local/bin` and set correct ownership and permissions**
   ```bash
   sudo cp cpu_fan_control /usr/local/bin/
   sudo chown root:root /usr/local/bin/cpu_fan_control
   sudo chmod 755 /usr/local/bin/cpu_fan_control
   ```
   now you can type in your terminal the `cpu_fan_control` as a command to start the application

## Usage

1. Start the `pigpiod` daemon (maybe optional):
   ```bash
   sudo pigpiod
   ```

2. Run the fan control program:
   ```bash
   sudo ./cpu_fan_control
   ```

3. Use `Ctrl+C` to stop the program safely.

## Configuration

You can adjust the desired temperature and critical temperature using a configuration file. The application will look for a configuration file named `cpu_fan_control.conf` in the following locations, in order:

1. **Current Directory**: Place the `cpu_fan_control.conf` file in the same directory from which you run the application.
2. **System Directory**: Alternatively, you can place the `cpu_fan_control.conf` file in `/etc/`.

You can copy the provided `cpu_fan_control.conf` inside your `/etc/` folder and set the correct permissions using:
```bash
sudo cp cpu_fan_control.conf /etc/
sudo chown root:root /etc/cpu_fan_control.conf
sudo chmod 644 /etc/cpu_fan_control.conf
```

### Configuration File Format

The configuration file should contain the following entries:

```
desiredTemp=<desired_temperature_in_Celsius>
criticalTemp=<critical_temperature_in_Celsius>
minFanSpeed=<minimum_fan_speed_percentage>
```

If no configuration file is found, the application will use the default values defined in the source code.

## Running at Startup

To make the application run automatically at startup, you can use the systemd service file provided.

0. **Copy the systemd service into:** `/etc/systemd/system/`
   ```bash
   sudo cp cpu_fan_control.service /etc/systemd/system/
   ```

1. **Set the Correct Permissions**:
   Ensure that the service file has the correct permissions:

   ```bash
   sudo chmod 644 /etc/systemd/system/cpu_fan_control.service
   ```

2. **Enable the Service**:
   Enable the service to start at boot with the following command:

   ```bash
   sudo systemctl enable cpu_fan_control.service
   ```

3. **Start the Service**:
   Start the service immediately without needing to reboot:

   ```bash
   sudo systemctl start cpu_fan_control.service
   ```

4. **Check the Service Status**:
   You can check the status of the service to ensure it is running properly:

   ```bash
   sudo systemctl status cpu_fan_control.service
   ```

This setup allows your CPU fan control application to run automatically at startup, ensuring optimal cooling for your system.


## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue.

## License

This project is licensed under [GLWTSPL](./LICENSE)

## Acknowledgments

- [pigpio](http://abyz.me.uk/rpi/pigpio/index.html) for GPIO control
- Raspberry Pi Foundation for their incredible hardware
