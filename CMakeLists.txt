cmake_minimum_required(VERSION 3.20.0)

set(BOARD "frdm_mcxn947/mcxn947/cpu0")

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(zephyr_can_basic_example)

target_sources(app PRIVATE
    src/main.c
    src/can_rx.c
    src/can_tx.c
)