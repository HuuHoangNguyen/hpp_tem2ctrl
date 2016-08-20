//
//========================================================
// Project	: hpp_tem2ctrl
// File 	: hpp_tem2ctrl_serial.h
// Date		: Oct 15, 2015
// Author	: HuuHoang Nguyen
// Contact	: hhnguyen@apm.com
//          : hoangnh.hpp@gmail.com
//========================================================
//

#ifndef SRC_HPP_TEM2CTRL_SERIAL_H_
#define SRC_HPP_TEM2CTRL_SERIAL_H_

#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstdlib>
#include <cstring>
#include <vector>
#include <stack>
#include <stdexcept>
#include <termios.h>
#include <string>
#include <cerrno>
#include <fstream>
#include <unistd.h>
#include <fcntl.h>


#include "hpp_common.h"


using namespace std;
//======================================================================
#define HPP_TEM2CTRL_SERIAL_DEBUG				0

//======================================================================
const unsigned char HPP_TEM2CTRL_CMD_READ_TEMP[]	=	"\x01\x03\x00\x00\x00\x01\x84\x0A";

const string SP_ERR_NOT_OPEN				= "Serial port : Not opened.";
const string SP_ERR_ALREADY_OPEN			= "Serial port : Already opened.";
const string SP_ERR_UNSUPPORTED_BAUD		= "Serial port : Unsupported baud rate.";
const string SP_ERR_UNKNOW_BAUD				= "Serial port : Unknown baud rate.";
const string SP_INVALID_PARITY				= "Serial port : Invalid parity setting.";
const string SP_INVALIS_STOP_BIT			= "Serial port : Invalid number of stop bit.";
const string SP_INVALID_FLOW_CTRL			= "Serial port : Invalid flow control.";


//======================================================================
typedef enum
{
    BAUD_50      = B50,
    BAUD_75      = B75,
    BAUD_110     = B110,
    BAUD_134     = B134,
    BAUD_150     = B150,
    BAUD_200     = B200,
    BAUD_300     = B300,
    BAUD_600     = B600,
    BAUD_1200    = B1200,
    BAUD_1800    = B1800,
    BAUD_2400    = B2400,
    BAUD_4800    = B4800,
    BAUD_9600    = B9600,
    BAUD_19200   = B19200,
    BAUD_38400   = B38400,
    BAUD_57600   = B57600,
    BAUD_115200  = B115200,
    BAUD_230400  = B230400,
    //

#ifdef __linux__
    BAUD_460800  = B460800,
    BAUD_500000  = B500000,
    BAUD_576000  = B576000,
    BAUD_921600  = B921600,
    BAUD_1000000 = B1000000,
    BAUD_1152000 = B1152000,
    BAUD_1500000 = B1500000,
    BAUD_2000000 = B2000000,
    BAUD_2500000 = B2500000,
    BAUD_3000000 = B3000000,
    BAUD_3500000 = B3500000,
    BAUD_4000000 = B4000000,
#endif

	BAUD_DEFAULT = BAUD_19200
}serial_baudrate_t;

//======================================================================
typedef enum
{
    CHAR_SIZE_5  = CS5, //!< 5 bit characters.
    CHAR_SIZE_6  = CS6, //!< 6 bit characters.
    CHAR_SIZE_7  = CS7, //!< 7 bit characters.
    CHAR_SIZE_8  = CS8, //!< 8 bit characters.
    CHAR_SIZE_DEFAULT = CHAR_SIZE_8
} character_size_t;


//======================================================================
typedef enum
{
    STOP_BITS_1,   //! 1 stop bit.
    STOP_BITS_2,   //! 2 stop bits.
    STOP_BITS_DEFAULT = STOP_BITS_1
} stop_bit_t;

//======================================================================
typedef enum
{
    PARITY_EVEN,     //!< Even parity.
    PARITY_ODD,      //!< Odd parity.
    PARITY_NONE,     //!< No parity i.e. parity checking disabled.
    PARITY_DEFAULT = PARITY_NONE
} parity_t;

//======================================================================
typedef enum
{
    FLOW_CTRL_HARD,
    FLOW_CTRL_SOFT,
    FLOW_CTRL_NONE,
    FLOW_CTRL_DEFAULT = FLOW_CTRL_NONE
} flow_ctrl_t;

//======================================================================
//======================================================================
//======================================================================

//======================================================================
class serial_port : public std::streambuf
{
public:
	serial_port();
	serial_port(string,
			    serial_baudrate_t,
				character_size_t,
				stop_bit_t,
				parity_t,
				flow_ctrl_t);

	~serial_port();

	//Some function to get error string
	//-------------------------------------------------------------------------
	class sp_err_not_open:public logic_error
	{
	public:
		sp_err_not_open(const string & msg_err): logic_error(msg_err) {};
	};
	//-------------------------------------------------------------------------
	class sp_err_open_failed: public runtime_error
	{
	public:
		sp_err_open_failed(const string & msg_err):runtime_error(msg_err) {};
	};
	//-------------------------------------------------------------------------
	class sp_err_already_open : public logic_error
	{
	public:
		sp_err_already_open(const string & msg_err) : logic_error(msg_err) {};
	};
	//-------------------------------------------------------------------------
	class sp_err_unsupport_baud:  public runtime_error
	{
	public:
		sp_err_unsupport_baud(const string & msg_err) : runtime_error(msg_err) {};
	};
	//-------------------------------------------------------------------------
	class sp_err_read_timeout : public runtime_error
	{
	public:
		sp_err_read_timeout(const string & msg_err) : runtime_error(msg_err) {};
	};
	//-------------------------------------------------------------------------

	ret_t 				sp_open(string Name, ios_base::open_mode Mode);
	bool 				sp_is_open() const;
	void 				sp_set_isopen(bool IsOpen);

	string				sp_get_name() const;
	void				sp_set_name(const string );

	int					sp_get_file_descriptor() const;
	void 				sp_set_file_descriptor(int FileDescriptor);


	//-------------------------------------------------------------------------
	serial_baudrate_t	sp_get_baudrate() const;
	serial_baudrate_t	sp_get_baudrate(const int FileDescriptor)
	            throw  (serial_port::sp_err_not_open,
						 std::runtime_error );

	void				sp_set_baudrate(const serial_baudrate_t) ;
	void				sp_set_baudrate(const int FileDescriptor, const serial_baudrate_t)
	            throw  (serial_port::sp_err_not_open,
	            		serial_port::sp_err_unsupport_baud,
						std::invalid_argument,
						std::runtime_error );

	//-------------------------------------------------------------------------
	character_size_t	sp_get_char_size() const;
	character_size_t	sp_get_char_size(const int FileDescriptor)
    			throw  (serial_port::sp_err_not_open,
    					std::runtime_error );

	void				sp_set_char_size(const character_size_t);
	void				sp_set_char_size(const int FileDescriptor, const character_size_t)
				throw  (serial_port::sp_err_not_open,
						std::invalid_argument,
						std::runtime_error );

	//-------------------------------------------------------------------------
	parity_t			sp_get_parity() const;
	parity_t			sp_get_parity(const int FileDescriptor)
				throw ( serial_port::sp_err_not_open);

	void				sp_set_parity(const parity_t);
	void				sp_set_parity(const int FileDescriptor, const parity_t)
				throw ( serial_port::sp_err_not_open,
						std::invalid_argument,
						std::runtime_error);

	//-------------------------------------------------------------------------
	stop_bit_t			sp_get_stop_bit() const;
	stop_bit_t			sp_get_stop_bit(const int FileDescriptor)
				throw ( serial_port::sp_err_not_open);

	void				sp_set_stop_bit(const stop_bit_t);
	void				sp_set_stop_bit(const int FileDescriptor, const stop_bit_t)
				throw ( serial_port::sp_err_not_open,
						std::invalid_argument);

	//-------------------------------------------------------------------------
	flow_ctrl_t	        sp_get_flow_ctrl() const;
	flow_ctrl_t	        sp_get_flow_ctrl(const int FileDescriptor)
				throw ( serial_port::sp_err_not_open );

	void	       	 	sp_set_flow_ctrl(const flow_ctrl_t);
	void	       	 	sp_set_flow_ctrl(const int FileDescriptor, const flow_ctrl_t)
				throw ( serial_port::sp_err_not_open,
						std::invalid_argument);

	//-------------------------------------------------------------------------
	ret_t 				sp_write(const int FileDescriptor, const unsigned char* DataBufffer, const int BufferSize)
				throw ( serial_port::sp_err_not_open,
						std::runtime_error);

	void 				sp_write_data(const int FileDescriptor, const unsigned char* DataBufffer, const int BufferSize)
				throw ( serial_port::sp_err_not_open,
						std::runtime_error);

	//-------------------------------------------------------------------------
	ret_t 				sp_read(const int FileDescriptor, unsigned char* DataBufffer, const int BufferSize)
				throw ( serial_port::sp_err_not_open,
						std::runtime_error);

	void 				sp_read_data(const int FileDescriptor, unsigned char* DataBufffer, const int BufferSize)
		throw ( serial_port::sp_err_not_open,
				std::runtime_error);
	//-------------------------------------------------------------------------



protected:
private:
	string 				sp_name;
	serial_baudrate_t	sp_baudrate;
	character_size_t	sp_char_size;
	stop_bit_t			sp_stop_bit;
	parity_t			sp_parity;
	flow_ctrl_t			sp_flow_ctrl;
	bool				sp_isopen;
	int					sp_file_descriptor;

	termios				sp_setting;




};



#endif /* SRC_HPP_TEM2CTRL_SERIAL_H_ */
