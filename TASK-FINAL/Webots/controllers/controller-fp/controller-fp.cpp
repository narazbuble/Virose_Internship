// Template FP-1
// Referensi model robot: https://webots.cloud/run?version=R2025a&url=https%3A%2F%2Fgithub.com%2Fcyberbotics%2Fwebots%2Fblob%2Freleased%2Fprojects%2Frobots%2Frobotis%2Fdarwin-op%2Fprotos%2FRobotisOp2.proto
// Referensi Api Function: https://cyberbotics.com/doc/reference/nodes-and-api-functions

#include <iostream>
#include <map>
#include <webots/Motor.hpp>
#include <webots/PositionSensor.hpp>
#include <webots/Robot.hpp>

namespace wb = webots;

int main(int argc, char** argv) {
    wb::Robot* robot = new wb::Robot();
    const int timeStep = static_cast<int>(robot->getBasicTimeStep());

    // Map to store all motors
    std::map<std::string, wb::Motor*> motors;
    std::map<std::string, wb::PositionSensor*> positionSensors;

    const char* motorNames[] = {
        // Head motors
        "Head", "Neck",

        "ShoulderL", "ShoulderR",
        "ArmUpperL", "ArmUpperR",
        "ArmLowerL", "ArmLowerR",

        // Paha Z(Belok)
        "PelvYL", "PelvYR",

        // Paha X(Miring)
        "PelvL", "PelvR",

        // Paha atas Y(gerak depan / belakang)
        "LegUpperL", "LegUpperR",

        // Lutut
        "LegLowerL", "LegLowerR",

        // Tumit
        "AnkleL", "AnkleR",

        // Tumit X(miring)
        "FootL", "FootR"};

    // Initialize all motors and position sensors
    int numMotors = sizeof(motorNames) / sizeof(motorNames[0]);
    std::cout << "Initializing " << numMotors << " motors..." << std::endl;

    for (int i = 0; i < numMotors; i++) {
        // Get each motor
        wb::Motor* motor = robot->getMotor(motorNames[i]);
        if (motor) {
            motors[motorNames[i]] = motor;
            std::cout << "[SUCCESS] Motor found: " << motorNames[i] << std::endl;

            // Get corresponding position sensor
            std::string sensorName = std::string(motorNames[i]) + "S";
            wb::PositionSensor* sensor = robot->getPositionSensor(sensorName);
            if (sensor) {
                sensor->enable(timeStep);
                positionSensors[motorNames[i]] = sensor;
                std::cout << "[SUCCESS] Sensor enabled! : " << sensorName << std::endl;
            }
        } else {
            std::cerr << "[FAILED] Motor not found! : " << motorNames[i] << std::endl;
        }
    }

    // Set initial positions for all motors
    std::cout << "\nSetting initial positions..." << std::endl;

    // Main control loop
    while (robot->step(timeStep) != -1) {
        // Read all position sensor values
        std::cout << "\n--- Current Joint Positions ---" << std::endl;
        for (auto& pair : positionSensors) {
            double position = pair.second->getValue();
            std::cout << pair.first << ": " << position << " rad" << std::endl;
        }

        // Control specific motors
        // motors["Neck"]->setPosition(0.5);
        // motors["Head"]->setPosition(0.3);

        break;
    }

    delete robot;
    return 0;
}