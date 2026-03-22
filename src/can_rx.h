#include "can_common.h"

void process_can_frame(struct can_frame *frame, bool *printing_enabled);
void can_rx_thread(void *arg1, void *arg2, void *arg3);