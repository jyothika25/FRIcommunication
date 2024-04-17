#include <cstdlib>
#include <cstdio>
#include <cstring> // strstr
#include <stdio.h> //rtt
#include <chrono>
#include "FRIClient.h"
#include "friUdpConnection.h"
#include "friClientApplication.h"


using namespace KUKA::FRI;


#define DEFAULT_PORTID 30200



int main(int argc, char** argv)
{

	/************************************************************************/
	// time measurement starts
	printf("Time measuresurement starts\n");
	
	// Start measuring time
	auto begin = std::chrono::high_resolution_clock::now();

	
	/*******************************************************************/

	// create new client
	FRIClient lbrClient;
	
	
	// create new udp connection
	UdpConnection connection;


	// pass connection and client to a new FRI client application
	ClientApplication app(connection, lbrClient);

	// Connect client application to KUKA Sunrise controller.
	// Parameter NULL means: repeat to the address, which sends the data
	// @todo understand what happened and write a short paragraph in your thesis describing the communication issue and solution by hardcoding the ip of koni in your app

	app.connect(DEFAULT_PORTID, "172.30.0.147");


	// repeatedly call the step routine to receive and process FRI packets
	bool success = true;
	while (success)
	{
		success = app.step();

		// check if we are in IDLE because the FRI session was closed
		if (lbrClient.robotState().getSessionState() == IDLE)
		{
			// We simply quit. Waiting for a FRI new session would be another possibility.
			break;
		}
	}


	

	/************************************************************************/

	// time measurement ends
	auto end = std::chrono::high_resolution_clock::now();
	auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(end - begin);
	printf("Time measured: %.3f seconds.\n", elapsed.count() * 1e-9);
	return 0;

	// disconnect from controller
	app.disconnect();
	return 1;
	
}
