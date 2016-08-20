//
//========================================================
// Project	: hpp_tem2ctrl
// File 	: hpp_tem2hex.cpp
// Date		: Oct 15, 2015
// Author	: HuuHoang Nguyen
// Contact	: hhnguyen@apm.com
//          : hoangnh.hpp@gmail.com
//========================================================
//


#include "hpp_tem2hex.h"


//======================================================================
hpp_tem2hex::hpp_tem2hex()
{
	this->crc_start = CRC_START;
	this->xor_const = XOR_CONSTANT;
	this->mask_xor_const = MASK_TO_XOR_CONSTANT;
	this->shift_xor_num = SHIFT_XOR_NUM;
	this->prefix_code =PREFIX_CODE;
	this->prefix_code_byte_num = PREFIX_CODE_BYTE_NUM;
	this->prefix_code_byte_num_max  = PREFIX_CODE_BYTE_NUM_MAX;
	this->hex_str ="\0";
	this->hex_value = 0;
}

//======================================================================
hpp_tem2hex::hpp_tem2hex(hpp_uint16_t crc_start_init,
			    hpp_uint16_t xor_const_init,
				hpp_uint16_t mask_xor_const_init,
				hpp_uint16_t shift_xor_num_init,
				hpp_uint32_t prefix_code_init,
				hpp_uint16_t prefix_code_byte_num_init,
				hpp_uint16_t prefix_code_byte_num_max_init
				)
{
	set_crc_start(crc_start_init);
	set_xor_const(xor_const_init);
	set_mask_xor_const(mask_xor_const_init);
	set_shift_xor_num(shift_xor_num_init);
	set_prefix_code(prefix_code_init);
	set_prefix_code_byte_num(prefix_code_byte_num_init);
	set_prefix_code_byte_num_max(prefix_code_byte_num_max_init);
}

//======================================================================
hpp_uint16_t hpp_tem2hex::get_abs_temp(hpp_sint16_t Temp)
{
	return abs(Temp);
}

//======================================================================
hpp_uint16_t hpp_tem2hex::get_sign_temp(hpp_sint16_t Temp)
{
	return signbit(Temp);
}

//======================================================================
hpp_uint16_t hpp_tem2hex::convert_temp(hpp_uint16_t absTemp, hpp_uint16_t SignTemp)
{
	hpp_uint16_t ConvTemp;
	ConvTemp = absTemp * 10;

	if(SignTemp)
	{
		ConvTemp  = ~ConvTemp;
		ConvTemp += 1;
	}

	return ConvTemp;
}
//======================================================================


//======================================================================
hpp_uint16_t hpp_tem2hex::get_crc_start() const
{
	return this->crc_start;
}

//======================================================================
hpp_uint16_t hpp_tem2hex::get_xor_const() const
{
	return this->xor_const;
}

//======================================================================
hpp_uint16_t hpp_tem2hex::get_mask_xor_const() const
{
	return this->mask_xor_const;
}

//======================================================================
hpp_uint16_t hpp_tem2hex::get_shift_xor_num() const
{
	return this->shift_xor_num;
}

//======================================================================
hpp_uint32_t hpp_tem2hex::get_prefix_code() const
{
	return this->prefix_code;
}

//======================================================================
hpp_uint16_t hpp_tem2hex::get_prefix_code_byte_num() const
{
	return this->prefix_code_byte_num;
}

//======================================================================
hpp_uint16_t hpp_tem2hex::get_prefix_code_byte_num_max() const
{
	return this->prefix_code_byte_num_max;
}

//======================================================================
void hpp_tem2hex::set_crc_start(hpp_uint16_t crc_init)
{
	this->crc_start = crc_init;
}

//======================================================================
void hpp_tem2hex::set_xor_const(hpp_uint16_t xor_init)
{
	this->xor_const = xor_init;
}

//======================================================================
void hpp_tem2hex::set_mask_xor_const(hpp_uint16_t mask_xor_init)
{
	this->mask_xor_const = mask_xor_init;
}

//======================================================================
void hpp_tem2hex::set_shift_xor_num(hpp_uint16_t shift_xor_init)
{
	this->shift_xor_num = shift_xor_init;
}

//======================================================================
void hpp_tem2hex::set_prefix_code(hpp_uint32_t prx_code_init)
{
	this->prefix_code = prx_code_init;
}

//======================================================================
void hpp_tem2hex::set_prefix_code_byte_num(hpp_uint16_t prx_code_init)
{
	this->prefix_code_byte_num = prx_code_init;
}

//======================================================================
void hpp_tem2hex::set_prefix_code_byte_num_max(hpp_uint16_t prx_code_max_init)
{
	this->prefix_code_byte_num_max = prx_code_max_init;
}

//======================================================================
hpp_uint16_t hpp_tem2hex::hpp_tem2hex_crc(hpp_uint16_t temp_x)
{
	hpp_uint16_t iprx_code;
	hpp_uint16_t i_s_xor;
	hpp_uint16_t nbyte_temp;
//	hpp_uint16_t nbyte_remain;
	hpp_uint16_t crc_tmp;

	hpp_uint16_t crc_result = crc_start;

	//CRC to prefix code
	if(HPP_TEM2HEX_DEBUG)
		cout<<"=============================================="<<endl;

	for(iprx_code = 1; iprx_code <= prefix_code_byte_num; iprx_code++)
	{
		if(HPP_TEM2HEX_DEBUG)
			cout<<"iprx_code =" <<iprx_code<<endl;

		crc_result = crc_result ^ ((prefix_code >> ((prefix_code_byte_num-iprx_code) *8)) & 0xFF);


		if(HPP_TEM2HEX_DEBUG)
			cout<<hex<<((prefix_code >> ((prefix_code_byte_num-iprx_code) *8)) & 0xFF)<<endl;


		for(i_s_xor = 0; i_s_xor < shift_xor_num; i_s_xor++)
		{
			if(crc_result & mask_xor_const)
			{
				crc_result = crc_result >> 1;
				crc_result = crc_result ^ xor_const;
			}
			else
			{
				crc_result = crc_result >> 1;
			}
//			cout<<hex<<"crc_result "<<crc_result<<endl;
		}

		if(HPP_TEM2HEX_DEBUG)
			cout<<hex<<crc_result<<endl;
	}

	//CRC to (temperature x 10) in hex
	if(HPP_TEM2HEX_DEBUG)
		cout<<"=============================================="<<endl;
	nbyte_temp = sizeof(temp_x);

	if(HPP_TEM2HEX_DEBUG)
		cout<<"nbyte_temp = "<<nbyte_temp<<endl;

	for(iprx_code = 1; iprx_code <= nbyte_temp; iprx_code++)
	{
		crc_result = crc_result ^ ((temp_x >> ((nbyte_temp- iprx_code) *8)) & 0xFF);

		if(HPP_TEM2HEX_DEBUG)
			cout<<"val_hex = "<<((temp_x >> ((nbyte_temp - iprx_code) *8)) & 0xFF)<<endl;

		for(i_s_xor = 0; i_s_xor < shift_xor_num; i_s_xor++)
		{
			if(crc_result & mask_xor_const)
			{
				crc_result = crc_result >> 1;
				crc_result = crc_result ^ xor_const;
			}
			else
			{
				crc_result = crc_result >> 1;
			}
		}

		if(HPP_TEM2HEX_DEBUG)
			cout<<hex<<crc_result<<endl;
	}

#if 0
	//CRC to PREFIX_CODE_BYTE_NUM_MAX - PREFIX_CODE_BYTE_NUM - nbyte_temp
	cout<<"=============================================="<<endl;
	nbyte_remain = prefix_code_byte_num_max - prefix_code_byte_num - nbyte_temp;
	cout<<"nbyte_remain = "<<nbyte_remain<<endl;

	for(iprx_code = 0; iprx_code < nbyte_remain; iprx_code++)
	{
		crc_result = crc_result ^ (0x0000);

		for(i_s_xor = 0; i_s_xor < SHIFT_XOR_NUM; i_s_xor++)
		{
			if(crc_result & mask_xor_const)
			{
				crc_result = crc_result >> 1;
				crc_result = crc_result ^ xor_const;
			}
			else
			{
				crc_result = crc_result >> 1;
			}
		}
		cout<<hex<<crc_result<<endl;
	}
#endif

	//swap crc result
	crc_tmp = crc_result >> 8;
	crc_result = ((crc_result << 8) & 0xFF00) | crc_tmp;

	if(HPP_TEM2HEX_DEBUG)
		cout<<hex<<"crc_result = " <<crc_result <<endl;

	return crc_result;
}

//======================================================================
hpp_uint64_t hpp_tem2hex::hpp_tem2hex_value(hpp_uint16_t temp_x, hpp_uint16_t crc)
{
	hpp_uint64_t hex_result;
	hex_result = ((hpp_uint64_t)prefix_code << 32) | ((hpp_uint64_t)temp_x << 16) | crc;
	return hex_result;
}
//======================================================================
//======================================================================
//======================================================================


