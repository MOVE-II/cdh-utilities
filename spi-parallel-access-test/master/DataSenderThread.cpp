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

#include <iostream>
#include "DataSenderThread.h"

DataSenderThread::DataSenderThread(DataSender &dataSender, int numBytes, int numRepetitions) :
    numBytes(numBytes),
    numRepetitions(numRepetitions),
    senderThread(nullptr),
    dataSender(dataSender){
}

void* DataSenderThread::run(DataSender* dataSender, int numBytes, int numRepetitions) {
    try {
        dataSender->sendData(numBytes, numRepetitions);
    } catch (string error) {
        cerr << "ERROR in sendData from thread " << this_thread::get_id() << ": " << error << endl;
    }
    return nullptr;
}

void DataSenderThread::startThread() {
    senderThread = new thread(DataSenderThread::run, &dataSender, numBytes, numRepetitions);
}

void DataSenderThread::join() {
    senderThread->join();
}
