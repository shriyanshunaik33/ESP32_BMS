# Working of ESP32-Based IoT Battery Management System

This document explains the detailed working of the ESP32-based IoT
Battery Management System (BMS), including hardware operation, data
flow, embedded logic, and cloud integration.

------------------------------------------------------------------------

# 1. Overview

The system is designed to monitor and manage a lithium-ion battery in
real-time using an ESP32 microcontroller.

It performs:

• Voltage monitoring\
• Temperature monitoring\
• State of Charge (SOC) estimation\
• Charging control using relay\
• IoT-based remote monitoring

The system ensures safe battery operation by maintaining it within safe
limits.

------------------------------------------------------------------------

# 2. System Architecture

The system consists of three main layers:

### 1. Sensing Layer

• Voltage Divider → Measures battery voltage\
• NTC Thermistor → Measures temperature

### 2. Control Layer

• ESP32 Microcontroller\
• Processes sensor data\
• Executes safety logic

### 3. Communication Layer

• Wi-Fi (ESP32)\
• Arduino IoT Cloud\
• Dashboard visualization

As shown in the paper, the ESP32 acts as the central controller handling
sensing, processing, and communication.
:contentReference[oaicite:1]{index="1"}

------------------------------------------------------------------------

# 3. Hardware Working

## Voltage Measurement

A voltage divider using two 10kΩ resistors scales battery voltage:

Battery range:\
3.0V -- 4.2V

Converted safely for ESP32 ADC (max 3.3V)

------------------------------------------------------------------------

## Temperature Measurement

• NTC Thermistor used\
• Connected in voltage divider\
• Measures battery surface temperature

------------------------------------------------------------------------

## Charging Control

• TP4056 module handles charging\
• Relay module controls power supply

ESP32 turns relay ON/OFF based on safety conditions

------------------------------------------------------------------------

# 4. Firmware Working (Arduino Code Logic)

## Initialization (setup)

When ESP32 starts:

• Serial communication initialized\
• Relay pin set as OUTPUT\
• Wi-Fi connection established\
• Arduino IoT Cloud initialized

------------------------------------------------------------------------

## Data Acquisition (loop)

ESP32 continuously:

• Reads voltage (GPIO 34)\
• Reads temperature (GPIO 35)\
• Takes multiple readings\
• Averages values to reduce noise

------------------------------------------------------------------------

## Data Processing

### Voltage Calculation

Voltage is calculated using divider formula:

Vbattery = Vadc × (R1 + R2) / R2

------------------------------------------------------------------------

### Temperature Calculation

• Thermistor resistance calculated\
• Converted to temperature using Steinhart-Hart equation

------------------------------------------------------------------------

### SOC Estimation

SOC is calculated using:

• Voltage-based mapping\
• Piecewise interpolation (from lookup table)

This improves accuracy compared to simple linear methods
:contentReference[oaicite:2]{index="2"}

------------------------------------------------------------------------

# 5. Safety Logic

The system continuously checks:

### Conditions:

• Voltage \> 4.2V → Over-voltage\
• Temperature \> 45°C → Over-temperature

------------------------------------------------------------------------

## Action:

• Relay is turned OFF\
• Charging is stopped

------------------------------------------------------------------------

## Hysteresis Control

To avoid rapid switching:

• Charging resumes only when voltage drops below safe margin (≈4.1V)

Prevents relay chattering and ensures stability
:contentReference[oaicite:3]{index="3"}

------------------------------------------------------------------------

# 6. IoT Communication

ESP32 sends data to Arduino IoT Cloud:

• Voltage\
• Temperature\
• SOC\
• Charging status

------------------------------------------------------------------------

## Dashboard Features

• Real-time monitoring\
• Graph visualization\
• Remote access

As shown in the paper dashboard (page 5), data updates continuously and
reliably :contentReference[oaicite:4]{index="4"}

------------------------------------------------------------------------

# 7. System Behavior (Real Operation)

During charging:

• Voltage increases gradually\
• Temperature rises slightly\
• SOC increases step-by-step

------------------------------------------------------------------------

## At Full Charge:

• Voltage reaches \~4.2V\
• System cuts off charging\
• Relay switches OFF

------------------------------------------------------------------------

## Observations

• Stable operation\
• Accurate sensing\
• Reliable cutoff\
• Safe temperature range maintained

Data table in paper (page 4) confirms correct system response
:contentReference[oaicite:5]{index="5"}

------------------------------------------------------------------------

# 8. Charging Profile

System follows:

### CC-CV Charging

1.  Constant Current Phase\
    → Voltage increases

2.  Constant Voltage Phase\
    → Current decreases

Controlled using TP4056 + ESP32 monitoring

------------------------------------------------------------------------

# 9. Advantages of the System

• Low cost\
• IoT enabled\
• Real-time monitoring\
• Open-source and customizable\
• Scalable for EV applications

------------------------------------------------------------------------

# 10. Future Improvements

• Add current sensor (for better SOC)\
• Implement AI-based battery prediction\
• Extend to multi-cell battery packs\
• Add cell balancing system

------------------------------------------------------------------------

# 11. Conclusion

The ESP32-based BMS successfully:

• Monitors battery parameters in real-time\
• Ensures safety through automated control\
• Enables remote monitoring using IoT

It provides a **low-cost, scalable, and intelligent battery management
solution** suitable for EV and IoT applications.
