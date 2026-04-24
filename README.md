# Digital Clock & Alarm System on SN32F407

![Microcontroller](https://img.shields.io/badge/MCU-SN32F407-blue.svg)
![Language](https://img.shields.io/badge/Language-C-orange.svg)
![Status](https://img.shields.io/badge/Status-Active-success.svg)

A fully functional digital clock and alarm system built on the **SN32F407_EVK** evaluation board. The project features real-time timekeeping, a matrix keypad interface for settings, a 7-segment display multiplexer, and persistent storage using internal Flash memory to emulate EEPROM.

---

## Key Features

* **Real-time Clock:** Tracks hours, minutes, and seconds accurately.
* **Alarm Function:** Built-in buzzer alert for alarms.
* **Persistent Storage:** Automatically saves alarm settings to the internal Flash memory (`0x00007C00`), ensuring data isn't lost after a power cycle.
* **Matrix Keypad Interface:** Debounced keypad scanning for intuitive time and alarm configuration.
* **Auto-Timeout:** Automatically exits setting modes after 30 seconds of inactivity to prevent accidental changes.
* **Status Indicators:** Blinking LED modes (D6 & D7) to distinguish between Clock Setting Mode and Alarm Setting Mode.

---

## Hardware Requirements

* **MCU:** SONiX SN32F407_EVK (or SN8F5708_EVK)
* **Display:** 7-Segment LED Display
* **Input:** Matrix Keypad
* **Outputs:**
  * Buzzer (Active High)
  * Status LEDs (Active Low)

### Pin Configuration

| Component | MCU Pin | Description |
| :--- | :--- | :--- |
| **Buzzer** | `P3.0` | Sounds the alarm and beeps on key press |
| **LED D6** | `P3.8` | Blinks during Alarm Setting Mode |
| **LED D7** | `P3.9` | Blinks during Clock Setting Mode |
| **Keypad Col 0** | `P2.4` | Column output for keypad scanning |
| **Keypad Col 1** | `P2.7` | Column output for keypad scanning |
| **Keypad Row 0** | `P1.4` | Reads SW3 & SW6 |
| **Keypad Row 1** | `P1.5` | Reads SW10 |
| **Keypad Row 3** | `P1.7` | Reads SW16 |

*(Note: The 7-segment display pins are configured internally via `Segment.h`)*

---

## How to Use

The system is controlled via 4 buttons on the matrix keypad: **SW3, SW6, SW10, SW16**.

### 1. Clock Setting Mode
* Press **SW3** to cycle through: `Normal Mode` ➔ `Set Hours` ➔ `Set Minutes` ➔ `Normal Mode`.
* While setting, use **SW6** to increase (`+`) and **SW10** to decrease (`-`) the value.
* LED **D7** will blink to indicate you are in this mode.

### 2. Alarm Setting Mode
* Press **SW16** to cycle through: `Set Alarm Hours` ➔ `Set Alarm Minutes` ➔ `Save & Exit`.
* While setting, use **SW6** to increase (`+`) and **SW10** to decrease (`-`) the value.
* LED **D6** will blink to indicate you are in this mode.
* *Note: Exiting this mode automatically saves the alarm time to Flash Memory.*

### 3. Stop Alarm
* When the alarm rings, **press any button** to stop the buzzer immediately.

---

## Software Setup & Installation

To build and flash this project, you need the appropriate development environment for SONiX MCUs:

1. **IDE:** Keil uVision (MDK-ARM).
2. **Device Family Pack (DFP):** You **must** install the SONiX pack:
   * `SONiX.SN32F4_DFP.0.0.18.pack` (or version `>= 0.0.18`).
3. Clone this repository:
   ```bash
   git clone [https://github.com/your-username/your-repo-name.git](https://github.com/your-username/your-repo-name.git)
