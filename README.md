# Custom Teensy LC game pad

In the context of exhibitions, software is often controlled via dedicated custom buttons build into the case of the exhibit. Most often, only a limited set of keys is needed to navigate the exhibit. 

This project aims to aid custom game pad creation using a Teensy LC micro controller. All push buttons that close an electrical connection when pressed and open it when released can be used.

The game pad supports the following buttons: left (`L`), right (`R`), up (`U`), down (`D`) as well as the action buttons `A`,  `B`,  `X`,  `Y`. The directions `L`,  `R`,  `U` and `D` emulate both, the analog stick and the directional pads, simultaneously.

# Wiring

A button must be connected to GND and one of the digital pins of the Teensy LC. See the `PIN_*` entries in the [`config.h`](config.h) file for the pin reference, e.g.
```
#define PIN_L 0
```
tells you to connect the button for `L` to pin 0.

# Flashing the .hex file

For convenience, we offer pre-compiled `.hex` files for download in the release section.
The device specific `.hex` file can be flashed onto the Teensy LC via the [Teensy Loader](https://www.pjrc.com/teensy/loader.html). Downloading and installing the
full [Teensyduino IDE](https://www.pjrc.com/teensy/teensyduino.html) is not necessary for flashing the `.hex` file.

Open the Teensy Loader, connect the Teensy LC, open the `.hex` file and flash.

There are also other methods involving [command line tools](https://www.pjrc.com/teensy/loader_cli.html), but they are not discussed here.

# Customization

The most important options of this sketch can be customized via the [`config.h`](config.h) file, but this requires [recompilation](#Compilation) of the code.

# Pins

If you prefer to connect your physical buttons to different pins, you can do so by modifying the `PIN_*` macros. Please check the [Teensy LC pinout](https://www.pjrc.com/teensy/pinout.html).

# Buttons

The button id that will be sent upon a button press can be configured via the `BUTTON_ID_*` macros. If you plan to emulate a certain gamepad, you should use a tool like [Gamepad Tester](https://gamepad-tester.com/) to figure out which button ids this gamepad sends and set the `BUTTON_ID_*` values accordingly.

# Debouncing

Almost every physical button is subject to contact-bounce such that it takes a couple of milliseconds until the contact is stable and stops bouncing between digital 0 and 1. The length of the debouncing interval (in milliseconds) can be configured via the `DEBOUNCE_INTERVAL` macro.

# Compilation

Compiling the sketch requires the [Teensyduino IDE](https://www.pjrc.com/teensy/teensyduino.html).

After opening the project in Teensyduino and connecting the Teensy LC, you have to apply a couple of settings:
- Tools -> Board -> Teensyduino -> Teensy LC
- Tools -> USB Type -> Serial + Keyboard + Mouse + Joystick
- CPU Speed -> 24 MHz

Add the `Bounce2` library (`~2.53.0`) to the sketch.

Now, you can verify and upload your sketch to the Teensy LC.
