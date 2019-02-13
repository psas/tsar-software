#!/bin/bash

#only used to copy cpp/h file to pi from host computer

rsync -cvazP ../../CentralManager pi@192.168.0.14:~/.
