# Pico_W bit-banger to control an RC Toy

A  Raspberry Pico project to control Radio Controlled toys using the integrated circuit **RX2** found in many inexpensive RC toys. The idea is to connect just ground and one pin from the Pico to the signal input of the RX2 and from there, send the data in the specified format (check the TX2/RX2 datasheet for more information) to control up to two motors connected to the RX2. No need for an external H-Bridge or any extra pins connected. [Check this Arduino Forum Thread for more information](https://forum.arduino.cc/t/control-cheap-rc-toy-car-rx2-chip-with-arduino-using-only-one-pin-solved/167268/8).

I'm using an Xbox controller to send the commands to the Pico but it can be driven autonomously with sensors or through WiFi.

This project is using a fork of [usedbytes/picow_ds4](https://github.com/usedbytes/picow_ds4) to connect the Xbox Controller. 

## Prerequisites

Before using this library, make sure you have the following:

- Raspberry Pico or Pico W board
- An RC toy with any version of the RX2 receiver. I'm using [this one](https://a.co/d/gfmupyS)
- [Pico SDK](https://www.raspberrypi.com/documentation/pico-sdk/) installed
- I'm also using an Xbox controller, but you don't need one if you wanna run autonomously or use a web server through wifi.

## Getting Started

Check the instructions for the [Xbox Controller](https://github.com/intGus/picow_xbox_controller) submodule if you plan to use it. Otherwise, skip step 3

On the toy, you need to find the RX2 chip and try to identify the Signal Input pin. There are two [datasheets](https://github.com/intGus/picow_rx2_bitbanger/tree/main/datasheets) attached to help you figure out which pin to use (in most cases will be pin 3, in my case is pin 6),
once you identified the pin, you'll need to connect that pin to any pin in the Pico and connect ground from the toy to any ground on the Pico. I'm using pin 14 with a voltage divider to drop output to 1.6v just for safety.

**IMPORTANT:** If you want to power the Pico from the toy car, make sure the voltage is not over or under the limits of the input voltage of your board! then connect the positive lead from the batteries to the VSYS pin on your Pico.

After that, you can use the following steps  to clone and build the file needed in the Pico

1. Clone this repository `git clone https://github.com/intGus/picow_rx2_bitbanger.git`
2. Use `cd picow_rx2_bitbanger` to navigate inside the repository
3. Run `git submodule update --init --recursive` to get the Xbox Controller library. Skip this if you are not using an Xbox controller and remove references from the code.
4. Create the build directory `mkdir build`
5. Open the build directory `cd build`
6. Execute `cmake -DPICO_BOARD=pico_w -DPICO_SDK_PATH=/your/path/to/pico-sdk ../` make sure to replace with the appropriate path to your pico-sdk directory or remove 
   if you are using an environmental variable and replace it with `-DPICO_BOARD=pico` if using the regular Pico
7. Execute `make`
8. Drag and drop or copy the `picow_rx2.uf2` file into the pico in BOOTSEL mode

The `.uf2` file will be `/build/src/picow_rx2.uf2`

## Usage

The `main.c` file already has some code on how to make it work, my toy only has one motor so I only needed forward and backward functions.

The start code (named W2 in the datasheet) is replicated by setting a pin to high for 1500 microseconds, then back to low for 500 microseconds, and needs to be sent before any function code.
The function code (named W1 in the datasheet) is replicated by setting the pin to high for 500 microseconds and then low for another 500 microseconds.

*This is a reference and you need to figure out what each motor is doing by testing the codes*

| Number of Function Codes (W1) |           Function           |
|:-----------------------------:|:----------------------------:|
|               10              | Motor 1 - Forward            |
|               28              | Motor 1 & 2 - Forward Left   |
|               34              | Motor 1 & 2 - Forward Right  |
|               40              | Motor 1 - Backward           |
|               46              | Motor 1 & 2 - Backward Right |
|               52              | Motor 1 & 2 - Backward Left  |
|               58              | Motor 2 - Left               |
|               64              | Motor 2 - Right              |

For example, to make the motor go backward, first, send the start code (W2) 4 times, then the function code (W1) 40 times.

## Acknowledgements
* To whoever figured this out and made it work on the Arduino and posted it on the [forum](https://forum.arduino.cc/t/control-cheap-rc-toy-car-rx2-chip-with-arduino-using-only-one-pin-solved/167268/8) 
  but has no account now.
* [usedbytes/picow_ds4](https://github.com/usedbytes/picow_ds4)
* [bluekitchen/btstack](https://github.com/bluekitchen/btstack)https://github.com/bluekitchen/btstack