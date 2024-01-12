// ir_laser_game.h
#pragma once

#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <uORB/Subscription.hpp>
#include <uORB/topics/pwm_input.h>
#include <uORB/topics/pwm_output.h>
#include <uORB/topics/manual_control_setpoint.h>

class IRLaserGameModule : public ModuleBase<IRLaserGameModule>, public ModuleParams
{
public:
    IRLaserGameModule();

    virtual ~IRLaserGameModule() = default;

    /** @see ModuleBase */
    static int task_spawn(int argc, char *argv[]);

    /** @see ModuleBase */
    static IRLaserGameModule *instantiate(int argc, char *argv[]);

    /** @see ModuleBase::run() */
    void run() override;

    /** @see ModuleBase::print_status() */
    int print_status() override;

private:
    void parameters_update(bool force = false);

    uORB::Subscription<pwm_input_s> _pwm_input_sub{ORB_ID(pwm_input)};
    uORB::Subscription<pwm_output_s> _pwm_output_sub{ORB_ID(pwm_output)};
    uORB::Subscription<manual_control_setpoint_s> _manual_control_setpoint_sub{ORB_ID(manual_control_setpoint)};

    // Add your member variables here
    bool _ir_signal_detected{false};
    hrt_abstime _ir_signal_timestamp{0};

    // Add your helper methods here
    void handle_pwm_inputs(const pwm_input_s &pwm_input);
    void handle_manual_control_setpoint(const manual_control_setpoint_s &manual_control_setpoint);
    void send_ir_signal();
};

