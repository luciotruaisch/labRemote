#include "ControllerBase.h"
#include "Helper.h"

#include <stdio.h>
#include <sstream>

ControllerBase::ControllerBase()
{
	status = -1;
	m_is_connected = false;
	m_z_calibrated = true;
}

ControllerBase::~ControllerBase(){
}

void ControllerBase::print_cmd(){
	printf( "MA X 10  --> move X-axis w.r.t home position 10 millimeter\n"
			"MR X 10 --> move X-axis w.r.t current position 10 millimeter\n"
			"SP X 10  --> set speed in x-axis direction by 10 millimeter/s\n"
			"SH --> move to HOME\n"
			"SM --> move to center\n"
			"FZRANGE --> find maximum and minimum of Z-axis\n"
			"FZMIN --> find minimum of Z-axis\n"
			"CHECKZMIN --> check the minimum position of Z-axis\n"
			"XY_PARK --> park the xy controller \n"
			"XY_UNPARK --> unpark the xy controller \n"
			"POLL_POS --> poll positions of controllers\n"
			"----------------------------------------------------------\n"
	      );
}

int ControllerBase::run_cmd(const string& cmd) {
	printf("HELLO??? %s\n", cmd.c_str());
	int axis = -1;

	if(cmd.empty()){
		return axis;
	}
	vector<string> raw_items;
	WaferProb::tokenizeString(cmd, ' ', raw_items);

	vector<string> items;
	// convert commands to upper case.
	for(auto& item: raw_items){
		items.push_back(WaferProb::toUpper(item));
	}

	const string& action(items[0]);

	// Check each case..
	if (action == "MA")
	{
		if(items.size() != 3){
			printf("argument of MA is wrong\n"
					"MA X/Y/Z 10\n");
			return axis;
		}
		axis = WaferProb::axis_number(items[1]);
		this->mv_abs(axis, atof(items[2].c_str()));
	} else if (action == "MR")
	{
		if(items.size() != 3){
			printf("argument of MR is wrong\n"
					"MR X/Y/Z 10\n");
			return axis;
		}
		axis = WaferProb::axis_number(items[1]);
		this->mv_rel(axis, atof(items[2].c_str()));

	} else if (action == "SH")
	{
		this->set_home();
		axis = 3;
	} else if (action == "SM")
	{
		this->set_center();
		axis = 3;
	} else if (action == "SP")
	{
		if (items.size() != 3){
			printf("argument of SP is wrong\n"
					"SP X/Y/Z 10000\n");
			return axis;
		}
		axis = WaferProb::axis_number(items[1]);
		this->set_speed(axis, atof(items[2].c_str()));
	} else if (action == "FZRANGE") 
	{
		this->find_max_min();
	} else if (action == "FZMIN"){
		this->find_z_min();
	} else if (action == "CHECKZMIN") {
		this->check_z_min();
	} else if (action == "XY_PARK"){
		this->xy_park();
	} else if (action == "XY_UNPARK") {
		this->xy_unpark();
	} else {
		printf("%s not supported yet!\n", action.c_str());
		print_cmd();
	}
	this->get_position();
	return axis;
}
