# UART LED Control — STM32F401RE

This project demonstrates UART communication and interrupt handling on the STM32 NUCLEO-F401RE.

The microcontroller receives commands from a computer through USART2 and controls the onboard LED and an external LED.

## Features

- UART communication at 115200 baud
- Interrupt-based UART reception
- Control of the onboard LED
- Control of an external LED
- Non-blocking LED blinking using `HAL_GetTick()`
- Push-button interrupt using EXTI
- Simple software button debouncing

## UART Commands

| Command | Action |
|---|---|
| `0` | Turn onboard LED off |
| `1` | Turn onboard LED on |
| `2` | Blink onboard LED every 500 ms |
| `3` | Turn external LED on |
| `4` | Turn external LED off |

The onboard user button toggles the external LED.

## Hardware

- NUCLEO-F401RE
- External LED
- 220–330 Ω resistor
- Jumper wires
- USB cable

## Main Concepts

- GPIO
- USART2
- NVIC interrupts
- UART receive callback
- EXTI button interrupt
- `volatile` variables
- Non-blocking timing


<img width="1200" height="1600" alt="WhatsApp Image 2026-08-04 at 22 17 32" src="https://github.com/user-attachments/assets/c39108ae-4895-41d8-8434-2b879648a7db" />
