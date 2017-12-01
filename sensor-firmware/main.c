#include <ch.h>
#include <hal.h>
#include <chprintf.h>

static THD_WORKING_AREA(waThread1, 128);
static THD_FUNCTION(Thread1, arg)
{
    (void)arg;
    chRegSetThreadName("blinker");
    while (true) {
        palClearLine(LINE_LED_GREEN);
        chThdSleepMilliseconds(500);
        palSetLine(LINE_LED_GREEN);
        chThdSleepMilliseconds(500);
    }
}

BaseSequentialStream *stdout = (BaseSequentialStream *)&SD2;

int main(void)
{
    halInit();
    chSysInit();

    sdStart(&SD2, NULL);
    chprintf(stdout, "boot\n");

    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

    while (true) {
        chThdSleepMilliseconds(500);
    }
}
