# USB HID Keyboard Bridge Using UART -- STM32

This project implements a **USB HID Keyboard emulator** on an STM32
microcontroller.\
Data received over **UART** is converted into **USB keyboard
keystrokes** and sent to the connected PC.\
It effectively works as a **UART-to-Keyboard bridge**, useful for
automation, scripts, debugging tools, or custom input devices.

------------------------------------------------------------------------

## ✨ Features

-   📥 **UART RX with Interrupts**\
    Receives characters one byte at a time using
    `HAL_UART_RxCpltCallback`.

-   ⌨️ **USB HID Keyboard Output**\
    Sends received text to the PC as keystrokes using the HID interface
    (`KeyBoardPrint`).

-   🔄 **Line-Based Processing**\
    UART data is buffered until a newline (`\n` or `\r`) is received.

-   🛡️ **Robust Buffer Handling**\
    Prevents buffer overflow by limiting characters to a defined size.

-   🔌 **Plug-and-Play USB HID**\
    Works without installing drivers on Windows/Mac/Linux.

------------------------------------------------------------------------

## 📂 Project Structure

    .
    ├── Core/
    │   ├── Src/
    │   │   ├── main.c      <-- Main program logic
    │   └── Inc/
    ├── USB_Device/
    │   └── HID implementation
    └── README.md

------------------------------------------------------------------------

## 🛠️ How It Works

### 1. UART Reception

-   UART2 is initialized at **115200 baud**.
-   Interrupt-based single-byte receive is enabled using:

``` c
HAL_UART_Receive_IT(&huart2, &uart_rx_byte, 1);
```

-   A line is considered complete when `\n` or `\r` is received.

### 2. HID Keyboard Output

Once `uart_line_ready = true`, the buffer content is sent to the
computer:

``` c
KeyBoardPrint(uart_rx_buffer, uart_rx_index - 1);
```

### 3. Main Loop

-   Continuously checks if a full line is received.
-   Sends keystrokes and resets buffer.

------------------------------------------------------------------------

## 🚀 Getting Started

### Hardware Required

-   STM32 board with USB Device support (e.g., STM32F1/F4 series)
-   USB cable
-   UART sender (PC terminal, external module, another MCU)

### Software Requirements

-   STM32CubeMX or STM32CubeIDE
-   HAL drivers for UART + USB HID
-   USB Device middleware (HID)

------------------------------------------------------------------------

## ⚙️ Configuration Summary

### UART Settings

  Parameter     Value
  ------------- --------
  Baud Rate     115200
  Word Length   8 bits
  Parity        None
  Stop Bits     1
  Mode          RX/TX

### USB

-   Class: **HID Keyboard**
-   Endpoint: IN
-   Polling interval: Firmware default (1ms typical)

------------------------------------------------------------------------

## 📡 Example Usage

Send the following over UART:

    Hello World\n

The PC will receive keyboard input as if typed:

> Hello World

------------------------------------------------------------------------

## 🔧 Customization

### Change UART buffer size:

``` c
#define UART_RX_BUFFER_SIZE 64
```

### Change keyboard behavior

Modify `KeyBoardPrint()` implementation inside USB HID source.

------------------------------------------------------------------------

## 📜 License

This project follows the default STM32Cube license as included in ST's
generated files.

------------------------------------------------------------------------

## 🤝 Contributions

PRs, suggestions, and improvements are welcome!
