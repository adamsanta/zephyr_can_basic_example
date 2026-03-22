#include "can_common.h"
#include <zephyr/random/random.h>

#define CAN_TX1_PERIOD K_MSEC(CONFIG_CAN_TX1_PERIOD_MS)
#define CAN_TX2_PERIOD K_MSEC(CONFIG_CAN_TX2_PERIOD_MS)
#define CAN_TX3_PERIOD K_MSEC(CONFIG_CAN_TX3_PERIOD_MS)

void can_tx1_thread(void *arg1, void *arg2, void *arg3);
void can_tx2_thread(void *arg1, void *arg2, void *arg3);
void can_tx3_thread(void *arg1, void *arg2, void *arg3);