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

static const I2CConfig i2cconfig = {
    STM32_TIMINGR_PRESC(15U) |
    STM32_TIMINGR_SCLDEL(4U) | STM32_TIMINGR_SDADEL(2U) |
    STM32_TIMINGR_SCLH(15U)  | STM32_TIMINGR_SCLL(21U),
    0,
    0
};

#define TCS3472_I2C_ADDR        (0x29)
#define TCS3472_CMD             (1 << 7)
#define TCS3472_CMD_AUTO_INC    (1 << 5) | TCS3472_CMD
#define TCS3472_CMD_SPECIAL_FN  (3 << 5) | TCS3472_CMD

#define TCS3472_REG_ENABLE      (0x00)
#define TCS3472_REG_ATIME       (0x01)
#define TCS3472_REG_WTIME       (0x03)
#define TCS3472_REG_AILTL       (0x04)
#define TCS3472_REG_AILTH       (0x05)
#define TCS3472_REG_AIHTL       (0x06)
#define TCS3472_REG_AIHTH       (0x07)
#define TCS3472_REG_PERS        (0x0C)
#define TCS3472_REG_CONFIG      (0x0D)
#define TCS3472_REG_CONTROL     (0x0F)
#define TCS3472_REG_ID          (0x12) // 0x44 for TCS34725
#define TCS3472_REG_STATUS      (0x13)
#define TCS3472_REG_CDATAL      (0x14)
#define TCS3472_REG_CDATAH      (0x15)
#define TCS3472_REG_RDATAL      (0x16)
#define TCS3472_REG_RDATAH      (0x17)
#define TCS3472_REG_GDATAL      (0x18)
#define TCS3472_REG_GDATAH      (0x19)
#define TCS3472_REG_BDATAL      (0x1A)
#define TCS3472_REG_BDATAH      (0x1B)

void TCS3472_write_reg(uint8_t reg, uint8_t val)
{
    uint8_t txbuf[2];
    txbuf[0] = reg | TCS3472_CMD_AUTO_INC;
    txbuf[1] = val;
    i2cMasterTransmitTimeout(&I2CD1, TCS3472_I2C_ADDR, txbuf, 2, NULL, 0, TIME_INFINITE);
}

uint8_t TCS3472_read_reg(uint8_t reg)
{
    uint8_t txbuf[1] = {reg | TCS3472_CMD_AUTO_INC};
    uint8_t rxbuf[1];
    i2cMasterTransmitTimeout(&I2CD1, TCS3472_I2C_ADDR, txbuf, 1, rxbuf, 1, TIME_INFINITE);
    return rxbuf[0];
}

void TCS3472_read_color(uint16_t crgb[4])
{
    while ((TCS3472_read_reg(TCS3472_REG_STATUS) & 1) == 0) {
        chThdSleepMilliseconds(1);
    }

    uint8_t txbuf[1] = {TCS3472_REG_CDATAL | TCS3472_CMD_AUTO_INC};
    uint8_t rxbuf[8];

    i2cMasterTransmitTimeout(&I2CD1, TCS3472_I2C_ADDR, txbuf, 1, rxbuf, 8, TIME_INFINITE);

    crgb[0] = rxbuf[0] | ((uint16_t)rxbuf[1]<<8);
    crgb[1] = rxbuf[2] | ((uint16_t)rxbuf[3]<<8);
    crgb[2] = rxbuf[4] | ((uint16_t)rxbuf[5]<<8);
    crgb[3] = rxbuf[6] | ((uint16_t)rxbuf[7]<<8);
}

int main(void)
{
    halInit();
    chSysInit();

    sdStart(&SD2, NULL);
    chprintf(stdout, "boot\n");

    chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

    // PA9  D1 I2C1_SCL AF4
    palSetPadMode(GPIOA, 9, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN);
    // PA10 D0 I2C1_SDA AF4
    palSetPadMode(GPIOA, 10, PAL_MODE_ALTERNATE(4) | PAL_STM32_OTYPE_OPENDRAIN);

    i2cStart(&I2CD1, &i2cconfig);

    uint8_t id = TCS3472_read_reg(TCS3472_REG_ID);

    chprintf(stdout, "TCS3472_REG_ID = 0x%2x\n", id);

    TCS3472_write_reg(TCS3472_REG_ENABLE, 0b00001011); // PON, AEN, WEN -> enable continuous RGBC conversion, with wait
    TCS3472_write_reg(TCS3472_REG_ATIME, 0xF6); // 24 ms integration time
    TCS3472_write_reg(TCS3472_REG_WTIME, 0xAB); // 204 ms wait time
    TCS3472_write_reg(TCS3472_REG_CONFIG, 0x00); // normal wait time
    TCS3472_write_reg(TCS3472_REG_CONTROL, 0b00000010); // 16x gain

    while (true) {
        chThdSleepMilliseconds(100);
        uint16_t crgb[4];
        TCS3472_read_color(crgb);
        chprintf(stdout, "CRGB %u %u %u %u\n", crgb[0], crgb[1], crgb[2], crgb[3]);
    }
}
