#include <zephyr/ztest.h>

#include "can_rx.h"

ZTEST_SUITE(test_can_rx, NULL, NULL, NULL, NULL, NULL);

/**
 * @brief Test the START printing message.
 * Verify that printing is enabled after receiving a CONFIG_START_MSG_ID frame.
 * Whatever the DLC.
 */
#if IS_ENABLED(CONFIG_START_MSG)
ZTEST(test_can_rx, test_start_printing)
{
    struct {
        uint8_t dlc;
        bool printing_enabled;
    } cases[] = {
        {0, true}, {0, false},
        {1, true}, {1, false},
        {2, true}, {2, false},
        {3, true}, {3, false},
        {4, true}, {4, false},
        {5, true}, {5, false},
        {6, true}, {6, false},
        {7, true}, {7, false},
        {8, true}, {8, false},
    };

    struct can_frame frame;
    bool printing_enabled;

    frame.flags = CAN_FILTER_IDE;

    for (int i = 0; i < ARRAY_SIZE(cases); i++) {

        frame.id = CONFIG_START_MSG_ID;
        frame.dlc = cases[i].dlc;
        printing_enabled = cases[i].printing_enabled;

        process_can_frame(&frame, &printing_enabled);

        zassert_true(printing_enabled, "Printing should be enabled after a START frame.");
    }
}
#endif

/**
 * @brief Test the STOP printing message.
 * Verify that printing is disabled after receiving a CONFIG_STOP_MSG_ID frame.
 * Whatever the DLC.
 */
#if IS_ENABLED(CONFIG_STOP_MSG)
ZTEST(test_can_rx, test_stop_printing)
{
    struct {
        uint8_t dlc;
        bool printing_enabled;
    } cases[] = {
        {0, true}, {0, false},
        {1, true}, {1, false},
        {2, true}, {2, false},
        {3, true}, {3, false},
        {4, true}, {4, false},
        {5, true}, {5, false},
        {6, true}, {6, false},
        {7, true}, {7, false},
        {8, true}, {8, false},
    };

    struct can_frame frame;
    bool printing_enabled;

    frame.flags = CAN_FILTER_IDE;

    for (int i = 0; i < ARRAY_SIZE(cases); i++) {

        frame.id = CONFIG_STOP_MSG_ID;
        frame.dlc = cases[i].dlc;
        printing_enabled = cases[i].printing_enabled;

        process_can_frame(&frame, &printing_enabled);

        zassert_false(printing_enabled, "Printing should be disabled after a STOP frame.");
    }
}
#endif

/**
 * @brief Test a sequence of START and STOP printing messages.
 * Verify that START and STOP messages still work when received
 * successively.
 */
#if (IS_ENABLED(CONFIG_START_MSG) && IS_ENABLED(CONFIG_STOP_MSG))
ZTEST(test_can_rx, test_stop_start_stop_sequence)
{
    struct can_frame frame;
    bool printing_enabled = true;

    frame.flags = CAN_FILTER_IDE;

    for (int i = 0; i<9; i++){
        frame.id = CONFIG_STOP_MSG_ID;
        frame.dlc = i;
        process_can_frame(&frame, &printing_enabled);
        zassert_false(printing_enabled, "Printing should be disabled after a STOP frame.");

        frame.id = CONFIG_START_MSG_ID;
        frame.dlc = i;
        process_can_frame(&frame, &printing_enabled);
        zassert_true(printing_enabled, "Printing should be enabled after a STOP-START sequence.");

        frame.id = CONFIG_STOP_MSG_ID;
        frame.dlc = i;
        process_can_frame(&frame, &printing_enabled);
        zassert_false(printing_enabled, "Printing should be disabled after a STOP-START-STOP sequence.");
    }
}
#endif

/**
 * @brief Test effect of other messages than START and STOP.
 * Verify that other messages than START and STOP messages do
 * not change the enabling of printing.
 */
ZTEST(test_can_rx, test_unknown_messages_do_not_change_state)
{
    struct can_frame frame;
    bool printing_enabled;

    frame.flags = CAN_FILTER_IDE;

    // Go through all the possible CAN IDs except START and STOP IDs.
    // (IDs range has been truncated to 0x400 to limit the tests run duration)
    // for(int i=0; i<=0x1FFFFFFF; i++){
    for(int i=0; i<=0x400; i++){
#if IS_ENABLED(CONFIG_START_MSG)
        if (i == CONFIG_START_MSG_ID){
            continue;
        }
#endif
#if IS_ENABLED(CONFIG_STOP_MSG)
        if (i == CONFIG_STOP_MSG_ID){
            continue;
        }
#endif

        frame.id = i;
        for(int j=0; j<9; j++){
            printing_enabled = true;
            frame.dlc = j;
            process_can_frame(&frame, &printing_enabled);
            zassert_true(printing_enabled, "CAN message with ID=0x%x shoud not disable printing.", frame.id);
        }

        frame.id = i;
        for(int j=0; j<9; j++){
            printing_enabled = false;
            frame.dlc = j;
            process_can_frame(&frame, &printing_enabled);
            zassert_false(printing_enabled, "CAN message with ID=0x%x shoud not enable printing.", frame.id);
        }
    }
}
