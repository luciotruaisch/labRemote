/*
   Writing a program to talk to Zaber Controller.
   */

#include <stdio.h>
#include <string.h>
#include <string>
#include <iostream>
#include <sstream>
#include <unistd.h> //for sleep function
#include <signal.h> //for signal handling

#include "za_serial.h"


bool emergency_stop = 0; 

void sig_handler(int signum){
	if(signum == SIGINT){
		emergency_stop = 1;
	}

	//printf("Received SIGINT. Exiting program.\n");
	//exit(signum);	
}


//unsigned sleep(unsigned seconds);signal handling

void poll_zaber(z_port port){
	char reply[256] = { 0 };
	struct za_reply decoded_reply;
	za_send(port, "/"); // '/' generates a response form all devices in the chain
	za_receive(port, reply, sizeof(reply));
	za_decode(&decoded_reply, reply);

	std::cout << reply << std::endl;

	char* current_status = reply;
	while(strstr(current_status, "BUSY") != NULL ){
		if(emergency_stop == 1){
			za_send(port,"/estop");
			exit(SIGINT);
		}
		za_send(port, "/ \n");
		za_receive(port, reply, sizeof(reply));
		std::cout << reply << std::endl;
		current_status = reply;
		sleep(0.5);
	}
}

void send(z_port port, const char* cmd){
	char reply[256] = {0};
	za_send(port, cmd);
	za_receive(port, reply, sizeof(reply));
	std::cout << reply << std::endl;

	//poll_zaber(port);
}

int main(int argc, char** argv){

	signal(SIGINT, sig_handler);


	if (argc < 2){	//if the program is called without specifiying where the USB connection is, reminder
		printf("%s device_name\n", argv[0]);
		printf("e.g: /dev/ttyACM0, /dev/tty.usbmodem1411\n");
		return 1;
	}	

	z_port port;
	char *device_name = argv[1]; // /dev/tty___


	if(za_connect(&port, device_name) != Z_SUCCESS){
		printf("Could not connect to device %s.\n", device_name);	
		return -1;
	}
	else{
		printf("%s is connected\n", device_name);
	}



	std::string input = "";
	while(true){
		printf("Please enter commands. p for poll.  q for quit\n");
		getline(std::cin, input);
		//if(emergency_stop == 1){
		//    za_send(port,"/estop");
		//    exit(SIGINT);
		//}
		if(input[0] == 'q'){
			za_send(port, "/estop");
			break;
		}
		if(input[0] == '\n') continue;
		if(input[0] == 'p') poll_zaber(port);
		char cmd[256];
		sprintf(cmd, "%s\n", input.c_str());		
		send(port, cmd);
	}

	za_disconnect(port);
	return 0;

}



