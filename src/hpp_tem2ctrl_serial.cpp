//
//========================================================
// Project	: hpp_tem2ctrl
// File 	: hpp_tem2ctrl_serial.cpp
// Date		: Oct 15, 2015
// Author	: HuuHoang Nguyen
// Contact	: hhnguyen@apm.com
//          : hoangnh.hpp@gmail.com
//========================================================
//

#include "hpp_tem2ctrl_serial.h"

//======================================================================
serial_port::serial_port()
{
	this->sp_name			="dev/ttyUSB0";
	this->sp_baudrate 		= BAUD_DEFAULT;
	this->sp_char_size		= CHAR_SIZE_DEFAULT;
	this->sp_stop_bit		= STOP_BITS_DEFAULT;
	this->sp_parity			= PARITY_DEFAULT;
	this->sp_flow_ctrl		= FLOW_CTRL_DEFAULT;
	this->sp_isopen			= false;
	this->sp_file_descriptor= -1;
}

//======================================================================
serial_port::serial_port(string Name,
	    serial_baudrate_t BaudRate,
		character_size_t CharZize,
		stop_bit_t StopBit,
		parity_t Parity,
		flow_ctrl_t FlowCtrl)
{
	this->sp_name		= Name;
	this->sp_baudrate 	= BaudRate;
	this->sp_char_size	= CharZize;
	this->sp_stop_bit	= StopBit;
	this->sp_parity		= Parity;
	this->sp_flow_ctrl	= FlowCtrl;
	this->sp_isopen		= false;
	this->sp_file_descriptor= -1;
}

//======================================================================
serial_port::~serial_port()
{
	if(this->sp_is_open() == true)
	{
		close(this->sp_get_file_descriptor());
	}
}

//======================================================================
ret_t serial_port::sp_open(string Name, ios_base::open_mode mode)
{
    // If the buffer is alreay open then we should not allow a call to
    // another open()
	if(this->sp_is_open() == true)
	{
		cout<<SP_ERR_ALREADY_OPEN<<endl;
		return RET_NO_RSC;
	}

    // We only allow three different combinations of ios_base::openmode
    // so we can use a switch here to construct the flags to be used
    // with the open() system call.
    //
    int flags ;
    if ( mode == (ios_base::in|ios_base::out) )
    {
        flags = O_RDWR ;
    }
    else if ( mode == ios_base::in )
    {
        flags = O_RDONLY ;
    }
    else if ( mode == ios_base::out )
    {
        flags = O_WRONLY ;
    }
    else
    {
        return RET_INVALID_PARAM ;
    }

    // Since we are dealing with the serial port we need to use the
    // O_NOCTTY option.
//    flags |= ( O_NONBLOCK | O_NDELAY) ;
    flags |= ( O_NOCTTY | O_NDELAY) ;
//    flags |= (O_NOCTTY | O_NONBLOCK | O_NDELAY) ;

    //try to open the serial port
    this->sp_set_file_descriptor( open(Name.c_str(), flags) );
    if(this->sp_get_file_descriptor() == -1)
    {
		cerr<<endl<<endl;
		cerr<<"==================================================="<<endl;
		cerr<<"     Can NOT open device "<<this->sp_get_name()<<endl;
		cerr<<"==================================================="<<endl<<endl;
    	return RET_FAIL;
    }

#if 0
    //Direct all SIGIO and SIGURG signals for the port to the current process
    if ( fcntl( this->sp_get_file_descriptor(), F_SETOWN, getpid() ) < 0 )
    {
    	cout<<"Direct all SIGIO and SIGURG signals for the port to the current process --> FAILED"<<endl;
    	return RET_FAIL;
    }

   // Enable asynchronous I/O with the serial port.
    if(fcntl(this->sp_get_file_descriptor(), F_SETFL, FASYNC, 0))
    {
    	cout<<"Enable asynchronous I/O with the serial port --> FAILED"<<endl;
    	return RET_FAIL;
    }
#endif

    fcntl(this->sp_get_file_descriptor(), F_SETFL, 0);
    this->sp_isopen = true;

#if HPP_TEM2CTRL_SERIAL_DEBUG
    cout<<"Open device successful --> PASSED"<<endl;
#endif

    return RET_OK;
}

//======================================================================
bool serial_port::sp_is_open() const
{
	return this->sp_isopen;
}

void serial_port::sp_set_isopen(bool IsOpen)
{
	this->sp_isopen = IsOpen;
}

//======================================================================
string serial_port::sp_get_name() const
{
	return this->sp_name;
}

//======================================================================
void serial_port::sp_set_name(const string Name)
{
	this->sp_name = Name;
}

//======================================================================
int serial_port::sp_get_file_descriptor() const
{
	return this->sp_file_descriptor;
}

//======================================================================
void serial_port::sp_set_file_descriptor(int FileDescriptor)
{
	this->sp_file_descriptor = FileDescriptor;
}

//======================================================================
serial_baudrate_t	serial_port::sp_get_baudrate() const
{
	return this->sp_baudrate;
}

//======================================================================
serial_baudrate_t	serial_port::sp_get_baudrate(const int FileDescriptor)
			throw  (	serial_port::sp_err_not_open,
						std::runtime_error)
{
	//Throw an exception if the serial port is not open.
	if(this->sp_is_open() == false)
	{
		cout<<"sp_get_baudrate: The serial port not opened"<<endl;
		throw serial_port::sp_err_not_open(SP_ERR_NOT_OPEN);
	}

	//Get current settings of the serial port
	if(tcgetattr(FileDescriptor, &this->sp_setting) <  0)
	{
		cout<<"sp_get_baudrate: Not get current of serial port"<<endl;
		throw std::runtime_error( strerror(errno));
	}

	this->sp_set_baudrate(static_cast<serial_baudrate_t>(cfgetispeed(&this->sp_setting)));

#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_get_baudrate: PASSED"<<endl;
#endif

	return this->sp_get_baudrate();
}

//======================================================================
void serial_port::sp_set_baudrate(const serial_baudrate_t BaudRate)
{
	this->sp_baudrate = BaudRate;
}

//======================================================================
void serial_port::sp_set_baudrate(const int FileDescriptor, const serial_baudrate_t BaudRate)
throw  (	serial_port::sp_err_not_open,
			serial_port::sp_err_unsupport_baud,
			std::invalid_argument,
			std::runtime_error)
{
	//Throw an exception if the serial port is not open.
	if(this->sp_is_open()  == false)
	{
		cout<<"sp_set_baudrate:The serial port not opened -> FAILED"<<endl;
		throw serial_port::sp_err_not_open(SP_ERR_NOT_OPEN);
	}

	//get current setting of serial port
	if(tcgetattr( FileDescriptor, &this->sp_setting) < 0)
	{
		cout<<"sp_set_baudrate:Not get current of serial port -> FAILED"<<endl;
		throw std::runtime_error(strerror(errno));
	}

	//set the baud rate for both input and output string
	if((cfsetispeed(&this->sp_setting, BaudRate) < 0) || (cfsetospeed(&this->sp_setting, BaudRate) < 0))
	{
		//If have any of the setting fail, we abandon the method
		cout<<"sp_set_baudrate:set the baud rate for both input and output string -> FAILED"<<endl;
		throw serial_port::sp_err_unsupport_baud(SP_ERR_UNSUPPORTED_BAUD);
	}

	//Set the new attributes of the serial port
	if (tcsetattr(FileDescriptor, TCSANOW, &this->sp_setting) < 0)
	{
		cout<<"sp_set_baudrate:Set new attributes to serial port -> FAILED"<<endl;
		throw serial_port::sp_err_unsupport_baud(strerror(errno));
	}
#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_set_baudrate: PASSED"<<endl;
#endif
}

//======================================================================
character_size_t serial_port::sp_get_char_size() const
{
	return this->sp_char_size;
}

//======================================================================
character_size_t serial_port::sp_get_char_size(const int FileDescriptor)
throw  (serial_port::sp_err_not_open,
		std::runtime_error)
{
	//Make sure that the serial port is open
	if(this->sp_is_open() == false)
	{
		cout<<"sp_get_char_size:Not get current of serial port  -> FAILED"<<endl;
		throw serial_port::sp_err_not_open( SP_ERR_NOT_OPEN);
	}

	//Read the current serial port setting
	if( tcgetattr(FileDescriptor, &this->sp_setting) < 0)
	{
		cout<<"sp_get_char_size:Not get current of serial port -> FAILED"<<endl;
		throw std::runtime_error(strerror(errno));
	}

	this->sp_set_char_size(static_cast<character_size_t>(this->sp_setting.c_cflag & CSIZE));


#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_get_char_size: PASSED"<<endl;
#endif

	return this->sp_get_char_size();
}

//======================================================================
void serial_port::sp_set_char_size(const character_size_t CharSize)
{
	this->sp_char_size = CharSize;
}

//======================================================================
void serial_port::sp_set_char_size(const int FileDescriptor, const character_size_t CharSize)
			throw  (serial_port::sp_err_not_open,
					std::invalid_argument,
					std::runtime_error)
{
	//Make sure the serial port is  opened
	if(this->sp_is_open() == false)
	{
		cout<<"sp_set_char_size:The serial port not opened"<<endl;
		throw serial_port::sp_err_not_open( SP_ERR_NOT_OPEN);
	}

	//Read the current serial port setting
	if( tcgetattr(FileDescriptor, &this->sp_setting) < 0)
	{
		cout<<"sp_set_char_size:Not get current of serial port -> FAILED"<<endl;
		throw std::runtime_error(strerror(errno));
	}

	//Set the character size
	this->sp_setting.c_cflag &= ~CSIZE;
	this->sp_setting.c_cflag |= CharSize;

	//Applied the modified settings.
	if(tcsetattr(FileDescriptor, TCSANOW, & this->sp_setting) < 0)
	{
		cout<<"sp_set_baudrate:Set new attributes to serial port -> FAILED"<<endl;
		throw std::invalid_argument( strerror(errno));
	}

#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_set_char_size: PASSED"<<endl;
#endif
}

//======================================================================
parity_t	serial_port::sp_get_parity() const
{
	return this->sp_parity;
}

//======================================================================
void serial_port::sp_set_parity(const parity_t Parity)
{
	this->sp_parity = Parity;
}

//======================================================================
parity_t serial_port::sp_get_parity(const int FileDescriptor)
			throw ( serial_port::sp_err_not_open)
{
	//Make sure the serial port is  opened
	if(this->sp_is_open() == false)
	{
		cout<<"sp_get_parity:The serial port not opened"<<endl;
		throw serial_port::sp_err_not_open( SP_ERR_NOT_OPEN);
	}

	//Read the current serial port setting
	if( tcgetattr(FileDescriptor, &this->sp_setting) < 0)
	{
		cout<<"sp_get_parity:Not get current of serial port -> FAILED"<<endl;
		throw std::runtime_error(strerror(errno));
	}

	// Get the parity type from the current settings.
	if( this->sp_setting.c_cflag & PARENB)
	{
		//Parity is enable. Lets checck if it odd or even
		if(this->sp_setting.c_cflag & PARODD)
		{
			this->sp_set_parity(PARITY_ODD);
		}
		else
		{
			this->sp_set_parity(PARITY_EVEN);
		}
	}
	else
	{
		this->sp_set_parity(PARITY_NONE);
	}

#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_get_parity: PASSED"<<endl;
#endif

	return this->sp_get_parity();
}

//======================================================================
void serial_port::sp_set_parity(const int FileDescriptor, const parity_t Parity)
			throw ( serial_port::sp_err_not_open,
					std::invalid_argument,
					std::runtime_error)
{
	//Make sure the serial port is  opened
	if(this->sp_is_open() == false)
	{
		cout<<"sp_set_parity:The serial port not opened"<<endl;
		throw serial_port::sp_err_not_open( SP_ERR_NOT_OPEN);
	}

	//Read the current serial port setting
	if( tcgetattr(FileDescriptor, &this->sp_setting) < 0)
	{
		cout<<"sp_set_parity:Not get current of serial port -> FAILED"<<endl;
		throw std::runtime_error(strerror(errno));
	}

	//Set the parity depending on the specified parameter
	switch (Parity)
	{
	case PARITY_EVEN:
		this->sp_setting.c_cflag |= PARENB;
		this->sp_setting.c_cflag &= ~PARODD;
		this->sp_setting.c_iflag |= INPCK;
		break;

	case PARITY_ODD:
		this->sp_setting.c_cflag |= (PARENB | PARODD);
		this->sp_setting.c_iflag |= INPCK;
		break;

	case PARITY_NONE:
		this->sp_setting.c_cflag &= ~(PARENB);
//		this->sp_setting.c_cflag &= ~(CSTOPB);
//		this->sp_setting.c_iflag |= IGNPAR;
		break;

	default:
		throw std::invalid_argument(SP_INVALID_PARITY);
		break;
	}

	//Applied the modified port settings.
	if( tcsetattr( FileDescriptor, TCSANOW, &this->sp_setting) < 0)
	{
		cout<<"sp_set_parity:Set new attributes to serial port -> FAILED"<<endl;
		throw std::invalid_argument(strerror(errno));
	}

#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_set_parity: PASSED"<<endl;
#endif

}

//======================================================================
stop_bit_t serial_port::sp_get_stop_bit() const
{
	return	this->sp_stop_bit;
}

//======================================================================
void serial_port::sp_set_stop_bit(const stop_bit_t StopBit)
{
	this->sp_stop_bit = StopBit;
}

//======================================================================
stop_bit_t serial_port::sp_get_stop_bit(const int FileDescriptor)
			throw ( serial_port::sp_err_not_open)
{
	//Make sure the serial port is  opened
	if(this->sp_is_open() == false)
	{
		cout<<"sp_get_stop_bit:The serial port not opened"<<endl;
		throw serial_port::sp_err_not_open( SP_ERR_NOT_OPEN);
	}

	//Read the current serial port setting
	if( tcgetattr(FileDescriptor, &this->sp_setting) < 0)
	{
		cout<<"sp_get_stop_bit:Not get current of serial port -> FAILED"<<endl;
		throw std::runtime_error(strerror(errno));
	}

	//If CSTOPB is set then we are using two 2 stops bit, otherwise we
	// are using 1 stop bit.
	if(this->sp_setting.c_cflag % CSTOPB)
	{
		this->sp_set_stop_bit(STOP_BITS_2);
	}

	this->sp_set_stop_bit(STOP_BITS_1);

#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_get_stop_bit: PASSED"<<endl;
#endif

	return this->sp_get_stop_bit();
}

//======================================================================
void serial_port::sp_set_stop_bit(const int FileDescriptor, const stop_bit_t StopBit)
			throw ( serial_port::sp_err_not_open,
					std::invalid_argument)
{
	//Make sure the serial port is  opened
	if(this->sp_is_open() == false)
	{
		cout<<"sp_set_stop_bit:The serial port not opened"<<endl;
		throw serial_port::sp_err_not_open( SP_ERR_NOT_OPEN);
	}

	//Read the current serial port setting
	if( tcgetattr(FileDescriptor, &this->sp_setting) < 0)
	{
		cout<<"sp_set_stop_bit:Not get current of serial port -> FAILED"<<endl;
		throw std::runtime_error(strerror(errno));
	}

	//Set the number of stop bits
	switch(StopBit)
	{
	case STOP_BITS_1:
		this->sp_setting.c_cflag &= ~(CSTOPB);
		break;

	case STOP_BITS_2:
		this->sp_setting.c_cflag |= CSTOPB;
		break;

	default:
		throw std::invalid_argument(SP_INVALIS_STOP_BIT);
		break;
	}

	//Applied the modified port settings
	if(tcsetattr(FileDescriptor, TCSANOW, &this->sp_setting) < 0)
	{
		cout<<"sp_set_stop_bit:Set new attributes to serial port -> FAILED"<<endl;
		throw std::invalid_argument( strerror(errno));
	}

#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_set_stop_bit: PASSED"<<endl;
#endif

}

//======================================================================
flow_ctrl_t serial_port::sp_get_flow_ctrl() const
{
	return this->sp_flow_ctrl;
}

//======================================================================
void serial_port::sp_set_flow_ctrl(const flow_ctrl_t FlowCtrl)
{
	this->sp_flow_ctrl = FlowCtrl;
}

//======================================================================
flow_ctrl_t	serial_port::sp_get_flow_ctrl(const int FileDescriptor)
			throw ( serial_port::sp_err_not_open )
{
	//Make sure the serial port is  opened
	if(this->sp_is_open() == false)
	{
		cout<<"sp_get_flow_ctrl:The serial port not opened"<<endl;
		throw serial_port::sp_err_not_open( SP_ERR_NOT_OPEN);
	}

	//Read the current serial port setting
	if( tcgetattr(FileDescriptor, &this->sp_setting) < 0)
	{
		cout<<"sp_get_flow_ctrl:Not get current of serial port -> FAILED"<<endl;
		throw std::runtime_error(strerror(errno));
	}

	// If CRTSCTS is set then we are using hardware flow
	// control. Otherwise, we are not using any flow control.
	if(this->sp_setting.c_cflag & CRTSCTS )
	{
		this->sp_set_flow_ctrl( FLOW_CTRL_HARD);
	}

	this->sp_set_flow_ctrl(FLOW_CTRL_NONE);

#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_get_flow_ctrl: PASSED"<<endl;
#endif

	return this->sp_get_flow_ctrl();
}

//======================================================================
void serial_port::sp_set_flow_ctrl(const int FileDescriptor, const flow_ctrl_t FlowCtrl)
			throw ( serial_port::sp_err_not_open,
					std::invalid_argument)
{
	//Make sure the serial port is  opened
	if(this->sp_is_open() == false)
	{
		cout<<"sp_set_flow_ctrl:The serial port not opened"<<endl;
		throw serial_port::sp_err_not_open( SP_ERR_NOT_OPEN);
	}

	//Read the current serial port setting
	if( tcgetattr(FileDescriptor, &this->sp_setting) < 0)
	{
		cout<<"sp_set_flow_ctrl:Not get current of serial port -> FAILED"<<endl;
		throw std::runtime_error(strerror(errno));
	}

	//Set the flow control.
	switch (FlowCtrl)
	{
	case FLOW_CTRL_HARD:
		this->sp_setting.c_cflag |= CRTSCTS;
		break;

	case FLOW_CTRL_NONE:
		this->sp_setting.c_cflag &= ~(CRTSCTS);
		//Disable software flow control
		this->sp_setting.c_iflag &= ~(IXON | IXOFF | IXANY);
		break;

	default:
		throw std::invalid_argument (SP_INVALID_FLOW_CTRL);
	}

	//Applied the modified setting.
	if( tcsetattr(FileDescriptor, TCSANOW, &this->sp_setting) < 0 )
	{
		cout<<"sp_set_flow_ctrl:Set new attributes to serial port -> FAILED"<<endl;
		throw std::invalid_argument( strerror(errno));
	}

#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_set_flow_ctrl: PASSED"<<endl;
#endif

}

//======================================================================
void serial_port::sp_write_data(const int FileDescriptor, const unsigned char* DataBufffer, const int BufferSize)
			throw ( serial_port::sp_err_not_open,
					std::runtime_error)
{
	//Make sure the serial port is  opened
	if(this->sp_is_open() == false)
	{
		cout<<"sp_write_data:The serial port not opened"<<endl;
		throw serial_port::sp_err_not_open( SP_ERR_NOT_OPEN);
	}

	//write data to serial port
	int num_of_bytes_written = -1 ;
	do
	{

		num_of_bytes_written = write (FileDescriptor, DataBufffer, BufferSize);
		for(int i=0; i< 100; i++);
	}while((num_of_bytes_written<BufferSize) && ( EAGAIN == errno ) ) ;

	if(num_of_bytes_written < 0)
	{
		cout<<"sp_write_data:Write data to serial port -> FAILED"<<endl;
		throw std::runtime_error( strerror(errno));
	}

#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_write_data: PASSED"<<endl;
#endif
}

//======================================================================
ret_t serial_port::sp_write(const int FileDescriptor, const unsigned char* DataBufffer, const int BufferSize)
throw ( serial_port::sp_err_not_open,
		std::runtime_error)
{
	//Make sure the serial port is  opened
	if(this->sp_is_open() == false)
	{
		cout<<"sp_write:The serial port not opened"<<endl;
		throw serial_port::sp_err_not_open( SP_ERR_NOT_OPEN);
	}

	this->sp_set_baudrate(FileDescriptor, this->sp_get_baudrate() );
	this->sp_set_char_size(FileDescriptor, this->sp_get_char_size());
	this->sp_set_parity(FileDescriptor, this->sp_get_parity());
	this->sp_set_stop_bit(FileDescriptor, this->sp_get_stop_bit());
	this->sp_set_flow_ctrl(FileDescriptor, this->sp_get_flow_ctrl());

	//Enable the receiver and set local mode
	this->sp_setting.c_cflag |= (CLOCAL | CREAD);

	//Choosing Raw input
	this->sp_setting.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	//Choosing Raw Output
	this->sp_setting.c_oflag &= ~OPOST;

	this->sp_setting.c_cc[VMIN] = 0;
	this->sp_setting.c_cc[VTIME] = 100;

	if( tcsetattr(FileDescriptor, TCSANOW, &this->sp_setting) < 0 )
	{
		throw std::runtime_error( strerror(errno));
	}

	this->sp_write_data(FileDescriptor, DataBufffer, BufferSize);

#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_write: PASSED"<<endl;
#endif

	return RET_OK;
}

//======================================================================
void serial_port::sp_read_data(const int FileDescriptor, unsigned char* DataBufffer, const int BufferSize)
	throw ( serial_port::sp_err_not_open,
			std::runtime_error)
{
	//Make sure the serial port is  opened
	if(this->sp_is_open() == false)
	{
		cout<<"sp_read_data:The serial port not opened"<<endl;
		throw serial_port::sp_err_not_open( SP_ERR_NOT_OPEN);
	}

//	cout<<"BufferSize = "<<BufferSize<<endl;
	int num_of_bytes_read = 0;
	int numbyte;
//	int i;
#if 1
	do
	{
		numbyte = ::read (FileDescriptor, &DataBufffer[num_of_bytes_read], (BufferSize - num_of_bytes_read));
//		numbyte = ::read (FileDescriptor, &DataBufffer[num_of_bytes_read], (BufferSize - num_of_bytes_read -1));
		for(int i=0; i<1000; i++);
		num_of_bytes_read +=numbyte;
//		cout<<"num_of_bytes_read = "<<dec<<num_of_bytes_read<<endl;
//		cout<<"DataBufffer[ = "<<num_of_bytes_read<<" ]="<<hex<<static_cast<int>(DataBufffer[num_of_bytes_read])<<endl;

		if((DataBufffer[num_of_bytes_read -1] == '\0') || (DataBufffer[num_of_bytes_read -1] == '\n') || (DataBufffer[num_of_bytes_read -1] == EOF))
//		if((DataBufffer[num_of_bytes_read -1] == '\0') || (DataBufffer[num_of_bytes_read -1] == '\n'))
		{
			break;
		}


	}while((num_of_bytes_read < BufferSize) && (numbyte > 0)) ;
#else
	for(i = 0; i< BufferSize; i++)
	{
		numbyte  = ::read (FileDescriptor, &DataBufffer[i], 1);
		if((DataBufffer[i] == '\0') || (DataBufffer[i] == '\n') || (DataBufffer[i] == EOF))
		{
			break;
		}
		cout<<i<<"   "<<numbyte<<endl;

		if(numbyte < 0)
			break;
	}

#endif

#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_read_data: PASSED"<<endl;
#endif
}

//======================================================================
ret_t serial_port::sp_read(const int FileDescriptor, unsigned char* DataBufffer, const int BufferSize)
			throw ( serial_port::sp_err_not_open,
					std::runtime_error)
{
	//Make sure the serial port is  opened
	if(this->sp_is_open() == false)
	{
		cout<<"sp_read:The serial port not opened"<<endl;
		throw serial_port::sp_err_not_open( SP_ERR_NOT_OPEN);
	}

	cfmakeraw(&this->sp_setting);

	this->sp_set_baudrate(FileDescriptor, this->sp_get_baudrate() );
	this->sp_set_char_size(FileDescriptor, this->sp_get_char_size());
	this->sp_set_parity(FileDescriptor, this->sp_get_parity());
	this->sp_set_stop_bit(FileDescriptor, this->sp_get_stop_bit());
	this->sp_set_flow_ctrl(FileDescriptor, this->sp_get_flow_ctrl());

	//Enable the receiver and set local mode
	this->sp_setting.c_cflag |= (CLOCAL | CREAD);


//	this->sp_setting.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON | IXOFF | IXANY);

	//Choosing Raw input
	this->sp_setting.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);

	//Choosing Raw Output
	this->sp_setting.c_oflag &= ~OPOST;

	this->sp_setting.c_cc[VMIN] = 0;
	this->sp_setting.c_cc[VTIME] = 100;

	if( tcsetattr(FileDescriptor, TCSANOW, &this->sp_setting) < 0 )
	{
		cout<<"sp_read:Set new attributes to serial port -> FAILED"<<endl;
		throw std::runtime_error( strerror(errno));
	}

	const unsigned char* cmd_read_prt = HPP_TEM2CTRL_CMD_READ_TEMP;
	this->sp_write_data(FileDescriptor, cmd_read_prt, sizeof(cmd_read_prt));

	for(int i =0; i< 10000; i++) __asm__ __volatile__("nop");

	this->sp_read_data(FileDescriptor, DataBufffer, BufferSize);

#if HPP_TEM2CTRL_SERIAL_DEBUG
	cout<<"sp_read: PASSED"<<endl;
#endif
	return RET_OK;
}

//======================================================================
//======================================================================
//======================================================================
//======================================================================
//======================================================================
//======================================================================
//======================================================================
//======================================================================

