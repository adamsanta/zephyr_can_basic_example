# Zephyr RTOS CAN TX/RX basic example


## Application Description

The application aims at proposing a method to transmit and receive CAN frames with Zephyr RTOS.<br>
More precisely, the application:
- Prints received CAN messages to the UART in this format: timestamp, CAN ID, DLC and data (e.g: `[33258 ms] ID:0x010 DLC:1 DATA:00`). CAN and UART are the default devices for the board used (refer to the board dts file).
- Is build time configurable for CAN baudrate, CAN messages transmit periods and CAN IDs.
- Sends three CAN messages with random data at build time defined periods.
- Starts printing when receiving a specific CAN message ID defined at build time (program starts immediately if the message is undefined).
- Stops printing when receiving a specific CAN message ID defined at build time (program never stops if the message is undefined).
- Prints 'Hello World' when receiving a specific CAN message ID defined at build time.
<br><br>

**Warning:** *If the STOP printing message is defined but not the START one, it will never be possible to START printing again after a STOP message.*

For a list of the build time configurable parameters, refer to the **Kconfig** files or to the **Configurable parameters** section below.<br>

The **tests** folder features the unit tests for the application.
<br><br>

## HOW TO

### Prerequisites

The content of the application folder should be placed in your **zephyrproject** folder at **zephyrproject/applications/zephyr_can_basic_example**.

It should also be possible to use it from anywhere if the ```ZEPHYR_BASE``` environment variable is defined correctly and a **west** environment is activated. Refer to Zephyr documentation if needed: [Zephyr Getting Started page](https://docs.zephyrproject.org/latest/develop/getting_started/index.html).
<br><br>

### Running the application on a board

<u>**Notes**</u>: The application has been tested with the **NXP Freedom MCXN947** board, a **PCAN Peak-USB** adapter, the **PCAN-View** software and **PuTTY** serial terminal.

All the commands below are considered being run from **zephyrproject/applications/zephyr_can_basic_example**.<br>

**Building**
```bash
west build -p always -b frdm_mcxn947/mcxn947/cpu0 .
```
The `-p always` option can be removed or change to avoid building everything from scratch.<br>
The board name can be changed with the `-b` option if needed. The board name is also set in **CMakeLists.txt** to make it possible to remove the option from the command.<br>
The **build** folder is generated in the application root folder.
<br>

**Flashing**
```bash
west flash
```
Drivers download might be needed for flashing.
<br>

**Configuring**
```bash
west build -t menuconfig -b frdm_mcxn947/mcxn947/cpu0 .
west flash # to apply the configuration
```
<br>

### Running the tests

The tests are gathered in the **tests** folder.<br>

**Listing**
```bash
west twister --list-tests -T tests/can_rx/
```

**Building**
```bash
west build -b native_sim tests/can_rx/ tests/can_rx/ --build-dir=tests/can_rx/build
```
The **build** folder is generated at **tests/can_rx/build**.<br>

**Running (locally)**
```bash
west build -t run --build-dir=tests/can_rx/build
```
A Linux machine is expected to run the tests built with the `-b native_sim` option (**Ubuntu WSL 24.04** on **Windows 11** has been used during development). Otherwise, containers or virtual machines can be used.<br>

**Configuring**
```bash
west build -t menuconfig -b native_sim tests/can_rx/ --build-dir=tests/can_rx/build
```
<br>

## Configurable parameters

By running one of the commands below,
```bash
west build -t menuconfig -b frdm_mcxn947/mcxn947/cpu0 . # for application

west build -t menuconfig -b native_sim tests/can_rx/ --build-dir=tests/can_rx/build # for tests
```

following elements can be configured:
- CAN_TX1 sending period in ms: `CAN_TX1_PERIOD_MS` (default=50ms)
- CAN_TX1 message ID: `CAN_TX1_ID` (default=0x11)
- CAN_TX1 message DLC: `CAN_TX1_DLC` (default=1)
- CAN_TX2 sending period in ms: `CAN_TX2_PERIOD_MS` (default=250ms)
- CAN_TX2 message ID: `CAN_TX2_ID` (default=0x22)
- CAN_TX2 message DLC: `CAN_TX2_DLC` (default=4)
- CAN_TX3 sending period in ms: `CAN_TX3_PERIOD_MS` (default=500ms)
- CAN_TX3 message ID: `CAN_TX3_ID` (default=0x33)
- CAN_TX3 message DLC: `CAN_TX3_DLC` (default=8)
- Activation of START_MSG: `START_MSG` (default=true)
- ID of START_MSG if activated: `START_MSG_ID` (default=0x200)
- Activation of STOP_MSG: `STOP_MSG` (default=true)
- ID of STOP_MSG if activated: `STOP_MSG_ID` (default=0x201)
- Activation of HELLO_MSG: `HELLO_MSG` (default=true)
- ID of HELLO_MSG if activated: `HELLO_MSG_ID` (default=0x202)
- Bitrate of the CAN communication: `CAN_BITRATE` (default=500kbit/s)
<br>

The **Kconfig** file can be consulted for more information.
<br><br>

## Scripts

Build and flash the app by running:
```bash
chmod +x scripts/app_build_flash.sh
./scripts/app_build_flash.sh
```
Build and run the tests by running:
```bash
chmod +x scripts/tests_build_run.sh
./scripts/tests_build_run.sh
```
Delete app and tests build folders by running:
```bash
chmod +x scripts/clean_build_folders.sh
./scripts/clean_build_folders.sh
```
<br>


## TODO

- Linting and formating code.
- Optimize threads stack size.
- Implement twister to run tests.
- Improve code factorization if possible (wrap printk and can_send to make it mockable for unit tests).
- Is it possible/better to fuse the 3 TX threads with common code ?
- Make possible to configure more than just the CAN baurate in Kconfig.
- Implement a true random generator in the TX threads (see warning when building).
- Use logging (LOG_INF, LOG_ERR) instead of printk.
- Is it better to use timers than k_sleep in the TX threads ?
- Use realtime for timestamp (RTC).
- Complete unit tests with potentially CAN and UART mocking.
- Manage CAN errors.
<br><br>

**Warnings**:
Depending on the evolution of the application's use cases, the value for the items below can become a problem:
- Threads stack size
- Threads priority
- CAN message RX queue size
<br>