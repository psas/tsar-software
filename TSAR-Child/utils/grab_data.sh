#!/bin/bash

#only used to copy all csv files from tsar to host computer

rsync -cvazP tsar@192.168.6.2:~/TSAR-Child/data/* ../../TSAR-Child/data/.
