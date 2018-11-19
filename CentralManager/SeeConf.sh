#!/bin/bash
grep "#define " *.h | cut -d ' ' -f 2-3 | grep -v ^\_
