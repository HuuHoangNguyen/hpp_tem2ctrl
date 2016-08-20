//
//========================================================
// Project	: hpp_tem2ctrl
// File 	: hpp_tem2hex.h
// Date		: Oct 15, 2015
// Author	: HuuHoang Nguyen
// Contact	: hhnguyen@apm.com
//          : hoangnh.hpp@gmail.com
//========================================================
//


#ifndef HPP_TEM2HEX_H_
#define HPP_TEM2HEX_H_

#include "hpp_common.h"

using namespace std;

#define CRC_START					0xFFFF
#define XOR_CONSTANT				0xA001
#define MASK_TO_XOR_CONSTANT		0x1
#define SHIFT_XOR_NUM				8
#define PREFIX_CODE					0x01060006
#define	PREFIX_CODE_BYTE_NUM		4
#define PREFIX_CODE_BYTE_NUM_MAX	16

#define HPP_TEM2HEX_DEBUG			0



class hpp_tem2hex
{
public:
	hpp_tem2hex();
	hpp_tem2hex(hpp_uint16_t crc_start_init,
			    hpp_uint16_t xor_const_init,
				hpp_uint16_t mask_xor_const_init,
				hpp_uint16_t shift_xor_num_init,
				hpp_uint32_t prefix_code_init,
				hpp_uint16_t prefix_code_byte_num_init,
				hpp_uint16_t prefix_code_byte_num_max_init
				);


	hpp_uint16_t get_abs_temp(hpp_sint16_t Temp);
	hpp_uint16_t get_sign_temp(hpp_sint16_t Temp);
	hpp_uint16_t convert_temp(hpp_uint16_t absTemp, hpp_uint16_t SignTemp);

	hpp_uint16_t get_crc_start() const;
	hpp_uint16_t get_xor_const() const;
	hpp_uint16_t get_mask_xor_const() const;
	hpp_uint16_t get_shift_xor_num() const;
	hpp_uint32_t get_prefix_code() const;
	hpp_uint16_t get_prefix_code_byte_num() const;
	hpp_uint16_t get_prefix_code_byte_num_max() const;

	hpp_uint16_t hpp_tem2hex_crc(hpp_uint16_t temp_x);
	hpp_uint64_t hpp_tem2hex_value(hpp_uint16_t temp_x, hpp_uint16_t crc);


protected:
private:
	hpp_uint16_t	crc_start;
	hpp_uint16_t	xor_const;
	hpp_uint16_t	mask_xor_const;
	hpp_uint16_t	shift_xor_num;
	hpp_uint32_t	prefix_code;
	hpp_uint16_t 	prefix_code_byte_num;
	hpp_uint16_t	prefix_code_byte_num_max;

	hpp_uint16_t 	hex_value;
	string			hex_str;

	void set_crc_start(hpp_uint16_t);
	void set_xor_const(hpp_uint16_t);
	void set_mask_xor_const(hpp_uint16_t);
	void set_shift_xor_num(hpp_uint16_t) ;
	void set_prefix_code(hpp_uint32_t);
	void set_prefix_code_byte_num(hpp_uint16_t);
	void set_prefix_code_byte_num_max(hpp_uint16_t);
};





#endif /* HPP_TEM2HEX_H_ */
