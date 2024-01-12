// ir_laser_game.cpp
#include "ir_laser_game.h"
#include <px4_platform_common/log.h>
#include <mathlib/mathlib.h>
#include <px4_platform_common/console/console.h>

IRLaserGameModule::IRLaserGameModule() : ModuleParams(nullptr)
{
    // Initialize variables
    _ir_signal_detected = false;
    _ir_signal_timestamp = 0;

    // Subscribe to relevant topics
    _pwm_input_sub = orb_subscribe(ORB_ID(pwm_input));
    _manual_control_setpoint_sub = orb_subscribe(ORB_ID(manual_control_setpoint));
    // Add additional subscriptions as needed

    // Advertise/Publish topics
    _actuator_controls_pub = orb_advertise(ORB_ID(actuator_controls_0), &actuator_controls);

    // Initialize hardware
    // Example: Configure PWM channels for input/output

    // Set default parameters
    parameters_update(true); // Force update on initialization
}

int IRLaserGameModule::task_spawn(int argc, char *argv[])
{
    return px4_task_spawn_cmd("ir_laser_game",
                              SCHED_DEFAULT,
                              SCHED_PRIORITY_DEFAULT,
                              1024,
                              (px4_main_t)&run_trampoline,
                              (char *const *)argv);
}

IRLaserGameModule *IRLaserGameModule::instantiate(int argc, char *argv[])
{
    IRLaserGameModule *instance = new IRLaserGameModule();

    if (instance == nullptr)
    {
        PX4_ERR("alloc failed");
    }

    return instance;
}

void IRLaserGameModule::run()
{
    printf('--------Application starting--------')
    // Run the loop synchronized to the pwm_input topic publication
    px4_pollfd_struct_t fds[1];
    fds[0].fd = _pwm_input_sub.get_fd();
    fds[0].events = POLLIN;

    while (!should_exit())
    {
        int pret = px4_poll(fds, 1, 1000);

        if (pret == 0)
        {
            // Timeout: let the loop run anyway, don't do `continue` here
        }
        else if (pret < 0)
        {
            PX4_ERR("poll error %d, %d", pret, errno);
            px4_usleep(50000);
            continue;
        }
        else if (fds[0].revents & POLLIN)
        {
            pwm_input_s pwm_input;
            _pwm_input_sub.copy(&pwm_input);

            // Handle PWM inputs : a priori pas nécessaire d'utiliser cette fonction donc je commente
            // handle_pwm_inputs(pwm_input);

            // Handle IR signal detection
            // Example: Assuming PWM channel 5 is connected to the IR receiver
            if (pwm_input.channel_count >= 6)
            {
                // Check if the PWM signal on channel 5 indicates the presence of an IR signal
                if (pwm_input.values[5] > your_ir_signal_threshold)
                {
                    // IR signal detected
                    _ir_signal_detected = true;
                    _ir_signal_timestamp = hrt_absolute_time();
                }
                else
                {
                    // IR signal not detected
                    _ir_signal_detected = false;
                }
            }
        }

        // Handle manual control setpoint
        manual_control_setpoint_s manual_control_setpoint;

        if (_manual_control_setpoint_sub.update(&manual_control_setpoint))
        {
            // Check if a specific switch or button on the radio controller is activated
            // Example: Assuming switch 1 (SWITCH_POS1) is used for triggering the IR emitter
            if (manual_control_setpoint.switches[0] == manual_control_setpoint_s::SWITCH_POS1)
            {
                // Send IR signal
                send_ir_signal();
            }
        }

        // Check if an IR signal was detected and trigger a stationary flight command
        if (_ir_signal_detected && (hrt_elapsed_time(&_ir_signal_timestamp) < 5_s)) {
            // Trigger your stationary flight command here
            PX4_INFO("IR signal detected. Triggering stationary flight command.");
            triggerStationaryFlightCommand();
        } else if (consoleCheck()) {
            // Trigger your IR send signal command when "send_signal" command is received
            PX4_INFO("Console command received: send_signal");
            send_ir_signal();
        }
        else
        {
            // IR signal not detected or timeout expired
            // Add any additional logic or cleanup here
        }

        // TODO: Add any other necessary logic

        // Sleep for a short duration before the next iteration
        px4_usleep(100000);
    }
}

// -----------------------------
// Instead of controlling actuators, use hold command using GPS
void IRLaserGameModule::triggerStationaryFlightCommand()
{
    // Example: Set your desired thrust, roll, pitch, yaw, etc.
    // These are arbitrary values; adjust based on your needs

    // Thrust command (normalized value between 0 and 1)
    float thrust_command = 0.6f; // Example: 60% thrust

    // Roll, pitch, and yaw commands (normalized values between -1 and 1)
    float roll_command = 0.0f;   // Example: No roll
    float pitch_command = 0.0f;  // Example: No pitch
    float yaw_command = 0.5f;    // Example: 50% yaw rate

    actuator_controls_s actuator_controls{};
    actuator_controls.control[actuator_controls_s::INDEX_THROTTLE] = thrust_command;
    actuator_controls.control[actuator_controls_s::INDEX_ROLL] = your_roll_command;
    actuator_controls.control[actuator_controls_s::INDEX_PITCH] = your_pitch_command;
    actuator_controls.control[actuator_controls_s::INDEX_YAW] = your_yaw_command;

    // Publish actuator controls
    orb_publish_auto(ORB_ID(actuator_controls_0), &_actuator_controls_pub, &actuator_controls);
}

bool IRLaserGameModule::consoleCheck()
{
    // Check if there's any data available in the console
    int available = console_read(NULL, 0);

    if (available > 0) {
        char buffer[128];
        int bytes_read = console_read(buffer, sizeof(buffer));

        if (bytes_read > 0) {
            // Null-terminate the string
            buffer[bytes_read] = '\0';

            // Check if the received console command is "send_signal"
            if (strcmp(buffer, "send_signal") == 0) {
                PX4_INFO("Console command received: %s", buffer);
                return true; // Console command "send_signal" received
            }
        }
    }

    return false; // No console command or command other than "send_signal" received
}

void IRLaserGameModule::parameters_update(bool force)
{
    // Check for parameter updates
    if (_parameter_update_sub.updated() || force)
    {
        // Clear update
        parameter_update_s update;
        _parameter_update_sub.copy(&update);

        // Update parameters from storage
        updateParams();
    }
}

void IRLaserGameModule::handle_pwm_inputs(const pwm_input_s &pwm_input)
{
    // Add your PWM input handling logic here
}

void IRLaserGameModule::send_ir_signal()
{
    // Add your code to send an IR signal through the emitter
    PX4_INFO("Sending IR signal");
    // Example: Assuming PWM channel 6 is connected to the IR emitter
    pwm_output_s pwm_output{};
    pwm_output.values[6] = your_ir_emitter_command;
    orb_publish_auto(ORB_ID(pwm_output), &_pwm_output_pub, &pwm_output);
}

int IRLaserGameModule::print_status()
{
    PX4_INFO("Running");
    // Add any additional status information here
    return 0;
}

