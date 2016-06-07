/**
 * Copyright (C) 2016  Jonas Sticha (jonas.sticha@tum.de)
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
 **/

#include <string>
#include <vector>
#include <csignal>
#include <iostream>
#include "GpioReader.h"
#include "PrioritySwitcher.h"
#include "StateChangeCounter.h"

using namespace std;

StateChangeCounter* stateChangeCounter;

void handleSignal(int signalID) {
    stateChangeCounter->printState();
}

int main(int argc, char* argv[]) {
    if(argc != 2) {
        cerr << "ERROR: Wrong number of arguments!" << endl;
        cout << "Usage: " << argv[0] << " gpio_pin_id" << endl;
        return 1;
    }
    cout << "Reading value for GPIO pin " << argv[1] << endl;
    string pindId = string(argv[1]);
    GpioReader* gpioReader = nullptr;
    try {
        gpioReader = new GpioReader(pindId);
    } catch (string error) {
        cerr << "ERROR: " << error << endl;
        return 1;
    }

    PrioritySwitcher prioritySwitcher(false);
    prioritySwitcher.switchToRealtimePriority();

    signal(SIGHUP, handleSignal);
    vector<GpioReader*> gpioReaders;
    gpioReaders.push_back(gpioReader);
    stateChangeCounter = new StateChangeCounter(gpioReaders);
    stateChangeCounter->countStateChanges();
    return 0;
}