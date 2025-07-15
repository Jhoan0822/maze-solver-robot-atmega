# Autonomous Maze-Solving Robot "OrbitALL"

![Platform](https://img.shields.io/badge/Platform-Atmel%20AVR-orange?style=for-the-badge&logo=atmel)
![Language](https://img.shields.io/badge/Language-C-blue?style=for-the-badge&logo=c)
![License](https://img.shields.io/badge/License-MIT-green?style=for-the-badge)

An autonomous mobile robot designed, built, and programmed from the ground up to navigate and solve line-based mazes. This project was developed for the Microprocessors and Assembly course at Universidad del Valle and represents a comprehensive application of mechatronics principles, covering system design, embedded C programming, control theory, and algorithmic problem-solving.

**Team Members:**
*   Jhoan Eduardo Saldarriaga Serna
*   Alejandro Muñoz Gutiérrez
*   Henry Daniel Arboleda Correa

---

### ► Project Overview

OrbitALL is an intelligent two-wheeled robot engineered to operate in an unknown maze environment. Its core mission is executed in two distinct phases:

1.  **Exploration & Mapping:** On its initial run, the robot uses a **Depth-First Search (DFS)** strategy to traverse every path of the maze. During this phase, it follows the line, identifies all intersections (nodes), and builds a complete graph representation of the maze's topology in its memory.
2.  **Optimization & Execution:** Once the map is stored, the robot leverages **Dijkstra's Algorithm** to compute the absolute shortest path from the start to the goal. On all subsequent runs, it bypasses exploration and executes this optimal route with speed and precision.

The entire system is powered by an **Atmel Atmega32A** microcontroller, with firmware written entirely in C for performance and low-level hardware control.

---

### ► From Design to Reality

The robot's development followed a structured engineering workflow, from digital conception to a functional physical prototype. This ensured that all mechanical and electronic components would integrate seamlessly.

| 3D CAD Render (Autodesk Fusion 360) | Assembled Physical Prototype |
|:---:|:---:|
| ![CAD Render](https://github.com/user-attachments/assets/e17eaa49-87b0-4bab-98ce-fb2f0f853505)<br>*Initial design concept and component layout.* | ![Physical Prototype](https://github.com/user-attachments/assets/5af1bc0c-498e-4c53-beba-d93e2cfbc045)<br>*The fully assembled robot with all electronic components.* |

---

### ► Technical Deep Dive: Firmware and Algorithms

The intelligence of OrbitALL resides in its firmware. The code is modular, efficient, and leverages the Atmega32A's hardware peripherals to maximize performance.

#### Code Architecture
The project is organized into distinct modules, each with a specific responsibility:
-   `main.c`: The central nervous system. It contains the main control loop, orchestrates the state machine (calibration, mapping, solving), and integrates all hardware modules.
-   `QTRA.c`: A robust sensor module for the line-following IR array.
-   `PID.c`: The core of the motion control system, implementing the PID algorithm.
-   `driver.c`: A low-level abstraction for controlling the motors via the TB6612FNG H-bridge.
-   `tcrt5000.c`: Manages the side-mounted IR sensors for sharp turn detection.
-   `tc0_pwm.c`: Initializes hardware timers for generating motor PWM signals.
-   `lcd.c`: A library for interfacing with a character LCD for real-time debugging (if attached).

#### Key Implementation Details

**1. Advanced Line Sensing (`QTRA.c`)**
Instead of simple digital reads, the robot implements a sophisticated line-sensing system:
-   **Analog Multiplexing:** To read the 6-sensor array with a single ADC channel, the firmware uses GPIOs to control an analog multiplexer. The `QtrRead(channel)` function selects the desired sensor before triggering an ADC conversion.
-   **Dynamic Calibration:** The `QTRcalibrate()` routine is run before each session. It prompts the user to move the robot over the white surface and the black line, storing the minimum and maximum readings. This allows it to calculate a dynamic `umbral` (threshold) for each sensor, making the robot highly resilient to changes in ambient lighting.
-   **Weighted Position Calculation:** The `lectura()` function converts the six analog sensor readings into a single, continuous position value (from 0 to 700). It calculates a weighted average based on which sensors are activated, providing a highly precise input for the PID controller. This is far superior to a simple "is a sensor on?" check.

**2. Motion Control: A Clamped PID System (`PID.c`)**
Smooth and stable line following is achieved via a carefully tuned PID controller:
-   **Inputs:** The controller uses the weighted position from `lectura()` as its process variable and a fixed center value (350) as its setpoint.
-   **Implementation:**
    -   `Proportional (P)` term provides immediate corrective force.
    -   `Integral (I)` term is a rolling sum of recent errors, which eliminates steady-state drift and compensates for mechanical asymmetries.
    -   `Derivative (D)` term anticipates future error, damping oscillations and preventing overshoots.
-   **Output Clamping:** The final PID output (`diferencial`) is clamped to the maximum motor velocity. This is a critical safety feature that prevents motor saturation and ensures predictable behavior during sharp corrections.
-   **Differential Drive:** The PID output is applied differentially to the motors (`engines(vel + diferencial, vel)`), allowing for smooth and proportional turning.

**3. Non-Blocking Ultrasonic Sensing (`main.c`)**
To detect the payload and the goal without halting the main control loop, the firmware uses an interrupt-driven approach for the HC-SR04 ultrasonic sensor:
-   **Timer1 Input Capture Unit (ICU):** Instead of using blocking `delay` loops to measure the echo pulse, the system configures Timer1's ICU.
-   The `ISR(TIMER1_CAPT_vect)` interrupt service routine automatically captures the timer value on both the rising and falling edges of the echo pin.
-   This allows the CPU to continue executing the PID loop and other critical tasks while the hardware timer measures the pulse width in the background. The result is a highly responsive and efficient system.

**4. Motor Control Abstraction (`driver.c`)**
The `engines(izq, der)` function provides a clean interface for motor control. It accepts signed integers, where the sign determines the direction (by setting the H-bridge inputs) and the magnitude sets the speed (by writing to the `OCR0` and `OCR2` PWM registers).

---

### ► Hardware System

*   **MCU:** Atmel **Atmega32A** (8-bit AVR).
*   **Motor Driver:** **TB6612FNG** Dual H-Bridge. Chosen for its higher efficiency and lower voltage drop compared to alternatives like the L298N.
*   **Line Sensors:** Six **QTR-1A** Infrared Reflectance Sensors.
*   **Turn Sensors:** Two **TCRT5000** Infrared Sensors.
*   **Distance Sensors:** Two **HC-SR04** Ultrasonic Sensors.
*   **Motors:** Two **N20 DC Gearmotors**.
*   **Power:** Two **18650 Li-ion Cells** (in series) regulated to 5V by an **LM7805**.

---

### ► How to Build and Run

#### Prerequisites
-   An AVR-compatible IDE, such as **Microchip Studio**.
-   An AVR programmer (e.g., USBasp, Atmel-ICE).
-   Software for flashing the MCU (e.g., **AVRDUDESS** or the programming tool integrated into your IDE).
-   All the hardware components listed above, assembled as per the project schematics.

#### Steps
1.  **Clone the Repository:**
    ```bash
    git clone <your-repo-url>
    ```
2.  **Open in IDE:** Open the project folder in Microchip Studio.
3.  **Compile:** Build the project to generate the final `.hex` file.
4.  **Flash:** Connect your AVR programmer to the robot's ICSP headers and use AVRDUDESS to upload the compiled `.hex` file to the Atmega32A.
5.  **Calibrate and Run:**
    -   Place the robot on the maze.
    -   Follow the prompts for the sensor calibration routine (this involves exposing the sensors to both the line and the background).
    -   Once calibrated, the robot will begin its autonomous mapping and solving sequence.

---

### ► License
This project is licensed under the MIT License.
