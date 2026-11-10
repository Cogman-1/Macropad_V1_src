# Macropad V1 — Firmware

Custom STM32G4 firmware for the Cogman Macropad V1.

The firmware provides hardware control and USB communication for a custom 9-key macropad.

> **Project Status: Early Development**
>
> The firmware is currently under active development. USB HID communication and basic hardware functionality are being implemented. The interface and protocol may change during development.

## Overview

Macropad V1 is a custom USB macropad powered by an STM32G4 microcontroller.

Unlike a conventional USB keyboard, the device is designed around a **vendor-defined USB HID interface**, allowing direct communication with a dedicated host application.

The firmware is intended to handle low-level hardware operations while higher-level functionality, such as configurable macros and system actions, will be managed by a separate host daemon.

## Hardware

The firmware targets the custom Macropad V1 PCB.

| Component | Description |
|---|---|
| MCU | STM32G4 Series |
| USB | USB 2.0 Full Speed |
| Interface | Custom HID |
| Buttons | 9 mechanical switches |
| LEDs | 9 individual LEDs |
| Debugging | SWD |

The hardware design is maintained in a separate repository.

[HW Repository Link](https://github.com/Cogman-1/Macropad_V1_PCB)

## Development Environment

The project is developed using:

- **STM32CubeIDE** — Development environment
- **STM32CubeMX** — MCU and peripheral configuration
- **STM32 HAL** — Hardware abstraction layer
- **STM32 USB Device Library** — USB communication stack
- **C** — Firmware implementation

The firmware currently follows a bare-metal architecture.

## USB Communication

The device uses USB Full Speed with a vendor-defined HID interface.

Rather than exposing individual buttons and LEDs through standard HID Usage Pages, the firmware uses a custom binary data format.

This allows the host application to interpret button states and control device functionality directly.

### HID Configuration

| Parameter | Value |
|---|---|
| USB Version | USB 2.0 |
| Transfer Speed | Full Speed |
| Device Class | HID |
| HID Interface | Custom HID |
| Usage Page | `0xFF00` (Vendor Defined) |
| Input Report Size | 2 bytes |
| Output Report Size | 2 bytes |
| Report IDs | Not currently used |

### HID Report Descriptor

The current descriptor defines two 16-bit data fields, each represented by two 8-bit values.

#### Input Report — Button States

| Usage | Description | Size |
|---|---|---|
| `0x01` | Buttons State — LSB | 8 bits |
| `0x02` | Buttons State — MSB | 8 bits |

#### Output Report — LED States

| Usage | Description | Size |
|---|---|---|
| `0x11` | LEDs State — LSB | 8 bits |
| `0x12` | LEDs State — MSB | 8 bits |

Both reports use **little-endian byte order**.

### Data Format

Button and LED states are represented as 16-bit unsigned values.

```text
Byte 0             Byte 1
┌─────────────────┬─────────────────┐
│   Bits 0–7      │   Bits 8–15     │
│   LSB           │   MSB           │
└─────────────────┴─────────────────┘
```

For the current hardware:

- Bits 0–8 represent the 9 buttons or LEDs.
- Bits 9–15 are reserved for future use.

Example:

```text
Buttons State: 0x0105

Byte 0: 0x05
Byte 1: 0x01

Active buttons: 1, 3 and 9
```

The HID descriptor defines the size and structure of the transmitted data, while the firmware and host daemon determine its interpretation.

> **Note:** The USB protocol is currently under development and is not considered stable.

## Firmware Architecture

The firmware is intended to separate hardware functionality into independent modules.

### Button Handling

Responsible for:

- Reading GPIO inputs
- Detecting button state changes
- Implementing software debouncing
- Preparing HID input reports

### LED Control

Responsible for:

- Controlling individual LED outputs
- Receiving LED state commands
- Updating LED states

### USB Communication

Responsible for:

- USB device initialization
- Custom HID report handling
- Transmitting button states
- Receiving host commands

Additional functionality may be introduced as development progresses.

## Building the Firmware

### Requirements

- STM32CubeIDE
- Compatible STM32CubeG4 firmware package
- STM32-compatible SWD programmer/debugger

### Importing the Project

1. Clone this repository:

   ```bash
   git clone <repository-ssh-url>
   ```

2. Open STM32CubeIDE.
3. Select `File → Import`.
4. Choose `General → Existing Projects into Workspace`.
5. Select the cloned repository directory.
6. Import the project.
7. Build the project using the provided configuration.

The repository includes the STM32CubeIDE project configuration and the STM32CubeMX `.ioc` file.

These files allow developers to inspect and modify the MCU configuration without recreating the project manually.

### Flashing

The firmware can be programmed using an SWD-compatible debugger, such as an ST-LINK.

The custom PCB provides an SWD programming and debugging interface.

## Host Software

A separate host application is planned for Linux and Windows.

The application will consist of two main components.

**Background Daemon**

Responsible for:

- Communicating with the macropad over USB HID
- Processing button events
- Executing configurable actions and macros
- Managing device configuration

**Configuration GUI**

Responsible for:

- Configuring button assignments
- Managing profiles
- Configuring LED behavior
- Communicating with the background daemon

The host software will be maintained in a separate repository once development begins.

### Important

The current vendor-defined HID interface is not a standard USB keyboard interface.

The operating system will not automatically interpret its reports as normal keyboard input.

Keyboard actions will instead be handled by the planned host application.

## Development Roadmap

- [x] STM32CubeIDE project initialization
- [x] Initial MCU peripheral configuration
- [x] Initial Custom HID report descriptor
- [x] Validate USB HID communication
- [x] Implement button scanning
- [x] Implement button debouncing
- [ ] Implement LED control
- [ ] Implement USB input and output report handling
- [ ] Define a stable communication protocol
- [ ] Integrate with the host daemon
- [ ] Test firmware on hardware
- [ ] Prepare initial firmware release

## Future Development

Potential improvements include:

- Configuration storage
- Device information and firmware version reporting
- Advanced LED behavior
- Extended HID commands
- Support for future Macropad hardware revisions

## License

Copyright 2026 Jan Kolář.

Unless otherwise specified, original code developed for this
project is licensed under the Apache License, Version 2.0.

Third-party software components remain subject to their
respective licenses and are not relicensed under Apache 2.0.

See [LICENSE](LICENSE) for the full license text and
[THIRD_PARTY_LICENSES.md](THIRD_PARTY_LICENSES.md)
for third-party licensing information.

