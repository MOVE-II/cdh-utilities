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

#ifndef SPI_PARALLEL_ACCESS_TEST_DATASENDERTHREAD_H
#define SPI_PARALLEL_ACCESS_TEST_DATASENDERTHREAD_H

#include <thread>
#include "DataSender.h"

using namespace std;

class DataSenderThread {
private:
    DataSenderThread();
    static void* run(DataSender* dataSender, int numBytes, int numRepetitions);
    int numBytes;
    int numRepetitions;
    thread* senderThread;
    DataSender& dataSender;
public:
    DataSenderThread(DataSender& dataSender, int numBytes, int numRepetitions);
    void startThread();
    void join();
};


#endif //SPI_PARALLEL_ACCESS_TEST_DATASENDERTHREAD_H
