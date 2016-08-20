//
//========================================================
// Project	: hpp_tem2ctrl
// File 	: hpp_tem2ctrl_def.h
// Date		: Oct 15, 2015
// Author	: HuuHoang Nguyen
// Contact	: hhnguyen@apm.com
//          : hoangnh.hpp@gmail.com
//========================================================
//

#ifndef SRC_HPP_TEM2CTRL_DEF_H_
#define SRC_HPP_TEM2CTRL_DEF_H_
//======================================================================

#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <string>
#include <cctype>
#include <ostream>
#include <fstream>
#include <iosfwd>
#include <ios>
#include <sstream>
#include <ctime>

#include "hpp_common.h"
#include "hpp_tem2hex.h"
#include "hpp_tem2ctrl_serial.h"

using namespace std;

//======================================================================
#define STRING_SIZE_DEFAULT					256
#define TEMP_P_MAX							110
#define TEMP_N_MAX							-30

#define WRITE_TEMP_LOOP_MAX					500

//======================================================================

//======================================================================
#define  HPP_STRING_HELP_DEBUG			1
#define  HPP_TEM2CTRL_DEF_DEBUG			0
#define  HPP_TEM2CTRL_DEF_DATA_DEBUG	0

//======================================================================
const string hpp_tem2ctrl_help_str[]=
{
	"\n----------------------------------------------------------------------",
	"Help :",
	"  Case 1: Set temperature to thermal controller\n",
	"       # ./hpp_tem2ctrl ttyUSB<n> <tempeature> [w/W]",
	"",
	"  Case 2: Read temperature from thermal controller \n",
	"       # ./hpp_tem2ctrl ttyUSB<n>  [r]",
	"",
	"  Case 3: Display to converted a/some temperature to hex value\n",
	"       # ./hpp_tem2ctrl <tempeature> [num-temp]",
	"",
	"       ttyUSB<n>     : Serial port, example: ttyUSB0, ttyUSB1, ttyUSB2 ...",
	"       <temperature> : Temperature [-30 C -> 110 C]",
	"       [w]           : Write temperature to serial port",
	"                     : We can not need pressed the argument",
	"",
	"       [r]           : Read temperature from serial port",
	"                     : We can not need pressed the argument",
	"",
	"       [num-temp]    : Number of temperature need converted to hex value",
	"                     : If not press the argument, then default is 1",
	"\n----------------------------------------------------------------------",
	" Author : HuuHoang Nguyen (hoangnh.hpp@gmail.com, hhnguyen@apm.com)",
	" Version: 2.00",
	" Date   : Jan 05, 2016",
	" License: MIT ",
	" Copyright 2016 HPPTECH ",
	"----------------------------------------------------------------------\n",
};

const int hpp_tem2ctrl_help_str_sise = sizeof(hpp_tem2ctrl_help_str)/sizeof(string);

typedef enum PROGRMAM_MOD
{
	PROG_MODE_NONE = 0,
	PROG_MODE_DUMP = 1,
	PROG_MODE_RUN = 2
}program_mod_t;


class hpp_tem2ctrl:public hpp_common, public hpp_tem2hex, public serial_port
{
public:

	hpp_tem2ctrl();
	hpp_tem2ctrl(hpp_sint16_t Temp_N_Limit, hpp_sint16_t 	Temp_P_Limit);
	~hpp_tem2ctrl();

	 ret_t 		dump();
	 ret_t 		run(const serial_baudrate_t BaudRate,
				    const character_size_t CharSize,
				    const stop_bit_t StopBit,
				    const parity_t Parity,
				    const flow_ctrl_t FlowCtrl);

	 ret_t 		check_arg(int argc, char** argv);
	 ret_t		set_arg(int argc, char** argv);

	 string 	hex2str(const hpp_uint64_t HexValue);


	 void 		set_dev(const string StrDevice);
	 string		get_dev() const;

	 hpp_sint16_t 	get_arg_temp() const;
	 hpp_sint16_t 	get_arg_temp_num() const;
	 option_t		get_arg_opt() const;
	 program_mod_t	get_prog_mod() const;


protected:

private:

	int str_wr_site;
	int	str_rd_site;
	string 			str_wr;
	string 			str_rd;
	string 			str_dev;
	hpp_sint16_t	arg_temp;
	hpp_sint16_t	arg_temp_num;
	option_t		arg_opt;
	program_mod_t	prog_mod;

	hpp_sint16_t 	temp_n_limit;
	hpp_sint16_t 	temp_p_limit;

	void set_arg_temp(hpp_sint16_t ArgTem);
	void set_arg_opt(option_t ArgOpt);
	void set_arg_temp_num(hpp_sint16_t ArgTemNum);


	void help() const;

};


#endif /* SRC_HPP_TEM2CTRL_DEF_H_ */
