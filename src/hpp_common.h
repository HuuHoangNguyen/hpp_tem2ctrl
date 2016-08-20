//
//========================================================
// Project	: hpp_tem2ctrl
// File 	: hpp_common.h
// Date		: Oct 15, 2015
// Author	: HuuHoang Nguyen
// Contact	: hhnguyen@apm.com
//          : hoangnh.hpp@gmail.com
//========================================================
//

#ifndef HPP_COMMON_H_
#define HPP_COMMON_H_

//======================================================================
#include <iostream>
#include <iomanip>
#include <cstdlib>
#include <cmath>
#include <cstring>
#include <ctime>

using namespace std;

#define HPP_TEM2CTRL_VERSION		"2.00"
#define GET_VAL_BUFF_MAX			256
#define HPP_COMMON_STR2VAL_DEBUG	0

//======================================================================
typedef unsigned char					hpp_uint08_t;
typedef char							hpp_sint08_t;
typedef unsigned short					hpp_uint16_t;
typedef short							hpp_sint16_t;
typedef unsigned int					hpp_uint32_t;
typedef int								hpp_sint32_t;
typedef unsigned long long				hpp_uint64_t;
typedef long long						hpp_sint64_t;

//======================================================================
typedef enum
{
	RET_OK				= 0,
	RET_ERROR			= 1,
	RET_INVALID_PARAM	= 2,
	RET_NO_RSC			= 3,
	RET_TIME_OUT		= 4,
	RET_FATAL			= 5,
	RET_FAIL			= 6,
	RET_NO_MEM			= 7
}ret_t;

//======================================================================
typedef enum
{
	OPT_RD		= 0,
	OPT_WR		= 1,
	OPT_RW		= 2,
	OPT_MAX		= 3
}option_t;

//======================================================================
class hpp_common
{
public:
	void delay_for(hpp_uint32_t val);
	void version(void);
#if 1
	ret_t str2val(char* s, hpp_sint32_t& val );
	ret_t str2val(char* s, hpp_sint16_t& val );
#else
	template <typename T>
	ret_t str2val(char* s, T& val );
#endif

	ret_t get_val(hpp_sint32_t& val);
protected:
private:
};

extern clock_t	Start,Stop;
extern hpp_common bs;
void hpp_delay_us(double time);
void hpp_delay_ms(double time);

//======================================================================
#endif /* HPP_COMMON_H_ */
