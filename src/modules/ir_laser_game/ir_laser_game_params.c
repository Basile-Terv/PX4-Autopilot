// ir_laser_game_params.c
#include <parameters/param.h>

// IR signal threshold
PARAM_DEFINE_FLOAT(THRESH, 0.7f);

// Stationary flight duration in seconds
PARAM_DEFINE_INT32(DURATION, 5);

// PWM channel for IR emitter
PARAM_DEFINE_INT32(CHANNEL_EMITTER, 6);

// PWM channel for IR receiver
PARAM_DEFINE_INT32(CHANNEL_RECEIVER, 5);

// Switch position for IR trigger
PARAM_DEFINE_INT32(TRIGGER, 1);

// Stationary flight thrust command
PARAM_DEFINE_FLOAT(THRUST, 0.5f);

// Stationary flight roll command
PARAM_DEFINE_FLOAT(ROLL, 0.0f);

// Stationary flight pitch command
PARAM_DEFINE_FLOAT(PITCH, 0.0f);

// Stationary flight yaw command
PARAM_DEFINE_FLOAT(YAW, 0.0f);
