#include <cstdio> 
#include <iostream>
#include "FRIClient.h"
#include "friLBRState.h"
#include "friLBRCommand.h"




using namespace KUKA::FRI;
//******************************************************************************




FRIClient::FRIClient()
{
	printf("FRIClient intialized\n");
	for (int i = 0; i < LBRState::NUMBER_OF_JOINTS; i++) 
	{ 
		newjointPos[i] = 0.0;
	}

}

//******************************************************************************
FRIClient::~FRIClient()
{
}
//******************************************************************************
void FRIClient::onStateChange(ESessionState oldState, ESessionState newState)
{
	LBRClient::onStateChange(oldState, newState);
	// react on state change events
	switch (newState)
	{

	 case MONITORING_READY:
	 {

		
		std::cout << "sampletime " << robotState().getSampleTime() << '\n';

	
		std::cout << "connectionquality " << robotState().getConnectionQuality() << '\n';

		
		std::cout << "FRISessionState " << robotState().getSessionState() << '\n';

		
		std::cout << "ClientCommandMode " << robotState().getClientCommandMode() << '\n';
		for (int i = 0; i < LBRState::NUMBER_OF_JOINTS; i++)
		{
			newjointPos[i] = 0.0;
		}
		break;
	 }
	 
	 default:
	 {
		break;
	 }
	
	}
}

//******************************************************************************
void FRIClient::monitor()
{
	LBRClient::monitor();

	
	std::cout << "connectionquality " << robotState().getConnectionQuality() << '\n';
	

	

	//apple_array[iter] = robotState().getConnectionQuality();
	//iter++;

	for (int i = 0; i < noOfaxis; i++)
	{
	  
	 jp = robotState().getMeasuredJointPosition();
	 std::cout << '\n' << "MeasuredJointPosition of Axis" << i+1 <<" "<< jp[i] << '\n';
	
	
	}
	
	
	/*std::cout << '\n';
	mt= robotState().getMeasuredTorque();
	std::cout << "MeasuredTorque Axis 1" << mt[0] << '\n';

	ct= robotState().getCommandedTorque();
	std::cout << "CommandedTorque " << ct << '\n';

	et= robotState().getExternalTorque();
	std::cout << "ExternalTorque " << et << '\n';*/
}

//******************************************************************************
void FRIClient::waitForCommand()
{
	// In waitForCommand(), the joint values have to be mirrored. Which is done, 
	// by calling the base method.

	LBRClient::waitForCommand();

	std::cout << "Entering into commanding wait mode " << '\n';

	
	std::cout << "connectionquality " << robotState().getConnectionQuality() << '\n';

       

    if (robotState().getClientCommandMode() == POSITION)
	{

		robotCommand().setJointPosition(newjointPos);
	}
	

	

}

//******************************************************************************
void FRIClient::command()
{

	LBRClient::command();

	std::cout << "Entering into command mode "  << '\n';
	

	std::cout << "connectionquality " << robotState().getConnectionQuality() << '\n';
	
	if (robotState().getClientCommandMode() == POSITION)
	{
		ijp = robotState().getIpoJointPosition();
	
		memcpy(newjointPos,ijp, LBRState::NUMBER_OF_JOINTS * sizeof(double));
	  
		for (int i = 0; i < LBRState::NUMBER_OF_JOINTS; i++)
		{ 
		  std::cout << "IpoJointPosition of Axis"<<i + 1 <<" "<< ijp[i] << '\n';
		  newjointPos[i] = ijp[i]+ 0.0001;
		  std::cout << "new IpoJointPosition of Axis" << i + 1 << " " << newjointPos[i] << '\n';
		}

		robotCommand().setJointPosition(newjointPos);
	   
	}


}




