# EcoGrow - Smart Greenhouse

Welcome to EcoGrow, the next-generation smart greenhouse designed to empower plant growth through precision automation, monitoring, and sustainable solar energy. This cutting-edge system not only fosters the perfect environment for plants but also ensures off-grid autonomy for enthusiasts and gardeners.

## Project Overview 🌱

EcoGrow is engineered to create an optimal environment for plant growth, utilizing advanced sensors and actuators to regulate climate and hydration—powered entirely by a solar energy loop. A standout feature is its ability to autonomously manage its power reserves and protect its hardware (like halting the pump if the reservoir is empty), ensuring an efficient, hands-off growing experience.

## Core Features 🌟

*   **Solar-Powered Autonomy:** Integrated Samsung charging board and 18650 battery circuit powered by a solar panel for off-grid continuous operation.
*   **Automated Climate Control:** Dynamic tracking of ambient temperature, triggering a 12V exhaust fan when thermal thresholds are exceeded.
*   **Precision Irrigation:** Smart water pumps and capacitive soil moisture sensors to deliver precise hydration.
*   **Hardware Safety Protocols:** Integrated water level sensor ensures the pump is deactivated if the reservoir is empty, preventing motor burnout.
*   **Low-Power Operation:** Optimized codebase ensuring efficient power management for continuous greenhouse operations.

## Enhanced Hardware Suite 🛠️

*   **ESP32 Microcontroller:** The powerhouse of EcoGrow, orchestrating all tasks seamlessly.
*   **Power & Battery Circuit:** Solar Panel, Samsung charging board, and 18650 Li-ion Battery for robust off-grid energy.
*   **DHT11 Sensor:** High-accuracy sensor for humidity and ambient temperature readings.
*   **Capacitive Soil Moisture Sensor (v1.2):** Corrosion-resistant sensor for vigilant monitoring of soil moisture levels.
*   **Water Level Sensor:** Monitors the reservoir capacity to ensure safe pump operation.
*   **Automated Water Pump:** 5V submersible mini water pump ensures your plants are watered just right, without waste.
*   **12V Ventilation Fan:** A breath of fresh air to simulate natural conditions and regulate heat.
*   **Dual Relay Modules:** The digital switchboard for safely controlling the high-current water pump and cooling fan.

## Design and Schematics 🔌

**Overview of EcoGrow Schematics**

Get an insight into the EcoGrow setup with our detailed schematic layout. For comprehensive wiring information for each module, refer to our pinout guide in the repository at `/hardware/schematics/Pinouts.md`. 

**Wiring Showcase**
<br>
<video src="./assets/wiring_video.mp4" controls="controls" style="max-width: 100%;"></video>

**Product Demonstration**
<br>
<audio src="./assets/audio_explanation.mp3" controls="controls"></audio>

## EcoGrow in Action 📸

Below is the final assembly of EcoGrow and a live demonstration of the system operating autonomously.

![EcoGrow Final Product](./assets/final_product.jpg)

**System Demonstration**
<br>
<video src="./assets/webpage_demo.mp4" controls="controls" style="max-width: 100%;"></video>

## Quickstart Guide 🚀

Begin your EcoGrow journey with these simple steps:

1.  **Clone the Repository:** Start by cloning this repository to your local machine. This will give you access to all necessary files for your EcoGrow setup.
2.  **Hardware Assembly:** Navigate to the `/hardware/schematics` folder within the cloned repository. Follow the provided `Pinouts.md` guide to assemble your power circuit and hardware components accurately.
3.  **Code Deployment:** Find your way to the `/src` directory, where the `EcoGrow.ino` source code resides. Deploy this code onto your ESP32 microcontroller using the Arduino IDE. 
4.  **Smart Greenhouse Activation:** Double-check all connections and configurations. Place the solar panel in direct sunlight to initialize the battery charging loop, and power on your ESP32 to activate your smart greenhouse system.

## Team 👥

*   **Sujayram Prasad** - Lead Programmer, Hardware Integration & Architecture
*   **Advita Vaibhav Bhosale** - System Design & Emotional Support

## Contribute to EcoGrow 🤝

Join the EcoGrow movement! Contributions can range from code enhancements, feature suggestions, documentation improvements, and more. 

## License 📄

EcoGrow is open-source and is proudly shared under the MIT License.

## Stay Connected 📬

Have questions or ideas? Let's cultivate a conversation! Reach out via GitHub issues or connect with me on my portfolio at prowiddan.me.
