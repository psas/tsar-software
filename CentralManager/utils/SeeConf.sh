#!/bin/bash
grep "#define " ../src/*.h | cut -d ' ' -f 2-3 | grep -v ^\_
