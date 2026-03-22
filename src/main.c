#include <stdio.h>

#include "can_tx.h"
#include "can_rx.h"

/*
* Threads configuration and creation (STARTS)
*
* RX_THREAD receives CAN frames and prints their content, or 'Hello World',
* to UART if enabled with the START message. Priting can be disabled with the STOP
* message.
* TX1_THREAD periodically transmits the message assigned to it, same thing for
* TX2 and TX3.
*/

// RX is more timing-sensitive than TX, hence the higher priority
#define CAN_RX_THREAD_STACK_SIZE 512
#define CAN_RX_THREAD_PRIORITY 1
#define CAN_TX1_THREAD_STACK_SIZE 512
#define CAN_TX1_THREAD_PRIORITY 2
#define CAN_TX2_THREAD_STACK_SIZE 512
#define CAN_TX2_THREAD_PRIORITY 2
#define CAN_TX3_THREAD_STACK_SIZE 512
#define CAN_TX3_THREAD_PRIORITY 2

K_THREAD_STACK_DEFINE(can_rx_thread_stack, CAN_RX_THREAD_STACK_SIZE);
K_THREAD_STACK_DEFINE(can_tx1_thread_stack, CAN_TX1_THREAD_STACK_SIZE);
K_THREAD_STACK_DEFINE(can_tx2_thread_stack, CAN_TX2_THREAD_STACK_SIZE);
K_THREAD_STACK_DEFINE(can_tx3_thread_stack, CAN_TX3_THREAD_STACK_SIZE);

struct k_thread can_rx_thread_data;
struct k_thread can_tx1_thread_data;
struct k_thread can_tx2_thread_data;
struct k_thread can_tx3_thread_data;

K_THREAD_DEFINE(can_rx_tid, CAN_RX_THREAD_STACK_SIZE,
                can_rx_thread,
                NULL, NULL, NULL,
                CAN_RX_THREAD_PRIORITY, 0, 0);
K_THREAD_DEFINE(can_tx1_tid, CAN_TX1_THREAD_STACK_SIZE,
                can_tx1_thread,
                NULL, NULL, NULL,
                CAN_TX1_THREAD_PRIORITY, 0, 0);
K_THREAD_DEFINE(can_tx2_tid, CAN_TX2_THREAD_STACK_SIZE,
                can_tx2_thread,
                NULL, NULL, NULL,
                CAN_TX2_THREAD_PRIORITY, 0, 0);
K_THREAD_DEFINE(can_tx3_tid, CAN_TX3_THREAD_STACK_SIZE,
                can_tx3_thread,
                NULL, NULL, NULL,
                CAN_TX3_THREAD_PRIORITY, 0, 0);

/* Threads configuration and creation (ENDS) */

// Default Zephyr CAN device is used (refer to board's dts file)
const struct device *const can_dev = DEVICE_DT_GET(DT_CHOSEN(zephyr_canbus));

/**
  * @brief  Initialize the CAN peripheral and prints instructions
  */
int main(void)
{
	int ret;

	if (!device_is_ready(can_dev)) {
		printk("CAN: Device %s not ready.\n", can_dev->name);
		return 0;
	}

	can_set_bitrate(can_dev, CONFIG_CAN_BITRATE);

	ret = can_start(can_dev);
	if (ret != 0) {
		printk("Error starting CAN controller [%d]", ret);
		return 0;
	}

	printk("Initialization done.\n");
#if IS_ENABLED(CONFIG_START_MSG)
		printk("Send 0x%x CAN ID to start printing (whatever the dlc and data).\n", CONFIG_START_MSG_ID);
#endif
#if IS_ENABLED(CONFIG_STOP_MSG)	
		printk("Send 0x%x CAN ID to stop printing (whatever the dlc and data).\n", CONFIG_STOP_MSG_ID);
#endif
#if IS_ENABLED(CONFIG_HELLO_MSG)	
		printk("Send 0x%x CAN ID to print 'Hello World' (whatever the dlc and data).\n", CONFIG_HELLO_MSG_ID);
#endif
}