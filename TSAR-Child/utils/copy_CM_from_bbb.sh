#!/bin/bash

#only used to copy cpp/h file from tsar to host computer

rsync -cvazP tsar@192.168.6.2:~/. ../../TSAR-Child
