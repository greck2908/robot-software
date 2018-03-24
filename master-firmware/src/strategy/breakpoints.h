#ifndef _BREAKPOINTS_H_
#define _BREAKPOINTS_H_

#ifdef __cplusplus
extern "C" {
#endif

void strategy_breakpoints_init(void);
void strategy_breakpoints_set_enabled(bool state);
void strategy_breakpoint(void);

#ifdef __cplusplus
}
#endif

#endif
