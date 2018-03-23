#ifndef TCS3472_H
#define TCS3472_H

#ifdef __cplusplus
extern "C" {
#endif

#include <hal.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct {
    I2CDriver *i2c;
} TCS3472_t;

void TCS3472_init(TCS3472_t *dev, I2CDriver *i2c);
void TCS3472_configure(TCS3472_t *dev);
bool TCS3472_ping(TCS3472_t *dev);
void TCS3472_read_color(TCS3472_t *dev, uint16_t crgb[4]);

#ifdef __cplusplus
}
#endif

#endif /* TCS3472_H */