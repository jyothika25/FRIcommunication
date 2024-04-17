#pragma once
#ifndef _KUKA_FRICLIENT_H
#define _KUKA_FRICLIENT_H

#include "friLBRClient.h"
#include <friClientIf.h>



/**
 * \brief Template client implementation.
 */
class FRIClient : public KUKA::FRI::LBRClient
{

public:

	/**
	 * \brief Constructor.
	 */
	FRIClient();

	/**
	 * \brief Destructor.
	 */
	~FRIClient();

	/**
	 * \brief Callback for FRI state changes.
	 *
	 * @param oldState
	 * @param newState
	 */
	virtual void onStateChange(KUKA::FRI::ESessionState oldState, KUKA::FRI::ESessionState newState);

	/**
	 * \brief Callback for the FRI session states 'Monitoring Wait' and 'Monitoring Ready'.
	 *
	 * If you do not want to change the default-behavior, you do not have to implement this method.
	 */
	virtual void monitor();

	/**
	 * \brief Callback for the FRI session state 'Commanding Wait'.
	 *
	 * If you do not want to change the default-behavior, you do not have to implement this method.
	 */
	virtual void waitForCommand();

	/**
	 * \brief Callback for the FRI state 'Commanding Active'.
	 *
	 * If you do not want to change the default-behavior, you do not have to implement this method.
	 */
	virtual void command();

	//const LBRState&LBRClient::robotState();

//	LBRCommand& robotCommand();


//	LBRState _robotState;      //!< wrapper class for the FRI monitoring message
//	LBRCommand _robotCommand;  //!< wrapper class for the FRI command message
	
	const double* jp;
	
	const double* ijp; 


	//double apple_array[499];
	//int iter;
	/* 
	#include "json.h"
	take apple_array and convert it 
	json json_apples;
	json_apples.convert(apple_array);
	json_apples.saveas("myFirstExperiment.json", addTime=True);
	*/
	
	int noOfaxis= KUKA::FRI::LBRState::NUMBER_OF_JOINTS;
	double newjointPos[KUKA::FRI::LBRState::NUMBER_OF_JOINTS];
};


#endif//  _KUKA_FRICLIENT_H

