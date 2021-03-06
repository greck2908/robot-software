#ifndef PRIORITIES_H_
#define PRIORITIES_H_

/* Higher number -> higher priority */

/* lwip threads */
#define TCPIP_THREAD_PRIO                       (NORMALPRIO)
#define LWIP_THREAD_PRIO                        (NORMALPRIO)
#define DEFAULT_THREAD_PRIO                     (NORMALPRIO)

#define USB_SHELL_PRIO                          (NORMALPRIO)
#define UAVCAN_PRIO                             (NORMALPRIO)
#define RPC_SERVER_PRIO                         (NORMALPRIO)
#define POSITION_MANAGER_PRIO                   (NORMALPRIO)
#define BASE_CONTROLLER_PRIO                    (NORMALPRIO)
#define TRAJECTORY_MANAGER_PRIO                 (NORMALPRIO)
#define MAP_SERVER_PRIO                         (NORMALPRIO+1)
#define COLOR_SEQUENCE_SERVER_PRIO              (NORMALPRIO)
#define ARMS_CONTROLLER_PRIO                    (NORMALPRIO)
#define ARM_TRAJ_MANAGER_PRIO                   (NORMALPRIO)
#define LEVER_MODULE_PRIO                       (NORMALPRIO)
#define BALLGUN_MODULE_PRIO                     (NORMALPRIO)
#define BALL_SENSE_PRIO                         (NORMALPRIO)
#define STRATEGY_PRIO                           (NORMALPRIO)
#define SCORE_COUNTER_PRIO                      (NORMALPRIO)
#define ENCODER_PRIO                            (NORMALPRIO)
#define STREAM_PRIO                             (NORMALPRIO)

#define STM32_USB_OTG_THREAD_PRIO           LOWPRIO

#endif
