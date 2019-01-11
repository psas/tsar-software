#!/bin/bash

#only used to copy cpp/h file from pi to host computer

rsync -cvazP pi@192.168.0.14:~/CentralManager/* ../
