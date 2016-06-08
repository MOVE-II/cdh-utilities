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
#include "DataSender.h"
#include "DataSenderThread.h"

using namespace std;

void printUsage(char* argv[]) {
    cout << "Usage: " << argv[0] << " num_bytes repetitions num_threads spi_deviceX.." << endl;
}

int main(int argc, char* argv[]) {
    if(argc < 5) {
        cerr << "Wrong number of arguments!" << endl;
        printUsage(argv);
        return 1;
    }
    int numBytes = atoi(argv[1]);
    if(numBytes > 4096) {
        cerr << "Maximum size for an SPI data transfer is 4096 bytes!" << endl;
        return 1;
    }
    int repetitions = atoi(argv[2]);
    const int numThreads = atoi(argv[3]);
    if(argc != (numThreads+4)) {
        cerr << "An SPI device is needed for each executing thread!" << endl;
        printUsage(argv);
        return 1;
    }
    DataSender* dataSender[numThreads];
    DataSenderThread* dataSenderThread[numThreads];
    for(int i = 0; i < numThreads; i++) {
        dataSender[i] = new DataSender(string(argv[4+i]));
        dataSenderThread[i] = new DataSenderThread(*dataSender[i], numBytes, repetitions);
    }
    for(int i = 0; i < numThreads; i++) {
        dataSenderThread[i]->startThread();
    }
    cout << numThreads << " threads started successfully." << endl;
    for(int i = 0; i < numThreads; i++) {
        dataSenderThread[i]->join();
        cout << "Thread " << i << " completed." << endl;
    }
    return 0;
}