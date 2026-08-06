
A compact parking-assistance project built with the **STM32 NUCLEO-F401RE**, an **HC-SR04 ultrasonic sensor**, an **SSD1306 OLED display**, and an **active buzzer**.

The system measures the distance to an obstacle, shows the result on the OLED, prints diagnostic information through UART, and generates different buzzer warnings depending on the measured distance.
<img width="2000" height="1500" alt="WhatsApp Image 2026-08-07 at 00 07 12" src="https://github.com/user-attachments/assets/6a5c8e02-b52f-4329-91c4-c84f6c048929" />

---

## Features

- Distance measurement using the HC-SR04
- TIM3 input capture for accurate ECHO pulse measurement
- Distance output on a 128×64 SSD1306 OLED
- UART output through the onboard ST-LINK Virtual COM Port
- Active-low buzzer warning
- Timeout handling when no ECHO pulse is received
- Three parking-safety zones

---

## Parking Warning Logic

| Distance | OLED message | Buzzer behavior |
|---|---|---|
| Below 10 cm | `STOP!` / `Obstacle` | Continuous beep |
| 10 cm to below 20 cm | `WARNING` / `Obstacle detected` | Intermittent beep |
| 20 cm or more | Distance value | Off |
| No ECHO received | `No echo` / `Check sensor` | Off |

> The buzzer used in this project is **active-low**:
>
> - GPIO `RESET` / LOW → buzzer ON  
> - GPIO `SET` / HIGH → buzzer OFF

---

## Hardware

- STM32 NUCLEO-F401RE
- HC-SR04 ultrasonic sensor
- SSD1306 OLED, 128×64, I2C, address `0x3C`
- Active buzzer
- 220 Ω resistor
- 330 Ω resistor
- Breadboard and jumper wires
- USB cable for programming and UART communication

---

## Connections

```text
┌──────────────────────────────────────────────────────────────┐
│                STM32 NUCLEO-F401RE CONNECTIONS               │
├───────────────────────┬──────────────────────────────────────┤
│ HC-SR04 VCC           │ Nucleo 5V                            │
│ HC-SR04 GND           │ Nucleo GND                           │
│ HC-SR04 TRIG          │ PA8                                  │
│ HC-SR04 ECHO          │ 220 Ω → junction → PA6 / TIM3_CH1    │
│                       │            │                         │
│                       │          330 Ω                       │
│                       │            │                         │
│                       │           GND                        │
├───────────────────────┼──────────────────────────────────────┤
│ OLED VCC              │ 3.3V or 5V, according to module     │
│ OLED GND              │ Nucleo GND                           │
│ OLED SCL              │ PB8 / I2C1_SCL                       │
│ OLED SDA              │ PB9 / I2C1_SDA                       │
├───────────────────────┼──────────────────────────────────────┤
│ Active buzzer signal  │ GPIO configured as Buzzer_Pin        │
│ Active buzzer GND     │ Nucleo GND                           │
├───────────────────────┼──────────────────────────────────────┤
│ USART2 TX             │ PA2 → onboard ST-LINK Virtual COM    │
│ USART2 RX             │ PA3 → onboard ST-LINK Virtual COM    │
└───────────────────────┴──────────────────────────────────────┘
```

### HC-SR04 ECHO Voltage Divider

The HC-SR04 ECHO pin outputs approximately 5 V, while the STM32 uses 3.3 V logic.  
A resistor divider protects the STM32 input:

```text
HC-SR04 ECHO
      │
    220 Ω
      │
      ├──────────── PA6 / TIM3_CH1
      │
    330 Ω
      │
     GND
```

The voltage at PA6 is approximately:

```text
5 V × 330 / (220 + 330) ≈ 3.0 V
```

---

## STM32CubeMX Configuration

### TIM3

- Clock Source: Internal Clock
- Channel 1: Input Capture direct mode
- Prescaler: `83`
- Counter Mode: Up
- Counter Period: `65535`
- Clock Division: No Division
- Input Capture Polarity: Rising Edge
- Input Capture Selection: Direct Input
- Input Capture Prescaler: No Division
- Input Filter: `0`
- TIM3 global interrupt: Enabled

With an 84 MHz TIM3 clock:

```text
84 MHz / (83 + 1) = 1 MHz
```

Therefore:

```text
1 timer count = 1 µs
```

### I2C1

- PB6: I2C1_SCL
- PB7: I2C1_SDA
- Clock speed: 100 kHz
- OLED address: `0x3C`

### USART2

- Mode: Asynchronous
- Baud rate: 115200
- Word length: 8 bits
- Stop bits: 1
- Parity: None
- Flow control: None

### GPIO

- PA8: HC-SR04 TRIG output
- `Buzzer_Pin`: active buzzer output
- Buzzer initial state: HIGH because the buzzer is active-low

---

## How It Works

1. PA8 sends a 10 µs trigger pulse to the HC-SR04.
2. The sensor emits an ultrasonic burst.
3. The ECHO signal goes HIGH until the reflected sound returns.
4. TIM3 Channel 1 captures the rising and falling edges.
5. The pulse width is calculated in microseconds.
6. Distance is calculated using:

```text
Distance in cm ≈ ECHO time in µs / 58
```

7. The result is:
   - printed through USART2,
   - shown on the OLED,
   - evaluated by the buzzer warning logic.

---

## Example UART Output

```text
HC-SR04 measurement started
Echo: 971 us | Distance: 16.7 cm
Echo: 952 us | Distance: 16.4 cm
Echo: 1010 us | Distance: 17.4 cm
```

Open the ST-LINK Virtual COM Port with:

```text
Baud rate: 115200
Data bits: 8
Parity: None
Stop bits: 1
Flow control: None
```

---

## Project Structure

```text
Core/
├── Inc/
│   ├── main.h
│   ├── ssd1306.h
│   ├── ssd1306_conf.h
│   └── ssd1306_fonts.h
└── Src/
    ├── main.c
    ├── ssd1306.c
    └── ssd1306_fonts.c
```

---

## Notes

- All modules must share a common GND.
- Do not connect the HC-SR04 ECHO pin directly to the STM32 input.
- Keep at least about 60–100 ms between measurements.
- The exact physical buzzer pin must match the GPIO selected in the `.ioc` file and generated as `Buzzer_Pin`.
- The onboard ST-LINK provides UART communication, so no external USB-to-UART adapter is required.

---

## Possible Improvements

- Average several measurements to reduce noise.
- Add hysteresis near the 10 cm and 20 cm thresholds.
- Change the intermittent beep rate based on distance.
- Separate the HC-SR04, OLED, and buzzer code into reusable driver files.
- Convert the project to FreeRTOS tasks.
- Add LEDs for safe, warning, and critical zones.

---

## Project Title

**STM32 Ultrasonic Parking Assistance System with OLED and Active Buzzer**

<img width="1500" height="2000" alt="WhatsApp Image 2026-08-07 at 00 07 21" src="https://github.com/user-attachments/assets/7230edcd-ee55-477a-a6ba-8a95ad983c73" />





