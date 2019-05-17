#!/bin/bash

#only used to copy cpp/h file to bbb from host computer

rsync -cvazP ../../TSAR-Child tsar@192.168.7.2:~/.
