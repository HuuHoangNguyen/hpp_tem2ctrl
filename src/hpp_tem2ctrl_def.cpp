//
//========================================================
// Project	: hpp_tem2ctrl
// File 	: hpp_tem2ctrl_def.cpp
// Date		: Oct 15, 2015
// Author	: HuuHoang Nguyen
// Contact	: hhnguyen@apm.com
//          : hoangnh.hpp@gmail.com
//========================================================
//

#include "hpp_tem2ctrl_def.h"

int hpp_tem2ctrl_write_temp_read_comfirm = 1;

//======================================================================
hpp_tem2ctrl::hpp_tem2ctrl()
{

	this->str_rd_site = 0;
	this->str_wr_site = 0;
	this->str_wr = "\0";
	this->str_rd = "\0";
	this->str_dev="\n";
	this->arg_temp = 0;
	this->arg_opt = OPT_WR;
	this->prog_mod = PROG_MODE_NONE;
	this->arg_temp_num = 0;
	this->temp_n_limit = TEMP_N_MAX;
	this->temp_p_limit = TEMP_P_MAX;

}

//hpp_tem2ctrl::hpp_tem2ctrl(hpp_sint16_t Temp_N_Limit= TEMP_N_MAX, hpp_sint16_t 	Temp_P_Limit  = TEMP_P_MAX)
hpp_tem2ctrl::hpp_tem2ctrl(hpp_sint16_t Temp_N_Limit, hpp_sint16_t 	Temp_P_Limit)
{
	this->str_rd_site = 0;
	this->str_wr_site = 0;
	this->str_wr = "\0";
	this->str_rd = "\0";
	this->str_dev="\n";
	this->arg_temp = 0;
	this->arg_opt = OPT_WR;
	this->prog_mod = PROG_MODE_NONE;
	this->arg_temp_num = 0;
	this->temp_n_limit = Temp_N_Limit;
	this->temp_p_limit = Temp_P_Limit;
}

//======================================================================
hpp_tem2ctrl::~hpp_tem2ctrl()
{
}

//======================================================================
void hpp_tem2ctrl::set_dev(const string StrDevice)
{
	this->str_dev = StrDevice;
}

//======================================================================
string hpp_tem2ctrl::get_dev() const
{
	return this->str_dev;
}

//======================================================================
ret_t hpp_tem2ctrl::check_arg(int argc, char** argv)
{
	hpp_sint32_t temperature;
	if(argc<2 || argc > 4)
	{
		if(HPP_STRING_HELP_DEBUG)
//			cout<<"-----1-------"<<endl;
		this->help();
		return RET_INVALID_PARAM;
	}
	else if((argc == 2) && str2val(argv[1], temperature) && strncmp(argv[1], "ttyUSB", 6) )
	{
		if(HPP_STRING_HELP_DEBUG)
//			cout<<"-----2-------"<<endl;
		this->help();
		return RET_INVALID_PARAM;
	}
	else if( (argc == 3) && str2val(argv[1], temperature) &&  strncmp(argv[1], "ttyUSB", 6))
	{
		if(HPP_STRING_HELP_DEBUG)
//			cout<<"-----3-------"<<endl;
		this->help();
		return RET_INVALID_PARAM;
	}

	else if( (argc == 3) && !str2val(argv[1], temperature) &&  str2val(argv[2], temperature) )
	{
		if(HPP_STRING_HELP_DEBUG)
//			cout<<"-----4-------"<<endl;
		this->help();
		return RET_INVALID_PARAM;
	}

	else if( (argc == 3) && !strncmp(argv[1], "ttyUSB", 6) && str2val(argv[2], temperature) && strcmp(argv[2],"r") && strcmp(argv[2],"R"))
	{
		if(HPP_STRING_HELP_DEBUG)
//			cout<<"-----5-------"<<endl;
		this->help();
		return RET_INVALID_PARAM;
	}

	else if( (argc == 4) && (strncmp(argv[1], "ttyUSB", 6) || str2val(argv[2], temperature) || (strcmp(argv[3],"w") && strcmp(argv[3],"W"))))
	{
		if(HPP_STRING_HELP_DEBUG)
//			cout<<"-----6-------"<<endl;
		this->help();
		return RET_INVALID_PARAM;
	}

	return RET_OK;
}

//======================================================================
ret_t hpp_tem2ctrl::set_arg(int argc, char** argv)
{
	ret_t ret;
	ret = this->check_arg(argc, argv);
	if(ret)
		return ret;
	hpp_tem2ctrl_write_temp_read_comfirm = 1;

	if(!strncmp(argv[1], "ttyUSB", 6))
	{
		//-----------------------------------------------------
		string device_path;
		string device(argv[1]);
		device_path = "/dev/";
		device_path +=device;
		this->set_dev(device_path);

		//-----------------------------------------------------
		hpp_sint16_t Temp;
		if(argc == 2)
		{
			this->set_arg_temp(0);
			this->set_arg_temp_num(0);
			this->set_arg_opt(OPT_RD);
		}
		else if (argc == 3 && (!strcmp(argv[2],"r") || !strcmp(argv[2],"R")))
		{
			this->set_arg_temp(0);
			this->set_arg_temp_num(0);
			this->set_arg_opt(OPT_RD);
		}
		else if (argc == 3 && !str2val(argv[2], Temp))
		{
			ret = str2val(argv[2], Temp);
			this->set_arg_temp(Temp);
			this->set_arg_temp_num(1);
			this->set_arg_opt(OPT_WR);
		}
		else if (argc == 4)
		{
			ret = str2val(argv[2], Temp);
			this->set_arg_temp(Temp);
			this->set_arg_temp_num(1);
			this->set_arg_opt(OPT_WR);
		}

		//-----------------------------------------------------
		this->prog_mod = PROG_MODE_RUN;

	}
	else
	{
		//-----------------------------------------------------
		hpp_sint16_t Temp;
		this->set_dev("NONE");

		ret = str2val(argv[1], Temp);
		this->set_arg_temp(Temp);

		if(argc == 3)
		{
			hpp_sint16_t TempNum;
			ret = str2val(argv[2], TempNum);
			if(TempNum < 0)
			{
				cerr<<"Number of temperature cannot negative"<<endl<<endl;
				return RET_INVALID_PARAM;
			}

			this->set_arg_temp_num(TempNum);
		}
		else
		{
			this->set_arg_temp_num(1);
		}


		//-----------------------------------------------------
		this->prog_mod = PROG_MODE_DUMP;

	}

//	if (argc == 4 && (!strcmp(argv[2],"w") || !strcmp(argv[2],"W")))
	if (argc == 4 && !strcmp(argv[3],"W"))
	{
		hpp_tem2ctrl_write_temp_read_comfirm = 0;
	}
	else
	{
		hpp_tem2ctrl_write_temp_read_comfirm = 1;
	}


	if(this->get_arg_temp() < temp_n_limit || this->get_arg_temp() > temp_p_limit)
	{
		cerr<<endl;
		cerr<<"============================================"<<endl;
		cerr<<"Press temperature from -30C to 110 C"<<endl;
		cerr<<"============================================"<<endl<<endl;

		this->help();
		return RET_INVALID_PARAM;
	}
	return RET_OK;
}

//======================================================================
void hpp_tem2ctrl::set_arg_temp(hpp_sint16_t ArgTem)
{
	this->arg_temp = ArgTem;
}

//======================================================================
hpp_sint16_t hpp_tem2ctrl::get_arg_temp() const
{
	return this->arg_temp;
}

//======================================================================
void hpp_tem2ctrl::set_arg_temp_num(hpp_sint16_t ArgTemNum)
{
	this->arg_temp_num = ArgTemNum;

}

//======================================================================
hpp_sint16_t hpp_tem2ctrl::get_arg_temp_num() const
{
	return this->arg_temp_num;

}

//======================================================================
void hpp_tem2ctrl::set_arg_opt(option_t ArgOpt)
{
	this->arg_opt = ArgOpt;
}

//======================================================================
option_t hpp_tem2ctrl::get_arg_opt() const
{
	return this->arg_opt;
}

//======================================================================
program_mod_t	hpp_tem2ctrl::get_prog_mod() const
{
	return this->prog_mod;
}

//======================================================================
ret_t hpp_tem2ctrl::dump()
{
	hpp_uint16_t i;
	hpp_uint64_t hex_val;
	hpp_uint16_t temp_conver;

	cout<<"---------------------------------------------------"<<endl;
	cout<<setw(6)<<"Temp "<<setw(30)<<" Hex number  "<<endl;
	cout<<"---------------------------------------------------"<<endl;

	for(i=0; i< this->arg_temp_num; i++)
	{
		temp_conver = convert_temp(get_abs_temp(this->arg_temp), get_sign_temp(this->arg_temp));
		hex_val = hpp_tem2hex_value(temp_conver, hpp_tem2hex_crc(temp_conver));

		if(i% 10 == 0)
			cout<<endl;
		cout<<dec<<setw(4)<<setfill(' ')<<this->get_arg_temp()<<setw(2)<<"C"<<hex<<setw(16)<<""<<setw(16)<<setfill('0')<<hex_val<<endl;

		if(get_sign_temp(this->arg_temp))
		{
			this->set_arg_temp(this->get_arg_temp() - 1);
		}
		else
		{
			this->set_arg_temp(this->get_arg_temp() + 1);
		}

		if(this->get_arg_temp() < temp_n_limit || this->get_arg_temp() > temp_p_limit)
			break;
	}
	cout<<"---------------------------------------------------"<<endl;

	return RET_OK;
}

//======================================================================
ret_t hpp_tem2ctrl::run(const serial_baudrate_t BaudRate = BAUD_DEFAULT,
		                const character_size_t CharSize  = CHAR_SIZE_DEFAULT,
						const stop_bit_t StopBit= STOP_BITS_DEFAULT,
						const parity_t Parity = PARITY_DEFAULT,
						const flow_ctrl_t FlowCtrl = FLOW_CTRL_DEFAULT)
{
	hpp_uint16_t i;
	hpp_uint64_t hex_val;
	hpp_uint16_t data_temp;
	ret_t ret;

	//Cover temperature to thermal hex value.
	data_temp = convert_temp(get_abs_temp(this->arg_temp), get_sign_temp(this->arg_temp));
	hex_val = hpp_tem2hex_value(data_temp, hpp_tem2hex_crc(data_temp));

	if(this->get_arg_opt() == OPT_WR)
		cout<<"   Hex value = "<<hex<<setw(16)<<setfill('0')<<hex_val<<endl;

	//Set device name
	this->sp_set_name(this->get_dev());

	//Set parameter to init port
	this->sp_set_baudrate(BaudRate);
	this->sp_set_char_size(CharSize);
	this->sp_set_parity(Parity);
	this->sp_set_stop_bit(StopBit);
	this->sp_set_flow_ctrl(FlowCtrl);

#if HPP_TEM2CTRL_DEF_DATA_DEBUG
	cout<<"Serial port name: "<<this->sp_get_name()<<endl;
	cout<<"Baud rate       : "<<this->sp_get_baudrate()<<endl;
	cout<<"Char size       : "<<this->sp_get_char_size()<<endl;
	cout<<"Parity          : "<<this->sp_get_parity()<<endl;
	cout<<"Stop bit        : "<<this->sp_get_stop_bit()<<endl;
	cout<<"Flow control    : "<<this->sp_get_flow_ctrl()<<endl;
#endif
	//Open device
	if(this->sp_open(this->sp_get_name(), (ios_base::in|ios_base::out)))
	{
		return RET_FAIL;
	}

#if HPP_TEM2CTRL_DEF_DATA_DEBUG
	cout<<"File Descriptor :"<<this->sp_get_file_descriptor()<<endl;;
	cout<<"Is open         :"<<this->sp_is_open()<<endl;

#endif
	vector<unsigned char>cmd_data;
	int HexValSize = sizeof(hex_val);
	hpp_uint08_t* DataBuffPtr= new hpp_uint08_t[HexValSize + 1];

	//--------------------------------------------------------------------------------
	//If mode: Run; Option: Read
	if(this->get_arg_opt() == OPT_RD)
	{

#if HPP_TEM2CTRL_DEF_DATA_DEBUG
		cout<<"HexValSize = "<<dec<<HexValSize<<endl;
#endif
		memset(DataBuffPtr, 0, sizeof(DataBuffPtr));

		ret = this->sp_read(this->sp_get_file_descriptor(),DataBuffPtr, (HexValSize -1));
		if(ret)
		{
			return RET_FAIL;
		}

//		for(i = 0; i< HexValSize; i++)
//		{
//			cmd_data.push_back(*(DataBuffPtr +i));
//		}

//		cout<<"---------------------------------------------------"<<endl<<endl;
		cout<<"   Buff = ";
		for(i=0; i<HexValSize; i++)
			cout<<"0x"<<hex<<uppercase<<setw(2)<<setfill('0')<<static_cast<int>(DataBuffPtr[i])<<" ";
		cout<<endl<<endl;

		hpp_uint16_t rTemp = 0;
		hpp_sint16_t rSign = 0;
		rTemp = (static_cast<hpp_uint16_t>(DataBuffPtr[3])<<8) | (static_cast<hpp_uint16_t>(DataBuffPtr[4]));

		if(rTemp & 0x8000)
			rSign = 1;

		if(rSign)
		{
			rTemp = rTemp -1;
			rTemp = ~rTemp;
		}

		rTemp = rTemp/10;
		cout<<"   Temperature ="<<(rSign?" - ":" ")<<dec<<rTemp<<" C"<<endl<<endl;
		cout<<"---------------------------------------------------"<<endl<<endl;
	}

	//--------------------------------------------------------------------------------
	//If mode: Run; Option: Write
	else if(this->get_arg_opt() == OPT_WR)
	{
		unsigned char HexTemp;

#if HPP_TEM2CTRL_DEF_DATA_DEBUG
		cout<<"Hex value = 0x"<<hex<<setw(16)<<setfill('0')<<hex_val<<endl;
		cout<<"Hex Size = "<<dec<<HexValSize<<endl;
#endif
		for(i = 1; i<= HexValSize; i++)
		{
			HexTemp = static_cast<unsigned char>(hex_val>>((HexValSize - i)*8));
#if HPP_TEM2CTRL_DEF_DATA_DEBUG
			cout<<"Hextemp-"<<dec<<i<<" : 0x"<<hex<<setw(2)<<setfill('0')<<static_cast<unsigned int>(HexTemp)<<endl;
#endif
			cmd_data.push_back(HexTemp);

		}
#if HPP_TEM2CTRL_DEF_DATA_DEBUG
		for (unsigned i=0; i<cmd_data.size(); ++i)
		{
			cout <<"cmd_data - "<<dec<<i<<": 0x"<<hex<<setw(2)<<setfill('0')<<static_cast<int>(cmd_data[i])<<endl;
		}
#endif
		hpp_uint08_t* DataBuff= cmd_data.data();
		int write_temp_loop;

		if (hpp_tem2ctrl_write_temp_read_comfirm)
			write_temp_loop = WRITE_TEMP_LOOP_MAX;
		else
			write_temp_loop = 1;
		do
		{
			delay_for(1000000);
			memset(DataBuffPtr, 0, (sizeof(DataBuffPtr)/sizeof(char *)));
			ret = this->sp_write(this->sp_get_file_descriptor(),DataBuff,HexValSize);
			if(ret)
			{
				return RET_FAIL;
			}

			if (hpp_tem2ctrl_write_temp_read_comfirm)
			{
				delay_for(1000000);
				delay_for(1000000);
				this->sp_read_data(this->sp_get_file_descriptor(),DataBuffPtr, HexValSize);

				if (DataBuff[4] == DataBuffPtr[4] && DataBuff[5] == DataBuffPtr[5])
					break;
			}

			int close_ret;
			do
			{
				delay_for(10000);
				tcflush(this->sp_get_file_descriptor(), TCOFLUSH);
				close_ret =  close(this->sp_get_file_descriptor());

			}while(close_ret !=0);

			this->sp_set_isopen(false);

			delay_for(10000);


			if(this->sp_open(this->sp_get_name(), (ios_base::in|ios_base::out)))
			{
				return RET_FAIL;
			}
		}
		while(--write_temp_loop);

		if (hpp_tem2ctrl_write_temp_read_comfirm)
		{
			if(!write_temp_loop)
			{
				cout<<endl<<"   Write temperature "<<dec<<this->arg_temp<<" C: FAILED "<<endl<<endl;
			}
			else
			{
				cout<<endl<<"   Write temperature "<<dec<<this->arg_temp<<" C: SUCCESSFUL "<<endl<<endl;
			}
		}
		else
		{
			cout<<endl<<"   Write temperature "<<dec<<this->arg_temp<<" C: SUCCESSFUL "<<endl<<endl;
		}
		cout<<"---------------------------------------------------"<<endl<<endl;

	}//End: If mode: Run; Option: Write
	//--------------------------------------------------------------------------------

#if HPP_TEM2CTRL_DEF_DEBUG
	for (unsigned i=0; i<cmd_data.size(); ++i)
	{
		cout <<"cmd_data[ "<<dec<<i<<" ] = 0x"<<hex<<setw(2)<<setfill('0')<<static_cast<int>(cmd_data[i])<<endl;
	}

	for (unsigned i=0; i<HexValSize; ++i)
	{
		cout <<"DataBuffPtr[ "<<dec<<i<<" ] = 0x"<<hex<<setw(2)<<setfill('0')<<static_cast<int>(DataBuffPtr[i])<<endl;
	}
#endif

	delete[] DataBuffPtr;
	close(this->sp_get_file_descriptor());

	return RET_OK;
}

//======================================================================
string hpp_tem2ctrl::hex2str(const hpp_uint64_t HexValue)
{
	string HexString = "\0";
	HexString = static_cast<ostringstream*>( &(ostringstream() << hex<<HexValue) )->str();

	if(HexString.length() % 2)
	{
		HexString.insert(0,"0");
	}

	return HexString;
}

//======================================================================
void hpp_tem2ctrl::help() const
{
	int HelpStrSize;
	int i;
//	HelpStrSize = 27;
	HelpStrSize = hpp_tem2ctrl_help_str_sise;
	for(i=0; i < HelpStrSize; i++)
		cout<<hpp_tem2ctrl_help_str[i]<<endl;
}

