#
#========================================================
# Project	: hpp_tem2ctrl
# File 		: Makefile
# Date		: Jan 13, 2016
# Author	: HuuHoang Nguyen
# Contact	: hhnguyen@apm.com
#          	: hoangnh.hpp@gmail.com
#========================================================
#

hpp_tem2ctrl_cross_compiler = clang++
#hpp_tem2ctrl_cross_compiler = g++
hpp_tem2ctrl_path = $(CURDIR)
hpp_tem2ctrl_bin  = bin
hpp_tem2ctrl_obj = $(hpp_tem2ctrl_bin)/hpp_tem2ctrl.o
hpp_tem2ctrl_obj += $(hpp_tem2ctrl_bin)/hpp_common.o
hpp_tem2ctrl_obj += $(hpp_tem2ctrl_bin)/hpp_tem2hex.o
hpp_tem2ctrl_obj += $(hpp_tem2ctrl_bin)/hpp_tem2ctrl_def.o
hpp_tem2ctrl_obj += $(hpp_tem2ctrl_bin)/hpp_tem2ctrl_serial.o

hpp_compile_flag = -Wall -fno-common -std=c++0x 

hpp_tem2ctrl_program = hpp_tem2ctrl

all: clean make_bin_dir $(hpp_tem2ctrl_obj)
	$(hpp_tem2ctrl_cross_compiler) -o $(hpp_tem2ctrl_bin)/$(hpp_tem2ctrl_program)  $(hpp_tem2ctrl_obj)
	@cp $(hpp_tem2ctrl_bin)/$(hpp_tem2ctrl_program) ./
	
	@echo "----------------------------------------------------------------------"
	@echo " Project : hpp_tem2ctrl"
	@echo " Author  : HuuHoang Nguyen (hoangnh.hpp@gmail.com, hhnguyen@apm.com)"
	@echo " Version : 2.00"
	@echo " Date    : Jan 13, 2016"
	@echo " License : MIT"
	@echo " Copyright 2016 HPPTECH"
	@echo "----------------------------------------------------------------------"

$(hpp_tem2ctrl_bin)/hpp_tem2ctrl.o:
	$(hpp_tem2ctrl_cross_compiler) -O $(hpp_compile_flag) -c -o $(hpp_tem2ctrl_bin)/hpp_tem2ctrl.o src/hpp_tem2ctrl.cpp

$(hpp_tem2ctrl_bin)/hpp_common.o:
	$(hpp_tem2ctrl_cross_compiler) -O $(hpp_compile_flag) -c -o $(hpp_tem2ctrl_bin)/hpp_common.o src/hpp_common.cpp

$(hpp_tem2ctrl_bin)/hpp_tem2hex.o:
	$(hpp_tem2ctrl_cross_compiler) -O $(hpp_compile_flag) -c -o $(hpp_tem2ctrl_bin)/hpp_tem2hex.o src/hpp_tem2hex.cpp

$(hpp_tem2ctrl_bin)/hpp_tem2ctrl_def.o:
	$(hpp_tem2ctrl_cross_compiler) -O $(hpp_compile_flag) -c -o $(hpp_tem2ctrl_bin)/hpp_tem2ctrl_def.o src/hpp_tem2ctrl_def.cpp

$(hpp_tem2ctrl_bin)/hpp_tem2ctrl_serial.o: 
	$(hpp_tem2ctrl_cross_compiler) -O $(hpp_compile_flag) -c -o $(hpp_tem2ctrl_bin)/hpp_tem2ctrl_serial.o src/hpp_tem2ctrl_serial.cpp



make_bin_dir:
	@mkdir -p $(hpp_tem2ctrl_bin)
clean:
	@echo "rm -rf $(hpp_tem2ctrl_bin) $(hpp_tem2ctrl_program)"
	@rm -rf $(hpp_tem2ctrl_bin) $(hpp_tem2ctrl_program)
