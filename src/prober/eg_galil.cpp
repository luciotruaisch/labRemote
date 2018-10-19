#include <stdio.h>
#include <iostream>
#include <string>
#include <unistd.h> //for sleep function
#include <signal.h> //for signal handling

#include "ControllerGalil.h"

using namespace std;

bool emergency_stop = 0; 

void sig_handler(int signum){
	if(signum == SIGINT){
		emergency_stop = 1;
	}
}

void poll_galil(ControllerGalil* ctrl){
	string *reply, *last_reply; 
	reply = ctrl->write_with_reply("TP");
	sleep(1);
	last_reply = ctrl->write_with_reply("TP");
	while (*last_reply != *reply) {
		if (emergency_stop == 1) {
			ctrl->write("AB");
			exit(SIGINT);	
		}
		reply = ctrl->write_with_reply("TP");
		sleep(1);
		last_reply = ctrl->write_with_reply("TP");
	}
} 


int main(int argc, char** argv){

	signal(SIGINT, sig_handler);

	ControllerGalil* ctrl = new ControllerGalil("192.168.1.30");
	ctrl->connect();

	string input = "";
	while(true){
		printf("Please enter commands. p for poll. q for quit\n");
		getline(cin, input);
		if(input[0] == 'q') {
			ctrl->write("AB");
			break;	
		}
		if(input[0] == '\n') continue;
		if(input[0] == 'p') poll_galil(ctrl);
		ctrl->write(input);
	}

	delete ctrl;
	return 0;
}
