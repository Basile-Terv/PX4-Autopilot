// ir_laser_game.h
#pragma once

#include <px4_platform_common/module.h>
#include <px4_platform_common/module_params.h>
#include <uORB/Subscription.hpp>
#include <uORB/topics/infrared_control.h>  // Include the new message header

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

    uORB::Subscription<infrared_control_s> _infrared_control_sub{ORB_ID(infrared_control)};  // Change the subscription to the new message

    // Add your member variables here
    bool _ir_signal_detected{false};
    hrt_abstime _ir_signal_timestamp{0};

    // Add your helper methods here
    void handle_infrared_control(const infrared_control_s &infrared_control);
    void send_ir_signal();
};
