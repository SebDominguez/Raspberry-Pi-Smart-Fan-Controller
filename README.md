# Raspberry Pi CPU Fan PID Controller

A PID controller for managing a Raspberry Pi CPU fan based on real-time temperature readings. This project efficiently adjusts fan speed to maintain optimal CPU temperatures, enhancing performance and longevity. Features include customizable temperature thresholds and graceful signal handling.

## Features

- **Real-time Temperature Monitoring**: Continuously reads CPU temperature using `vcgencmd`.
- **PID Control Algorithm**: Smoothly adjusts fan speed to maintain desired temperature.
- **Customizable Temperature Thresholds**: Easily modify desired and critical temperature settings.
- **User-friendly Interface**: Provides real-time feedback on CPU temperature and fan status.
- **Signal Handling**: Gracefully turns off the fan and cleans up resources on exit.

## Installation

1. **Clone the repository**:
   ```bash
   git clone https://github.com/SebDominguez/Raspberry-Pi-Smart-Fan-Controller.git
   cd Raspberry-Pi-Smart-Fan-Controller
   ```

2. **Install the necessary library**:
   Ensure you have `pigpio` installed:
   ```bash
   sudo apt update
   sudo apt install pigpio
   ```

3. **Compile the code**:
   Use the provided `Makefile`:
   ```bash
   make
   ```

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

You can adjust the desired temperature and critical temperature in the source code by modifying the respective constants.

## Contributing

Contributions are welcome! Please feel free to submit a pull request or open an issue.

## License

When I wrote this, only God and I understood what I was doing.
Now, only God knows. Therefore, this project is licensed under [GLWTPL](./LICENSE)

## Acknowledgments

- [pigpio](http://abyz.me.uk/rpi/pigpio/index.html) for GPIO control
- Raspberry Pi Foundation for their incredible hardware
