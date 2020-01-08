#!/bin/bash
grep "#define " ../src/*.h | cut -d ' ' -f 2- | cut -d '/' -f 1 | grep -v ^\_
