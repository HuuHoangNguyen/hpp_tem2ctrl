//
//========================================================
// Project	: hpp_tem2ctrl
// File 	: hpp_tem2ctrl.h
// Date		: Oct 15, 2015
// Author	: HuuHoang Nguyen
// Contact	: hhnguyen@apm.com
//          : hoangnh.hpp@gmail.com
//========================================================
//

#ifndef HPP_TEM2CTRL_H_
#define HPP_TEM2CTRL_H_
//======================================================================

#include "hpp_common.h"
#include "hpp_tem2hex.h"
#include "hpp_tem2ctrl_serial.h"
#include "hpp_tem2ctrl_def.h"

using namespace std;

//======================================================================
#define HPP_TEM2CTRL_DEBUF	0

//======================================================================
const serial_baudrate_t	HPP_TEM2CTRL_BAUD_RATE	= BAUD_19200;
const character_size_t	HPP_TEM2CTRL_CHAR_SIZE	= CHAR_SIZE_8;
const stop_bit_t		HPP_TEM2CTRL_STOP_BIT	= STOP_BITS_1;
const parity_t			HPP_TEM2CTRL_PARITY		= PARITY_NONE;
const flow_ctrl_t		HPP_TEM2CTRL_FLOW_CTRL	= FLOW_CTRL_NONE;


//======================================================================
extern hpp_tem2ctrl tem2ctrl;

//======================================================================
int main(int argc, char **argv);

//======================================================================
#endif /* HPP_TEM2CTRL_H_ */
