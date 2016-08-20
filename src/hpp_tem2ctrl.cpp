//
//========================================================
// Project	: hpp_tem2ctrl
// File 	: hpp_tem2ctrl.cpp
// Date		: Oct 15, 2015
// Author	: HuuHoang Nguyen
// Contact	: hhnguyen@apm.com
//          : hoangnh.hpp@gmail.com
//========================================================
//





//======================================================================
#include "hpp_tem2ctrl.h"


//======================================================================
hpp_tem2ctrl tem2ctrl(TEMP_N_MAX, TEMP_P_MAX);

//======================================================================
int main(int argc, char **argv)
{
	ret_t ret = RET_OK;
	if(tem2ctrl.set_arg(argc, argv ))
	{
		cerr<<"Argument that have pressed not correct. Please look to help"<<endl;
		return RET_INVALID_PARAM;
	}

//	if(HPP_TEM2CTRL_DEBUF)
	{
		cout<<endl;
		cout<<"---------------------------------------------------"<<endl;
		cout<<"   Device             : "<<tem2ctrl.get_dev()<<endl;
		cout<<"   Temperature        : "<<tem2ctrl.get_arg_temp()<<endl;
		cout<<"   Temperature number : "<<tem2ctrl.get_arg_temp_num()<<endl;
		cout<<"   Option to run      : "<<((tem2ctrl.get_prog_mod()==1)?"NONE":tem2ctrl.get_arg_opt()?"WR":"RD")<<endl;
		cout<<"   Mode to run        : "<<(tem2ctrl.get_prog_mod()==1?"DUMP":"RUN")<<endl;
		cout<<"---------------------------------------------------"<<endl<<endl;
	}

	if(tem2ctrl.get_prog_mod() == PROG_MODE_RUN)
	{
		ret = tem2ctrl.run(HPP_TEM2CTRL_BAUD_RATE,
				           HPP_TEM2CTRL_CHAR_SIZE,
						   HPP_TEM2CTRL_STOP_BIT,
						   HPP_TEM2CTRL_PARITY,
						   HPP_TEM2CTRL_FLOW_CTRL);
	}
	else if(tem2ctrl.get_prog_mod() == PROG_MODE_DUMP)
	{
		ret = tem2ctrl.dump();
	}

	if(ret)
	{
		cerr<<" The process not successful "<<endl;
	}

	return ret;

}


