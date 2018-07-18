/*
 * Copyright (C) 2018 Ola Benderius
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ncurses.h>

#include "cluon-complete.hpp"
#include "opendlv-standard-message-set.hpp"

#include "Ps3Controller.h"

int32_t main(int32_t argc, char **argv) {
  int32_t retCode{0};
  auto commandlineArguments = cluon::getCommandlineArguments(argc, argv);
  if (0 == commandlineArguments.count("cid") ||
      0 == commandlineArguments.count("input") ||
      0 == commandlineArguments.count("freq") ||
      0 == commandlineArguments.count("angleconversion")) {
    std::cerr << argv[0] << " interfaces to the ps3controller for the moby dick." << std::endl;
    std::cerr << "Usage:   " << argv[0] << " --cid=<OpenDaVINCI session> --freq=<hz>--input=<js node> --angleconversion=<const>[--verbose]" << std::endl;
    std::cerr << "Example: " << argv[0] << " --cid=219 --cidpwm=222 --freq=10 --input=/dev/input/js0 --angleconversion=1" << std::endl;
    retCode = 1;
  } else {
    int32_t VERBOSE{commandlineArguments.count("verbose") != 0};
    if (VERBOSE) {
      VERBOSE = std::stoi(commandlineArguments["verbose"]);
    }
    cluon::OD4Session od4{static_cast<uint16_t>(std::stoi(commandlineArguments["cid"])),
      [](auto){}
    };
    cluon::OD4Session od4pwm{static_cast<uint16_t>(std::stoi(commandlineArguments["cidpwm"])),
      [](auto){}
    };
    float const FREQ = std::stof(commandlineArguments["freq"]);
    Ps3Controller ps3controller(commandlineArguments["input"]);
    if (VERBOSE == 2) {
      initscr();
    }
    float const ANGLECONVERSION = std::stof(commandlineArguments["angleconversion"]);

    /*//Enable braking
    cluon::data::TimeStamp sampleTimeInit;
    opendlv::proxy::SwitchStateRequest brakeState;
    brakeState.state(1);
    od4.send(brakeState,sampleTimeInit,1069);*/


    auto atFrequency{[&ps3controller, &ANGLECONVERSION, &VERBOSE, &od4, &od4pwm]() -> bool
    {
      ps3controller.readPs3Controller();
      opendlv::proxy::GroundSpeedRequest ppr = ps3controller.getGroundSpeedRequest();
      opendlv::proxy::GroundSteeringRequest gsr = ps3controller.getGroundSteeringRequest();
      opendlv::proxy::TorqueRequest torqueMsgLeft = ps3controller.getTorqueRequestLeft();
      opendlv::proxy::TorqueRequest torqueMsgRight = ps3controller.getTorqueRequesRight();
      gsr.groundSteering(gsr.groundSteering() * ANGLECONVERSION);
      opendlv::proxy::PulseWidthModulationRequest pwmr = ps3controller.getBrakePwmRequest();
      uint32_t pwmrupdate = (pwmr.dutyCycleNs() + 32767 > 50000)?(50000):(pwmr.dutyCycleNs() + 32767);
      pwmr.dutyCycleNs(pwmrupdate);
      opendlv::proxy::SwitchStateReading goSignal = ps3controller.getGoSignalMsg();
      opendlv::proxy::SwitchStateReading finishSignal =  ps3controller.getFinishSignalMsg();


     cluon::data::TimeStamp sampleTime;
     od4.send(ppr, sampleTime, 0);
     od4.send(gsr, sampleTime, 0);
     od4.send(torqueMsgLeft, sampleTime, 1502);
     od4.send(torqueMsgRight, sampleTime, 1503);
     od4pwm.send(pwmr,sampleTime,1341);
     od4.send(goSignal,sampleTime,1402);
     od4.send(finishSignal,sampleTime,1403);
     if (VERBOSE == 1) {
        std::cout 
            << ps3controller.toString() << std::endl
            << "gsr: " + std::to_string(gsr.groundSteering()) << std::endl
            << "ppr: " + std::to_string(ppr.groundSpeed()) << std::endl
            << "torque left: " + std::to_string(torqueMsgLeft.torque()) << std::endl
            << "torque right: " + std::to_string(torqueMsgRight.torque()) << std::endl
            << "pwmr: " + std::to_string(pwmr.dutyCycleNs()) << std::endl
            << "go: " + std::to_string(goSignal.state()) << std::endl
            << "finish: " + std::to_string(finishSignal.state()) << std::endl;
      }
      if (VERBOSE == 2) {
        mvprintw(1,1,(ps3controller.toString()).c_str()); 
        mvprintw(1,50,("gsr: " + std::to_string(gsr.groundSteering())).c_str()); 
        mvprintw(10,50,("ppr: " + std::to_string(ppr.groundSpeed())).c_str());
        refresh();   
      }

      return true;
    }};
        

    od4.timeTrigger(FREQ, atFrequency);

    while (od4.isRunning()) {

    }
    if (VERBOSE == 2) {
      endwin();     
    }
  }
  return retCode;
}
