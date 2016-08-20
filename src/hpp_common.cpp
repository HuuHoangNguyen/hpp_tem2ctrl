//
//========================================================
// Project	: hpp_tem2ctrl
// File 	: hpp_common.c
// Date		: Oct 15, 2015
// Author	: HuuHoang Nguyen
// Contact	: hhnguyen@apm.com
//          : hoangnh.hpp@gmail.com
//========================================================
//

#include "hpp_common.h"

//======================================================================
void hpp_common::delay_for(hpp_uint32_t val)
{
	while(--val)
		__asm__ __volatile__("nop");
}

//======================================================================
#if 1
ret_t hpp_common::str2val(char* s, hpp_sint32_t& val )
{
	hpp_uint32_t s_len = 0;
	hpp_uint32_t i = 0;

	char* str_2b[] = {"0x", "0X", "-0x", "-0X", "-"};

	val = 0;
	s_len = 0;

	//length of str
	while(s[s_len] != '\0')
		s_len++;

//	cout<<"len = "<<s_len<<endl;
//	for(i=0 ; i< s_len; i++)
//	{
//		cout<<s[i]<<endl;
//	}


	//--------------------------------------------------------------------------------
	if((strncmp(s, str_2b[0], 2) == 0) ||(strncmp(s, str_2b[1], 2) == 0))
	{
		for( i= 2; i< s_len; i++)
		{
			switch(s[i])
			{
			case '0':	val = 16* val + 0;	break;
			case '1':	val = 16* val + 1;	break;
			case '2':	val = 16* val + 2;	break;
			case '3':	val = 16* val + 3;	break;
			case '4':	val = 16* val + 4;	break;
			case '5':	val = 16* val + 5;	break;
			case '6':	val = 16* val + 6;	break;
			case '7':	val = 16* val + 7;	break;
			case '8':	val = 16* val + 8;	break;
			case '9':	val = 16* val + 9;	break;
			case 'a':
			case 'A':	val = 16* val + 10;	break;
			case 'b':
			case 'B':	val = 16* val + 11;	break;
			case 'c':
			case 'C':	val = 16* val + 12;	break;
			case 'd':
			case 'D':	val = 16* val + 13;	break;
			case 'e':
			case 'E':	val = 16* val + 14;	break;
			case 'f':
			case 'F':	val = 16* val + 15;	break;
			default: break;
			}
		}
	}
	//--------------------------------------------------------------------------------
	else if((strncmp(s, str_2b[2], 3) == 0) ||(strncmp(s, str_2b[3], 3) == 0))
	{
//		vb_print_("2\n");
		for( i= 3; i< s_len; i++)
		{
			switch(s[i])
			{
			case '0':	val = 16* val + 0;	break;
			case '1':	val = 16* val + 1;	break;
			case '2':	val = 16* val + 2;	break;
			case '3':	val = 16* val + 3;	break;
			case '4':	val = 16* val + 4;	break;
			case '5':	val = 16* val + 5;	break;
			case '6':	val = 16* val + 6;	break;
			case '7':	val = 16* val + 7;	break;
			case '8':	val = 16* val + 8;	break;
			case '9':	val = 16* val + 9;	break;
			case 'a':
			case 'A':	val = 16* val + 10;	break;
			case 'b':
			case 'B':	val = 16* val + 11;	break;
			case 'c':
			case 'C':	val = 16* val + 12;	break;
			case 'd':
			case 'D':	val = 16* val + 13;	break;
			case 'e':
			case 'E':	val = 16* val + 14;	break;
			case 'f':
			case 'F':	val = 16* val + 15;	break;
			default: break;
			}
		}
		val = -val;
	}
	//--------------------------------------------------------------------------------
	else if((strncmp(s, str_2b[4], 1) == 0) )
	{
		for( i = 1; i< s_len; i++)
		{
//			vb_print_("s[%d] = %c \n", i, s[i]);
			if((s[i] < '0') || (s[i] > '9'))		// # 0 - 9
			{
#if HPP_COMMON_STR2VAL_DEBUG
				cout<<"Please input press as format "<<endl;
				cout<<"Decimal     :ddddddddd... (x : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 )"<<endl;
				cout<<"Hexadecimal :0xhhhhhhh... (h : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,A, B, C, D, E, F )"<<endl;
#endif
				return RET_INVALID_PARAM;
			}
		}

		for( i = 1; i< s_len; (i)++)
		{
			switch(s[i])
			{
			case '0':	val = 10* val + 0;	break;
			case '1':	val = 10* val + 1;	break;
			case '2':	val = 10* val + 2;	break;
			case '3':	val = 10* val + 3;	break;
			case '4':	val = 10* val + 4;	break;
			case '5':	val = 10* val + 5;	break;
			case '6':	val = 10* val + 6;	break;
			case '7':	val = 10* val + 7;	break;
			case '8':	val = 10* val + 8;	break;
			case '9':	val = 10* val + 9;	break;
			default: break;
			}
		}

		val =-val;
	}
	//--------------------------------------------------------------------------------
	else
	{
		for( i = 0; i< s_len; (i)++)
		{
//			vb_print_("4\n");
//			vb_print_("s[%d] = %c \n", i, s[i]);
			if((s[i] < '0') || (s[i] > '9'))		// # 0 - 9
			{
#if HPP_COMMON_STR2VAL_DEBUG
				cout<<"Please input press as format"<<endl;
				cout<<"Decimal     :ddddddddd... (x : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 )"<<endl;
				cout<<"Hexadecimal :0xhhhhhhh... (h : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,A, B, C, D, E, F )"<<endl;
#endif
				return RET_INVALID_PARAM;
			}
		}

		for( i = 0; i< s_len; (i)++)
		{
			switch(s[i])
			{
			case '0':	val = 10* val + 0;	break;
			case '1':	val = 10* val + 1;	break;
			case '2':	val = 10* val + 2;	break;
			case '3':	val = 10* val + 3;	break;
			case '4':	val = 10* val + 4;	break;
			case '5':	val = 10* val + 5;	break;
			case '6':	val = 10* val + 6;	break;
			case '7':	val = 10* val + 7;	break;
			case '8':	val = 10* val + 8;	break;
			case '9':	val = 10* val + 9;	break;
			default: break;
			}
		}
	}

	return RET_OK;
}

//======================================================================
#if 1
ret_t hpp_common::str2val(char* s, hpp_sint16_t& val )
{
	ret_t ret;
	hpp_sint32_t val_32;
	val_32 = val;
	ret = str2val(s, val_32);
	val = val_32;

	return ret;
}
#endif

#else
template <typename T>
ret_t str2val(char* s, T& val )
{
	hpp_uint32_t s_len = 0;
	hpp_uint32_t i = 0;

	char* str_2b[] = {"0x", "0X", "-0x", "-0X", "-"};

	val = 0;
	s_len = 0;

	//length of str
	while(s[s_len] != '\0')
		s_len++;

//	cout<<"len = "<<s_len<<endl;
//	for(i=0 ; i< s_len; i++)
//	{
//		cout<<s[i]<<endl;
//	}


	//--------------------------------------------------------------------------------
	if((strncmp(s, str_2b[0], 2) == 0) ||(strncmp(s, str_2b[1], 2) == 0))
	{
		for( i= 2; i< s_len; i++)
		{
			switch(s[i])
			{
			case '0':	val = 16* val + 0;	break;
			case '1':	val = 16* val + 1;	break;
			case '2':	val = 16* val + 2;	break;
			case '3':	val = 16* val + 3;	break;
			case '4':	val = 16* val + 4;	break;
			case '5':	val = 16* val + 5;	break;
			case '6':	val = 16* val + 6;	break;
			case '7':	val = 16* val + 7;	break;
			case '8':	val = 16* val + 8;	break;
			case '9':	val = 16* val + 9;	break;
			case 'a':
			case 'A':	val = 16* val + 10;	break;
			case 'b':
			case 'B':	val = 16* val + 11;	break;
			case 'c':
			case 'C':	val = 16* val + 12;	break;
			case 'd':
			case 'D':	val = 16* val + 13;	break;
			case 'e':
			case 'E':	val = 16* val + 14;	break;
			case 'f':
			case 'F':	val = 16* val + 15;	break;
			default: break;
			}
		}
	}
	//--------------------------------------------------------------------------------
	else if((strncmp(s, str_2b[2], 3) == 0) ||(strncmp(s, str_2b[3], 3) == 0))
	{
//		vb_print_("2\n");
		for( i= 3; i< s_len; i++)
		{
			switch(s[i])
			{
			case '0':	val = 16* val + 0;	break;
			case '1':	val = 16* val + 1;	break;
			case '2':	val = 16* val + 2;	break;
			case '3':	val = 16* val + 3;	break;
			case '4':	val = 16* val + 4;	break;
			case '5':	val = 16* val + 5;	break;
			case '6':	val = 16* val + 6;	break;
			case '7':	val = 16* val + 7;	break;
			case '8':	val = 16* val + 8;	break;
			case '9':	val = 16* val + 9;	break;
			case 'a':
			case 'A':	val = 16* val + 10;	break;
			case 'b':
			case 'B':	val = 16* val + 11;	break;
			case 'c':
			case 'C':	val = 16* val + 12;	break;
			case 'd':
			case 'D':	val = 16* val + 13;	break;
			case 'e':
			case 'E':	val = 16* val + 14;	break;
			case 'f':
			case 'F':	val = 16* val + 15;	break;
			default: break;
			}
		}
		val = -val;
	}
	//--------------------------------------------------------------------------------
	else if((strncmp(s, str_2b[4], 1) == 0) )
	{
		for( i = 1; i< s_len; i++)
		{
//			vb_print_("s[%d] = %c \n", i, s[i]);
			if((s[i] < '0') || (s[i] > '9'))		// # 0 - 9
			{
				cout<<"Please input press as format "<<endl;
				cout<<"Decimal     :ddddddddd... (x : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 )"<<endl;
				cout<<"Hexadecimal :0xhhhhhhh... (h : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,A, B, C, D, E, F )"<<endl;

				return RET_INVALID_PARAM;
			}
		}

		for( i = 1; i< s_len; (i)++)
		{
			switch(s[i])
			{
			case '0':	val = 10* val + 0;	break;
			case '1':	val = 10* val + 1;	break;
			case '2':	val = 10* val + 2;	break;
			case '3':	val = 10* val + 3;	break;
			case '4':	val = 10* val + 4;	break;
			case '5':	val = 10* val + 5;	break;
			case '6':	val = 10* val + 6;	break;
			case '7':	val = 10* val + 7;	break;
			case '8':	val = 10* val + 8;	break;
			case '9':	val = 10* val + 9;	break;
			default: break;
			}
		}

		val =-val;
	}
	//--------------------------------------------------------------------------------
	else
	{
		for( i = 0; i< s_len; (i)++)
		{
//			vb_print_("4\n");
//			vb_print_("s[%d] = %c \n", i, s[i]);
			if((s[i] < '0') || (s[i] > '9'))		// # 0 - 9
			{
				cout<<"Please input press as format"<<endl;
				cout<<"Decimal     :ddddddddd... (x : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 )"<<endl;
				cout<<"Hexadecimal :0xhhhhhhh... (h : 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,A, B, C, D, E, F )"<<endl;
				return RET_INVALID_PARAM;
			}
		}

		for( i = 0; i< s_len; (i)++)
		{
			switch(s[i])
			{
			case '0':	val = 10* val + 0;	break;
			case '1':	val = 10* val + 1;	break;
			case '2':	val = 10* val + 2;	break;
			case '3':	val = 10* val + 3;	break;
			case '4':	val = 10* val + 4;	break;
			case '5':	val = 10* val + 5;	break;
			case '6':	val = 10* val + 6;	break;
			case '7':	val = 10* val + 7;	break;
			case '8':	val = 10* val + 8;	break;
			case '9':	val = 10* val + 9;	break;
			default: break;
			}
		}
	}

	return RET_OK;
}
#endif

//======================================================================
void hpp_common::version()
{
	cout<<endl<<"======================================="<<endl;
	cout<<"  Version of hpp_hex2ctr is: "<<HPP_TEM2CTRL_VERSION <<endl;
	cout<<"======================================="<<endl;

}

//======================================================================
ret_t hpp_common::get_val(hpp_sint32_t& val)
{
	hpp_uint16_t i = 0;
	char str[GET_VAL_BUFF_MAX];

	//clear data
	memset(str, 0, GET_VAL_BUFF_MAX);

	//Press number
	do
	{
		if(((str[i] >= '0') && (str[i] <= '9')) || ((str[i] >='a') && (str[i] <= 'f')) || ((str[i] >= 'A') && (str[i] <= 'F')) || (str[i] == 'x') || (str[i] == 'X')|| (str[i] == '\n') || (str[i] == '\r'))
		{
			cout<<str[i];
		}
		else
			continue;

		i++;

	}while ((str[i-1] != '\n') && (str[i-1] != '\r'));
	cout<<"String is: "<<str;

	return(str2val(str, val));
}

//======================================================================
//ret_t hpp_common::get_str2val(char* str)
//{
//	return (str2val(str, &ret_str2val));
//}
//======================================================================
//---------------------------------------------------------------------------------------

//======================================================================
hpp_common bs;
const double  M_S  = (CLOCKS_PER_SEC/1000);
const double  U_S  = (CLOCKS_PER_SEC/1000000);
clock_t		Start=0,Stop=0;

//======================================================================
void hpp_delay_us(double time)
{
	Start=clock();
	do
	{
		bs.delay_for(100);
		Stop=clock();
	}
	while(((Stop-Start)/U_S)<time);
}

//======================================================================
void hpp_delay_ms(double time)
{
	Start=clock();
	do
	{
		bs.delay_for(100);
		Stop=clock();
	}
	while(((Stop-Start)/M_S)<time);
}
//======================================================================
//======================================================================
//======================================================================
//======================================================================































