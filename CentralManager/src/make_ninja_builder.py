#!/usr/bin/env python3

import sys

'''
This will build a ninja file for different parts of Central Manager based on input.
For people who are unfimilar with ninja-build, it is a every simple and fast assembler.
This and the ninja file it creates are use in the place of a complicated
makefile to allow easy testing of selected parts of the central manager.

to uses this:
    ./make_ninja_builder.py arg1 arg2 args

    arg1: must be "run" or "test"
        "run"   to make ninja file to run the full system and will ignore all other arguments
        "test"  to test a selected class to test
    arg2: is the selected class to test. 
        "sequencer", "hardware", "link", or "server"
    args: is a LIST of special flags for testing, it will ignore all other input.
        "no_ll"             for hardware without the link_logger running.
        "print_hdw_data"    prints sensor data on terminal.
        "sensor_data_off"   make hardware constantly read from a file instead of sensor (to test sequencer).

    examples:
        ./make_ninja_builder.py test server
        ./make_ninja_builder.py test hardware no_link
        ./make_ninja_builder.py run

'''


# the list of sub-class need by selectec class
server = ['server', 'main_class']

link = ['link_logger', 'sequencer_status', 'hardware_data_frame','client_command']
link.extend(server)

hardware = ['hardware/hardware_controller', 'hardware/i2c_library', 'hardware/uart_library']
hardware.extend(link)

sequencer = ['sequencer']
sequencer.extend(hardware)

governor = ['governor']
governor.extend(sequencer)


def main():
    if(len(sys.argv) == 1):
        print("./make_ninja_builder.py run")
        print("                 or")
        print("./make_ninja_builder.py test object test_flags\n")
        print("object:")
        print("  hardware or server")
        print("test_flags:")
        print("  no_ll              -- for hardware without the link_logger running")
        print("  print_hdw_data     -- prints sensor data on terminal")
        print("  sensor_data_off    -- make hardware constantly read from a file instead of sensor (test sequence)")
        return

    if(sys.argv[1] == 'run'):
        file_list = make_file_list('governor') # sequencer includes all file in central manager
        make_ninja_file(file_list)
    elif(sys.argv[1] == 'test' and len(sys.argv) >= 3):
        target = sys.argv[2]
        flag_list = sys.argv[2:]

        main_file = get_main_path(target)
        test_flags = get_test_flags(target, flag_list)
        file_list = make_file_list(target, flag_list)
        make_ninja_file(file_list, main_file, test_flags)
    else:
        print("Input Error:")
        print("run ./make_ninja_builder.py for formating")
        return

    print("build.ninja was made")
    return


# figures out which main file is needed for testing
def get_main_path(target):
    main = ''

    if(target == 'hardware'):
        main = 'tests/hardware_test_main.cpp'
    elif(target == 'server'):
        main = 'tests/server_test_main.cpp'
    else:
        print('Error: arg2 is not hardware or server')
        exit()

    return main


# figures what test flags are needed
def get_test_flags(target, flags=[]):
    test_flags = ''

    # add test flags
    if (len(flags) != 0):
        if ('no_ll' in flags):
            test_flags += '-DLL_OFF '
        if('print_hdw_data' in flags):
            test_flags += '-DPRINT_DATA '
        if('sensor_data_off' in flags):
            test_flags += '-DSENSOR_DATA_OFF '

    return test_flags


# get a list of all *.c needed for compiling from global lists
def make_file_list(target, flags=[]):
    file_list = []

    if(target == 'governor'):
        file_list = governor
    elif(target == 'sequencer'):
        file_list = sequencer
    elif(target == 'hardware'):
        file_list = hardware
    elif(target == 'link'):
        file_list = link
    elif(target == 'server'):
        file_list = server
    else:
        print('Error: arg2 is not sequencer, hardware, link, or server')
        exit()

    return file_list


# creates the build.ninja file
def make_ninja_file(file_list, main_file='', test_flags=''):
    pi_flag = ''
    if(main_file != 'tests/server_test_main.cpp'):
        pi_flag = '-lwiringPi' # needed for hardware

    f = open("build.ninja","w")

    # write header
    f.write("cc = g++\n")
    f.write("cflags = -std=c++14 -pthread -Wall %s\n" % pi_flag)
    f.write("output = CentralManager\n\n")

    # write rules
    f.write("rule compile\n")
    f.write("    command = $cc $cflags -c $in -o $out %s\n\n" % test_flags)
    f.write("rule link\n")
    f.write("    command = $cc $cflags $in -o $output %s\n\n" % test_flags)
    f.write("rule clean_all\n")
    f.write("    command = rm -rf *.o hardware/*.o $output\n\n")

    # write object file builders
    for x in file_list:
        f.write("build %s.o: compile %s.cpp\n" % (x, x))

    # write linker
    f.write("\nbuild run: link %s " % main_file)
    for x in file_list:
        f.write("%s.o " % x)
    
    # clean and default
    f.write("\n\nbuild clean: clean_all\n")
    f.write("\ndefault run\n")
    f.close()


if __name__ == '__main__':
    main()
