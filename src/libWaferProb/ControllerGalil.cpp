#include <stdio.h>
#include <stdlib.h>
# include <string.h>
#include <unistd.h>

#include "ControllerGalil.h"

#include <vector>
#include "Helper.h"

ControllerGalil::ControllerGalil(const char* device_name){
	char device[256];
	sprintf(device, "%s --subscribe ALL", device_name);
	dn = string(device);
	port = 0;
	m_position[0] = m_position[1] = m_position[2] = DEFAULT_GALIL_POS;
	m_raw_position[0] = m_raw_position[1] = m_raw_position[2] = DEFAULT_GALIL_POS;
	//TO DO: to read in from the text file "z_range.txt"
	m_ymax = 600000;//765253;
	m_ymin = -100000;//-364475;
}

ControllerGalil::~ControllerGalil(){
	disconnect();
}

int ControllerGalil::connect(){
	if(m_is_connected) return 0;

	printf("%s: connecting to device %s\n", __FUNCTION__, dn.c_str());
	if( check(GOpen(dn.c_str(), &port)) ){
		printf("%s connected\n", dn.c_str());
		get_position();
		status = 0;
		m_is_connected = true;
	} else {
		printf("%s not connected\n", dn.c_str());
		status = 1;
	}
	return status;
}

int ControllerGalil::disconnect(){
	if(port != 0){
		GClose(port);
		port = 0;
	}
	return 0;
}

int ControllerGalil::write(const string& cmd){
	if (port == 0){
		printf("%s is not open\n", dn.c_str());
		return -1;
	}
	char* trimmed;
	if( ! check(GCmdT(port, cmd.c_str(), buf, sizeof(buf), &trimmed)) )
	{
		printf("%s is not recognized\n", cmd.c_str());
		return -2;
	}
	printf("Galil: %s --> %s\n", cmd.c_str(), buf);

	poll_position();
	return 0;
}

string* ControllerGalil::write_with_reply(const string& cmd){
	if (port == 0){
		printf("%s is not open\n", dn.c_str());
		return NULL;
	}
	char* trimmed;
	if( ! check(GCmdT(port, cmd.c_str(), buf, sizeof(buf), &trimmed)) )
	{
		printf("%s is not recognized\n", cmd.c_str());
		return NULL;
	}
	printf("Galil: %s --> %s\n", cmd.c_str(), buf);
	string* result = new string(buf);	

//	poll_position();	
	return result;
}

int ControllerGalil::stop(){
	int status = write("AB");
	get_position();
	return status;
}

int ControllerGalil::set_speed(int axis, float sp)
{
	int steps = convert_mm_to_turns(sp);

	string cmd = generate_cmd("SP", axis, steps);
	return write(cmd);
}

int ControllerGalil::mv_abs(int axis, float value){
	int steps = convert_mm_to_turns(value - m_position[axis]);
	string cmd = generate_cmd("PR", axis, steps);
	write(cmd);
	make_a_move(axis);
	return 0;
}

int ControllerGalil::mv_rel(int axis, float value){
	int steps = convert_mm_to_turns(value);
	string cmd = generate_cmd("PR", axis, steps);
	printf("%s\n", cmd.c_str());
	write(cmd);
	make_a_move(axis);
	return 0;
}

int ControllerGalil::get_position(){
	if(port == 0) return -1;

	char* trimmed;
	GCmdT(port, "RP", buf, sizeof(buf), &trimmed);
	string data(trimmed);
	printf("Galic: RP --> %s\n", trimmed);

	vector<string> raw_items;
	WaferProb::tokenizeString(data, ',', raw_items);
	for(int i = 0; i < 3; i++)
	{
		float raw_pos = atof(raw_items.at(i).c_str());
		m_raw_position[i] = raw_pos;
		m_position[i] = convert_turns_to_mm(raw_pos);
	}
	return 0;
}


int ControllerGalil::set_home(){
	return 0;
}

int ControllerGalil::set_center(){
	return 0;
}

string ControllerGalil::generate_cmd(const char* cmd, int axis, int steps)
{
	char val_str[256];
	sprintf(val_str, "%d", steps);
	string cmd_val(",,");
	cmd_val.insert(axis, val_str);

	char res_str[256];
	sprintf(res_str, "%s %s", cmd, cmd_val.c_str());
	return string(res_str);
}

void ControllerGalil::make_a_move(int axis){
	if(axis == 2 && !m_z_calibrated) {
		printf("ERROR, Z-axis is not calibrated!");
		return;
	}
	char mv[256];
	char axis_name = axis_index_to_name(axis);
	sprintf(mv, "BG %c", axis_name);

	write(mv);
	// block until motion is complete.
	GMotionComplete(port, string(1, axis_name).c_str());

	get_position();
}

void ControllerGalil::find_max_min()
{
	m_z_calibrated = true;

	int axis = 2;
	set_speed(axis, 1); //second argument: mm per s 
	string cmd = generate_cmd("PA", axis, 1000000);
	write(cmd);
	make_a_move(axis);

	get_position();
	m_ymax = m_raw_position[2];
	printf("HELLO: %.2f\n", m_ymax);

	string cmd2 = generate_cmd("PA", axis, -1000000);
	write(cmd2);
	make_a_move(axis);
	get_position();
	m_ymin = m_raw_position[2];

	printf("range of z-axis in turns: [%.2f, %.2f]\n", m_ymin, m_ymax);

	//float quarter =  (m_ymax - m_ymin)/4;
	//if (quarter<0) quarter = -quarter;
	//float quarter_up = m_ymin +  quarter;

	//Move quarter of the range up from the minimum
	//string cmd3 = generate_cmd("PA", axis, quarter_up);
	//write(cmd3);
	//make_a_move(axis);
	//get_position();

}

void ControllerGalil::find_z_min(){
	m_z_calibrated = true;

	int axis = 2;
	set_speed(axis, 1);

	string cmd = generate_cmd("PA", axis, -1000000);
	write(cmd);
	make_a_move(axis);
	get_position();
	m_ymin = m_raw_position[2];
	printf("HELLO: %.2f\n", m_ymin);
}

void ControllerGalil::check_z_min() {
	int axis = 2;
	string cmd2 = generate_cmd("PA", axis, -1000000);
	write(cmd2);
	make_a_move(axis);
	get_position();
	if (m_position[2] != 0) m_z_calibrated = false;
}

void ControllerGalil::poll_position() {
	string *reply, *last_reply; 
	reply = write_with_reply("TP");
	sleep(1);
	last_reply = write_with_reply("TP");
	while (*last_reply != *reply) {
		reply = write_with_reply("TP");
		sleep(1);
		last_reply = write_with_reply("TP");
	}
}

