#include <ch.h>
#include <hal.h>
#include "control_panel.h"

static BSEMAPHORE_DECL(breakpoint_semaphore, true);
static bool breakpoints_enabled = false;

static void strategy_debugger_thd(void *p)
{
    (void) p;

    while (true) {
        chThdSleepMilliseconds(200);
        if (control_panel_read(BUTTON_YELLOW) || control_panel_read(BUTTON_GREEN)) {
            chBSemSignal(&breakpoint_semaphore);
        }
    }
}

void strategy_breakpoints_init(void)
{
    static THD_WORKING_AREA(wa, 512);
    chThdCreateStatic(wa, sizeof(wa), NORMALPRIO, strategy_debugger_thd, NULL);
}

void strategy_breakpoints_set_enabled(bool state)
{
    breakpoints_enabled = state;
}

void strategy_breakpoint(void)
{
    if (breakpoints_enabled) {
        NOTICE("Waiting for user break");
        chBSemWait(&breakpoint_semaphore);
    }
}

