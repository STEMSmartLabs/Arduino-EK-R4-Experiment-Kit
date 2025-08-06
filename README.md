# Arduino R4 Minima Experiment Kit 
_Discover the Inventor in You_

![Made in India](https://img.shields.io/badge/Origin-Made%20in%20India-orange)

![Arduino EK R4 Experiment Kit](https://shop.stemsmartlabs.com/wp-content/uploads/2025/07/ArduinoEk_IMage.png)

 
## Overview:

The **Arduino R4 Minima Experiment Kit** is a fully-loaded, classroom-ready development board designed to help students (and hobbyists) learn the fundamentals of electronics, programming, and mechatronics. It combines the power of the Arduino EK R4 Minima with onboard sensors, actuators, an OLED display, NeoPixel ring, and four Grove-compatible external sensor ports—everything you need to prototype 50+ hands-on projects.


## Demo Program:

**A built-in demo program runs automatically on the kit, showcasing the functionality of each module.**

You can navigate through the different demo examples using the **arrow keys** on the **IR remote**, and select a demo by pressing the **OK button**.

---

## Setting Up Arduino EK R4 Experiment Kit 

**Install the Arduino EK R4 Experiment Kit Library and Dependencies**

Before starting with your projects, make sure you have the **Arduino IDE** installed and set up properly.

## Install Arduino IDE

If you don’t already have the Arduino IDE:

1. Download it from the official site: <https://www.arduino.cc/en/software>
2. Follow the installation instructions for your platform (Windows/macOS/Linux)

Need help? Watch this step-by-step video:  
[How to Install Arduino IDE](https://www.youtube.com/watch?v=7h_KRz9gVKc&list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG&index=1&pp=iAQB)

## Install the Arduino EK R4 Experiment Kit (ZIP)

To get started with the projects, you need to install the custom library provided in our GitHub repository. Follow the steps below:

## Download the Library

1. Visit our GitHub repository:  
    <https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit>
2. Click on the green **“Code”** button and select **“Download ZIP”**.

## Add the Library to Arduino IDE

1. Open the **Arduino IDE**.
2. Go to **Sketch** → **Include Library** → **Add .ZIP Library…**
3. Select the ZIP file you just downloaded.

This will install all necessary examples, demo and module code for using the AI & IoT Kit.

## Install Required Libraries via Library Manager

Open Arduino IDE and go to:  
**Sketch** → **Include Library** → **Manage Libraries…**

In the Library Manager, search  and install the latest version of the following libraries:

| **Library Name** | **Notes** |
| --- | --- |
| **Adafruit SSD1306** | For OLED Display |
| **IRremote** | For IR sensor/remote handling |
| **Servo** | For controlling servo motor |
| **Adafruit NeoPixel** | For RGB LED animations |
| **DHT sensor library (Kxn)** | For temperature & humidity sensor |
| **RTClib** | For real-time clock module |

Need help? Watch this step-by-step video:  [How to install Arduino EK R4 Experiment Kit library](https://youtu.be/NynkjxE_f0E)  

## Kit Modules – Examples & Video Tutorials

Each module in the **Arduino EK R4 Experiment Kit** is supported by a dedicated .ino example file and a video tutorial to help you understand its functionality and how to use it in real-world applications.

You can access all module examples directly from the Arduino IDE:  
**File** → **Examples** → **Arduino EK R4 Experiment Kit** → **examples** → **sensors**

Click on the video link next to each module in the table below to watch a step-by-step demonstration.


| Module Name | Description | Module Code | Video Link |
| :-- | :-- | :-- | :-- |
| Servo | Moves servo motor to desired angles | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/Servo/Servo.ino) | [Watch Video](https://youtu.be/mvdJesc4Bp0?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |
| NeoPixel | Displays colorful LED animations | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/NeoPixel/NeoPixel.ino) | [Watch Video](https://youtu.be/aflbNHTgnTw?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |
| Fan | Controls DC fan speed or on/off state | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/Fan/Fan.ino) | [Watch Video](https://youtu.be/-V036wOBVBQ?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |
| DHT | Reads temperature and humidity values | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/DHT/DHT.ino) | [Watch Video](https://youtu.be/yxTLp2EgEgY?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |
| LED | Blinks and controls external LEDs | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/LED/LED.ino) | [Watch Video](https://youtu.be/9pjA6bSlIlE?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |
| IR Remote | Receives signals from an IR remote | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/IR/IR.ino) | [Watch Video](https://youtu.be/oqgKEzpTiyU?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |
| OLED | Displays text and graphics on screen | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/OLED/OLED.ino) | [Watch Video](https://youtu.be/qHU3qu7oAQo?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |
| LDR | Detects ambient light intensity | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/LDR/LDR.ino) | [Watch Video](https://youtu.be/DFxrTkIoJ8g?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |
| Ultrasonic | Measures distance using sound waves | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/Ultrasonic/Ultrasonic.ino) | [Watch Video](https://youtu.be/QnRZb2g8Dv8?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |
| Buzzer | Produces tones and sound alerts | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/Buzzer/Buzzer.ino) | [Watch Video](https://youtu.be/gbFjNumvReE?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |
| Button | Detects push button press | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/Button/Button.ino) | [Watch Video](https://youtu.be/XIhvD9RvUtk?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |
| Switch | Reads toggle switch on/off state | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/Switch/Switch.ino) | [Watch Video](https://youtu.be/XIhvD9RvUtk?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |
| Trimpot | Reads analog value from potentiometer | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/sensors/TrimPOT/TrimPOT.ino) | [Watch Video](https://youtu.be/jsoSBXgQ93w?list=PL_NAQ0cj_VWI-FwRrFVTtYNqDVCF2LvEG) |

## External Grove Ports:

The **Arduino EK R4 Experiment Kit** offers **External Grove Ports** that make it easy to connect additional sensors and modules. We also support breadboarding via external port. This enables users to go beyond the pre-mounted components and experiment with new ideas, custom circuits, or external I2C devices.

## What You Can Do:

- Connect sensors included in the main kit
- Prototype with breadboards while using Grove connectors
- Access **I2C communication** through the Grove expansion ports
- Extend functionality using jumper wires and custom circuits

## Breadboarding External Sensors

Breadboarding allows you to connect sensors that are **not Grove-compatible** or **not included in the kit**. Use jumper wires to easily integrate these external components with your microcontroller for extended functionality.

### Example Sensors (**not included in the kit**)

| Sensor                | Description               |
|-----------------------|---------------------------|
| **PIR Motion Sensor** | Detects human movement    |
| **IR Obstacle Sensor**| Detects nearby objects    |
| **Gas Sensor (MQ-2)** | Detects smoke or gas      |
| **Flame Sensor**      | Detects fire or flame     |
| **Soil Moisture Sensor** | Measures soil wetness |
| **Ultrasonic Sensor** | Measures distance with sound |

> 💡 Use the breadboard for prototyping and testing these components before creating permanent circuits.


## Real-World Applications & Projects

Explore exciting hands-on projects categorized by real-world domains. Each project comes with working .ino files, and some include video tutorials for deeper understanding.

**Home Automation**

Projects that automate your living space using sensors and actuators.  
Build smart fans, curtain systems, door locks, and appliance controllers using IR remotes and sensors.

**Interactive Games**

Fun, interactive ways to learn electronics and logic.  
Use buttons, sensors, and OLEDs to build games that respond to user interaction.

**Assistive Technology**

Create solutions that assist the differently-abled or elderly.  
Automate doors, lights, or devices using simple input methods to support independent and safe living.

**Clocks and Scientific Tools**

Build tools inspired by lab setups and scientific instruments.  
Projects include timers, counters, and measurement displays to understand physics and data logging.

**Environmental Monitoring**

Track changes in your surroundings using smart sensors.  
Build systems that detect temperature, humidity, and light to trigger automatic lighting or alert systems.

**Interactive Display**

Projects that use the OLED display for real-time visual feedback.  
Display sensor values, animations, messages, or system status to bring projects to life.

**You can access all application examples directly from the Arduino IDE:
File → Examples → Arduino EK R4 Experiment Kit → examples → applications**
## Featured Project Videos

| **Project Title** | **Description / Highlights** | **Project Code** | **Video Link** |
| --- | --- | --- | --- |
| **Tic Tac Toe Game on Arduino EK R4** | Playable Tic Tac Toe using IR Remote & OLED Display | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/blob/main/examples/applications/Games/tic_tac_toe/tic_tac_toe.ino) | [Watch Video](https://youtu.be/4XrE3T21CvA?list=PL_NAQ0cj_VWJGEcv7IAwLWgFyA2s_rXrA) |
| **Arcade Cyclone Game with Arduino EK R4** | Fast-reaction LED ring game with NeoPixel + Buzzer | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/tree/main/examples/applications/Games/arcade_cyclone) | [Watch Video](https://youtu.be/DR1ks8xgdQE?list=PL_NAQ0cj_VWJGEcv7IAwLWgFyA2s_rXrA) |
| **Smart Parking Assist System with Arduino EK R4** | Ultrasonic sensor + NeoPixel + Buzzer to guide parking | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/blob/main/examples/applications/Assistive%20Technology/parking_assist/parking_assist.ino) | [Watch Video](https://youtu.be/9raalE5PwAM?list=PL_NAQ0cj_VWJGEcv7IAwLWgFyA2s_rXrA) |
| **Musical Keyboard with IR Remote** | IR remote as keyboard, buzzer for tones | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/blob/main/examples/applications/Games/IR_keyboard/IR_keyboard.ino) | [Watch Video](https://youtu.be/FPQu9rpY4O8?list=PL_NAQ0cj_VWJGEcv7IAwLWgFyA2s_rXrA) |
| **Tile Tower Game with Arduino EK R4** | OLED-based stacking game testing speed & timing | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/blob/main/examples/applications/Games/tile_tower/tile_tower.ino) | [Watch Video](https://youtu.be/5Y-gn9RKH18?list=PL_NAQ0cj_VWJGEcv7IAwLWgFyA2s_rXrA) |
| **Smart Lock System using IR Remote, OLED, Servo, and Buzzer** | IR-based access control system with feedback mechanisms | [View Code](https://github.com/STEMSmartLabs/Arduino-EK-R4-Experiment-Kit/blob/main/examples/applications/Home%20Automation/IR_remote_lock/IR_remote_lock.ino) | [Watch Video](https://youtu.be/CuWeehWU5jA?list=PL_NAQ0cj_VWJGEcv7IAwLWgFyA2s_rXrA) |


---

## License

This project is released under the MIT License.



## Contact

Made in India by **STEM Smart Labs**  
- 🌐 https://stemsmartlabs.com  
- ✉️ hello@stemsmartlabs.com  
