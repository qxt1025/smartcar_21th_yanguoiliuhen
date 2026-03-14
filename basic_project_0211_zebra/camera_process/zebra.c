/*
 * zebra.c
 *
 *  Created on: 2024-03-24
 *      Author: hwj
 */
#include "zebra.h"

#define ZEBRA_SCAN_START_LINE   (15)   /* Start line for zebra feature counting */
#define ZEBRA_SCAN_END_LINE     (80)   /* End line (exclusive) for zebra counting */
#define ZEBRA_RUNTIME_MIN_TICK  (10UL) /* Minimum runtime ticks before zebra detect */
#define ZEBRA_STOP_HOLD_COUNT   (10)   /* Zero-speed frames required before stop */

/*
 * Purpose: Detect zebra feature lines and enter zebra element state.
 * Param  : None
 * Return : None
 * Note   : Call in main loop after lineinfo[] has been updated.
 */
void zebra_enter(void)
{
    uint8 y;                    /* Scan row index: [15, 79] */
    uint8 zebra_count;          /* Number of rows marked as zebra */
    int zebra_line_threshold;   /* Entry threshold from set parameters */

    if(mycar.RUNTIME < ZEBRA_RUNTIME_MIN_TICK)
    {
        return;                 /* Ignore startup frames to avoid false trigger */
    }

    zebra_count = 0;
    zebra_line_threshold = setpara.zebra_line_count;

    for(y = ZEBRA_SCAN_START_LINE; y < ZEBRA_SCAN_END_LINE; y++)
    {
        if(lineinfo[y].zebra_flag == 1)
        {
            zebra_count++;
            if(zebra_count > zebra_line_threshold)
            {
                break;          /* Threshold already met, stop scanning early */
            }
        }
    }

    if(zebra_count <= zebra_line_threshold)
    {
        return;                 /* Not enough zebra rows, keep current state */
    }

    clear_all_flags();                               /* Reset other element flags */
    enter_element(zebra);                            /* Switch to zebra element */
    watch.zebra_flag = 1;                            /* Mark zebra phase: entered */
    begin_distant_integeral(setpara.zebra_distance); /* Start distance integral */
}

/*
 * Purpose: Advance zebra state when configured distance integral is finished.
 * Param  : None
 * Return : None
 * Note   : This function only updates zebra state machine flags.
 */
void zebra_stop(void)
{
    if(watch.zebra_flag != 1)
    {
        return;                 /* Only process when zebra phase is active */
    }

    if(get_integeral_state(&distance_integral) != 2)
    {
        return;                 /* Wait until distance integral reaches target */
    }

    watch.zebra_flag = 2;       /* Enter zebra stop-and-hold phase */
}

/*
 * Purpose: Exit zebra state after vehicle speed stays at zero long enough.
 * Param  : None
 * Return : None
 * Note   : Stop decision uses watch.present_speed and stop_count.
 */
void zebra_out(void)
{
    if(watch.zebra_flag != 2)
    {
        return;                 /* Only process during stop-and-hold phase */
    }

    if(watch.present_speed > 0U)
    {
        return;                 /* Vehicle still moving, keep waiting */
    }

    watch.stop_count++;
    if(watch.stop_count < ZEBRA_STOP_HOLD_COUNT)
    {
        return;                 /* Need stable zero speed for enough frames */
    }

    mycar.car_stop = 1;         /* Trigger low-level stop control */
    watch.stop_count = 0;       /* Clear hold counter for next zebra event */
    out_element();              /* Leave zebra element and restore defaults */
}
