#include "can_tx.h"

/**
  * @brief  Transmit a CAN message every CONFIG_CAN_TX1_PERIOD_MS ms
  * with ID=CONFIG_CAN_TX1_ID, DLC=CONFIG_CAN_TX1_DLC and random data.
  */
void can_tx1_thread(void *arg1, void *arg2, void *arg3)
{
	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);

	int i;
	struct can_frame can_tx1_frame = {
		.id = CONFIG_CAN_TX1_ID,
		.dlc = CONFIG_CAN_TX1_DLC,
		.flags = CAN_FILTER_IDE,
	};

	while(1){
		for (i=0;i<can_tx1_frame.dlc;i++){
			// Generate random data
    		can_tx1_frame.data[i] = sys_rand8_get();
		}
		can_send(can_dev, &can_tx1_frame, K_FOREVER, NULL, NULL);
		k_sleep(CAN_TX1_PERIOD);
	}
}

/**
  * @brief  Transmit a CAN message every CONFIG_CAN_TX2_PERIOD_MS ms
  * with ID=CONFIG_CAN_TX2_ID, DLC=CONFIG_CAN_TX2_DLC and random data.
  */
void can_tx2_thread(void *arg1, void *arg2, void *arg3)
{
	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);

	int i;
	struct can_frame can_tx2_frame = {
		.id = CONFIG_CAN_TX2_ID,
		.dlc = CONFIG_CAN_TX2_DLC,
		.flags = CAN_FILTER_IDE,
	};

	while(1){
		for (i=0;i<can_tx2_frame.dlc;i++){
			// Generate random data
    		can_tx2_frame.data[i] = sys_rand8_get();
		}
		can_send(can_dev, &can_tx2_frame, K_FOREVER, NULL, NULL);
		k_sleep(CAN_TX2_PERIOD);
	}
}

/**
  * @brief  Transmit a CAN message every CONFIG_CAN_TX3_PERIOD_MS ms
  * with ID=CONFIG_CAN_TX3_ID, DLC=CONFIG_CAN_TX3_DLC and random data.
  */
void can_tx3_thread(void *arg1, void *arg2, void *arg3)
{
	ARG_UNUSED(arg1);
	ARG_UNUSED(arg2);
	ARG_UNUSED(arg3);

	int i;
	struct can_frame can_tx3_frame = {
		.id = CONFIG_CAN_TX3_ID,
		.dlc = CONFIG_CAN_TX3_DLC,
		.flags = CAN_FILTER_IDE,
	};

	while(1){
		for (i=0;i<can_tx3_frame.dlc;i++){
			// Generate random data
    		can_tx3_frame.data[i] = sys_rand8_get();
		}
		can_send(can_dev, &can_tx3_frame, K_FOREVER, NULL, NULL);
		k_sleep(CAN_TX3_PERIOD);
	}
}