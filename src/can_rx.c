#include "can_rx.h"

// can_rx_msgq is a queue storing the received CAN frames.
// 2 as a maximum number of frames in the queue is enough for manual testing.
// It might need to be increased in case of heavy CAN bus activity.
CAN_MSGQ_DEFINE(can_rx_msgq, 2);

/**
  * @brief  Print information about a received CAN frame if enabled
  * with CONFIG_START_MSG_ID, print 'Hello World' if requested
  * with CONFIG_HELLO_MSG_ID, or disable printing with CONFIG_STOP_MSG_ID.
  * @param  frame: Frame for which to print the information.
  * @param  printing_enabled: true if printing enabled, false if disabled.
  */
void process_can_frame(struct can_frame *frame, bool *printing_enabled){

#if IS_ENABLED(CONFIG_START_MSG)
	if (frame->id == CONFIG_START_MSG_ID){
		*printing_enabled = true;
	}
#endif
	
#if IS_ENABLED(CONFIG_STOP_MSG)
	if(frame->id == CONFIG_STOP_MSG_ID){
		*printing_enabled = false;
	}
#endif

	if (*printing_enabled){
		// Get timestamp
		uint32_t ts = k_uptime_get_32();

#if IS_ENABLED(CONFIG_HELLO_MSG)
		if (frame->id == CONFIG_HELLO_MSG_ID){
			printk("Hello World\n");
		}
#endif

		// Print CAN frame information with timestamp
		printk("[%u ms] ID:0x%03X DLC:%d DATA:", ts, frame->id, frame->dlc);
		for (int i = 0; i < frame->dlc; i++) {
			printk("%02X ", frame->data[i]);
		}
		printk("\n");
	}
}

/**
  * @brief  Receive any CAN frame and process it with process_can_frame.
  */
void can_rx_thread(void *arg1, void *arg2, void *arg3)
{
	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);

	int filter_id;
	bool printing_enabled;
	struct can_frame frame;

	// Filters define the types of CAN frame to accept.
	// We want anyframe: with standard and extended IDs.
	struct can_filter std_id_filter = {
		.id = 0,
		.mask = 0,
	};
	struct can_filter ext_id_filter = {
		.id = 0,
		.mask = 0,
		.flags = CAN_FILTER_IDE,
	};

	filter_id = can_add_rx_filter_msgq(can_dev, &can_rx_msgq, &std_id_filter);
	if (filter_id < 0) {
		printk("Failed to add standard ID CAN filter\n");
	}

	filter_id = can_add_rx_filter_msgq(can_dev, &can_rx_msgq, &ext_id_filter);
	if (filter_id < 0) {
		printk("Failed to add extended ID CAN filter\n");
	}

	// If START message is undefined, printing starts immediately.
#if IS_ENABLED(CONFIG_START_MSG)
	printing_enabled = false;
#else
	printing_enabled = true;
#endif

    while (1) {
        if (k_msgq_get(&can_rx_msgq, &frame, K_FOREVER) == 0) {
			process_can_frame(&frame, &printing_enabled);
        }
		else{
			printk("CAN message reception with k_msgq_get raised an error\n");
		}
    }
}