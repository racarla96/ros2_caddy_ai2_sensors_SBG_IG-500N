#include "sbgCanCommandsIg30.h"
#include "sbgCanProtocolOutputMode.h"

/*!
 *	Set the different cutoff frequencies.
 *	\param[in]	deviceHandle					A valid ig can device handle.
 *	\param[in]	cutOffGyro						The gyro cut off frequency in Hz.
 *	\param[in]	cutOffAccel						The accelero cut off frequency in Hz.
 *	\param[in]	cutOffMag						The magneto cut off frequency in Hz.
 *	\return										SBG_NO_ERROR in case of a good operation.
 */
SbgErrorCode sbgCanSetFilterFrequencies(SbgCanDeviceHandle deviceHandle, float cutOffGyro, float cutOffAccel, float cutOffMag)
{
	uint8 receivedLength;
	uint8 receivedData[8];
	uint8 dataBuffer[3*sizeof(int16)];
	SbgErrorCode errorCode;

	//
	// Fill the CAN message data
	//
	*(int16*)(dataBuffer + 0*sizeof(int16)) = sbgCanHostToTarget16((uint16) (cutOffGyro*10));
	*(int16*)(dataBuffer + 1*sizeof(int16)) = sbgCanHostToTarget16((uint16) (cutOffAccel*10));
	*(int16*)(dataBuffer + 2*sizeof(int16)) = sbgCanHostToTarget16((uint16) (cutOffMag*10));

	//
	// Create and send the CAN command
	//
	errorCode = sbgCanDeviceSendSpecificMessage(deviceHandle, SBG_CAN_ID_FILTER_FREQUENCIES, sizeof(dataBuffer), dataBuffer);

	//
	// Check if we were able to send the CAN message
	//
	if (errorCode == SBG_NO_ERROR)
	{
		//
		// Wait for an answer and return it
		//
		errorCode = sbgCanDeviceReceiveSpecificMessage(deviceHandle, SBG_CAN_ID_FILTER_FREQUENCIES, SBG_CAN_FRAME_RECEPTION_TIME_OUT, &receivedLength, receivedData);

		//
		// Check if we were able to receive the CAN message
		//
		if (errorCode == SBG_NO_ERROR)
		{
			//
			// Check if the answer is valid
			//
			if (receivedLength != 3*sizeof(int16)) 
			{
				//
				// The answer length is invalid
				//
				errorCode = SBG_INVALID_FRAME;
			}
			else if ( memcmp(receivedData, dataBuffer, 3*sizeof(int16)) != 0)
			{
				//
				// The received buffer isn't the same as the sent one
				//
				errorCode = SBG_INVALID_PARAMETER;
			}
		}
	}

	return errorCode;
}

/*!
 *	Get the different cutoff frequencies.
 *	\param[in]	deviceHandle					A valid ig can device handle.
 *	\param[out]	pCutOffGyro						Pointer to an float used to hold the gyro cut off frequency in Hz.
 *	\param[out] pCutOffAccel					Pointer to an float used to hold the accelero cut off frequency in Hz.
 *	\param[out] pCutOffMag						Pointer to an float used to hold the magneto cut off frequency in Hz.
 *	\return										SBG_NO_ERROR in case of a good operation.
 */
SbgErrorCode sbgCanGetFilterFrequencies(SbgCanDeviceHandle deviceHandle, float *pCutOffGyro, float *pCutOffAccel, float *pCutOffMag)
{
	uint8 receivedLength;
	uint8 receivedData[8];
	uint16 cutOffGyro;
	uint16 cutOffAccel;
	uint16 cutOffMag;
	SbgErrorCode errorCode;

	//
	// Create and send the CAN command
	//
	errorCode = sbgCanDeviceSendSpecificMessage(deviceHandle, SBG_CAN_ID_FILTER_FREQUENCIES, 0, NULL);

	//
	// Check if we were able to send the CAN message
	//
	if (errorCode == SBG_NO_ERROR)
	{
		//
		// Wait for an answer and return it
		//
		errorCode = sbgCanDeviceReceiveSpecificMessage(deviceHandle, SBG_CAN_ID_FILTER_FREQUENCIES, SBG_CAN_FRAME_RECEPTION_TIME_OUT, &receivedLength, receivedData);
		
		//
		// Check if we were able to receive the CAN message
		//
		if (errorCode == SBG_NO_ERROR)
		{
			//
			// Check if the answer length is valid
			//
			if (receivedLength == 3*sizeof(int16))
			{
				//
				// Get the raw answer data
				//
				cutOffGyro	= sbgCanTargetToHost16( *(uint16*)(receivedData + 0*sizeof(int16)) );
				cutOffAccel	= sbgCanTargetToHost16( *(uint16*)(receivedData + 1*sizeof(int16)) );
				cutOffMag	= sbgCanTargetToHost16( *(uint16*)(receivedData + 2*sizeof(int16)) );
					
				//
				// If possible, convert and return the gyroscope cut-off frequency
				//
				if (pCutOffGyro)
				{
					*pCutOffGyro = ((float)cutOffGyro)/10.0f;
				}

				//
				// If possible, convert and return the acceleromters cut-off frequency
				//
				if (pCutOffAccel)
				{
					*pCutOffAc                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                 Kalman period in 0.1 ms.
 *	\param[in]	kalOptions						The Kalman filter attitude mode.
 *	\return										SBG_NO_ERROR in case of a good operation.
 */
SbgErrorCode sbgCanSetKalmanFilter(SbgCanDeviceHandle deviceHandle, uint16 kalPeriod, uint16 kalOptions)
{
	uint8 receivedLength;
	uint8 receivedData[8];
	uint8 dataBuffer[2*sizeof(uint16)];
	SbgErrorCode errorCode;
	
	//
	// Fill the CAN message data
	//
	*(uint16*)(dataBuffer) = sbgCanHostToTarget16(kalPeriod);
	*(uint16*)(dataBuffer + sizeof(uint16)) = sbgCanHostToTarget16(kalOptions);
	
	//
	// Create and send the CAN command
	//
	errorCode = sbgCanDeviceSendSpecificMessage(deviceHandle, SBG_CAN_ID_KALMAN_FILTER, sizeof(dataBuffer), dataBuffer);

	//
	// Check if we were able to send the CAN message
	//
	if (errorCode == SBG_NO_ERROR)
	{
		//
		// Wait for an answer and return it
		//
		errorCode = sbgCanDeviceReceiveSpecificMessage(deviceHandle, SBG_CAN_ID_KALMAN_FILTER, SBG_CAN_FRAME_RECEPTION_TIME_OUT, &receivedLength, receivedData);
		
		//
		// Check if we were able to receive the CAN message
		//
		if (errorCode == SBG_NO_ERROR)
		{
			//
			// Check if the answer is valid
			//
			if (receivedLength != 2*sizeof(uint16)) 
			{
				//
				// The answer length is invalid
				//
				errorCode = SBG_INVALID_FRAME;
			}
			else if ( memcmp(receivedData, dataBuffer, 2*sizeof(uint16)) != 0)
			{
				//
				// The received buffer isn't the same as the sent one
				//
				errorCode = SBG_INVALID_PARAMETER;
			}
		}
	}

	return errorCode;
}

/*!
 *	Get the Kalman filter period and mode.
 *	\param[in]	deviceHandle					A valid ig can device handle.
 *	\param[out]	pKalPeriod						Pointer to an uint16 used to hold the Kalman period in 0.1 ms unit.
 *	\param[out] pKalOptions						Pointer to an uint16 used to hold the Kalman filter attitude mode.
 *	\return										SBG_NO_ERROR in case of a good operation.
 */
SbgErrorCode sbgCanGetKalmanFilter(SbgCanDeviceHandle deviceHandle, uint16 *pKalPeriod, uint16 *pKalOptions)
{
	uint8 receivedLength;
	uint8 receivedData[8];
	SbgErrorCode errorCode;
	
	//
	// Create and send the CAN command
	//
	errorCode = sbgCanDeviceSendSpecificMessage(deviceHandle, SBG_CAN_ID_KALMAN_FILTER, 0, NULL);

	//
	// Check if we were able to send the CAN message
	//
	if (errorCode == SBG_NO_ERROR)
	{
		//
		// Wait for an answer and return it
		//
		errorCode = sbgCanDeviceReceiveSpecificMessage(deviceHandle, SBG_CAN_ID_KALMAN_FILTER, SBG_CAN_FRAME_RECEPTION_TIME_OUT, &receivedLength, receivedData);
		
		//
		// Check if we were able to receive the CAN message
		//
		if (errorCode == SBG_NO_ERROR)
		{
			//
			// Check if the answer length is valid
			//
			if (receivedLength == 2*sizeof(uint16))
			{
				//
				// If possible, return the Kalman filter period
				//
				if (pKalPeriod)
				{
					*pKalPeriod = sbgCanTargetToHost16( *(uint16*)(receivedData));
				}

				//
				// If possible, return the Kalman options mask
				//
				if (pKalOptions)
				{
					*pKalOptions = sbgCanTargetToHost16( *(uint16*)(receivedData + sizeof(uint16)));
				}
			}
			else
			{
				//
				// The answer length is invalid
				//
				errorCode = SBG_INVALID_FRAME;
			}
		}
	}

	return errorCode;
}

/*!
 *	Set the GPS different options.
 *	\param[in]	deviceHandle					A valid ig can device handle.
 *	\param[in]	model							The gps model.
 *	\param[in]	options							The gps options.
 *	\return										SBG_NO_ERROR if no error.
 */
SbgErrorCode sbgCanSetGpsOptions(SbgCanDeviceHandle deviceHandle, SbgCanGpsDynamicModel model, uint8 options)
{
	uint8 receivedLength;
	uint8 receivedData[8];
	uint8 dataBuffer[2*sizeof(uint8)];
	SbgErrorCode errorCode;

	//
	// Fill the CAN message data
	//
	dataBuffer[0] = (uint8)model;
	dataBuffer[1] = options;

	//
	// Create and send the CAN message
	//
	errorCode = sbgCanDeviceSendSpecificMessage(deviceHandle, SBG_CAN_ID_GPS_OPTIONS, sizeof(dataBuffer), dataBuffer);
		
	//
	// Check if we were able to send the CAN message
	//
	if (errorCode == SBG_NO_ERROR)
	{
		//
		// Wait for an answer and return it
		//
		errorCode = sbgCanDeviceReceiveSpecificMessage(deviceHandle, SBG_CAN_ID_GPS_OPTIONS, SBG_CAN_FRAME_RECEPTION_TIME_OUT, &receivedLength, receivedData);

		//
		// Check if we were able to receive a valid answer
		//
		if (errorCode == SBG_NO_ERROR)
		{
			//
			// Check if the answer is valid
			//
			if (receivedLength != 2*sizeof(uint8))
			{
				errorCode = SBG_INVALID_FRAME;
			}
			else if (memcmp(receivedData, dataBuffer, 2*sizeof(uint8)) != 0)
			{
				errorCode = SBG_INVALID_PARAMETER;
			}
		}
	}

	return errorCode;
}

/*!
 *	Get the GPS different options.
 *	\param[in]	deviceHandle					A valid ig can device handle.
 *	\param[out]	pModel							Pointer to an uint8 used to hold the gps model.
 *	\param[out]	pOptions						Pointer to an uint8 used to hold the gps options.
 *	\return										SBG_NO_ERROR if no error.
 */
SbgErrorCode sbgCanGetGpsOptions(SbgCanDeviceHandle deviceHandle, SbgCanGpsDynamicModel *pModel, uint8 *pOptions)
{
	uint8 receivedLength;
	uint8 receivedData[8];
	SbgErrorCode errorCode;

	//
	// Create and send the CAN message
	//
	errorCode = sbgCanDeviceSendSpecificMessage(deviceHandle, SBG_CAN_ID_GPS_OPTIONS, 0, NULL);

	//
	// Check if we were able to send the CAN message
	//
	if (errorCode == SBG_NO_ERROR)
	{
		//
		// Wait for an answer and return it
		//
		errorCode = sbgCanDeviceReceiveSpecificMessage(deviceHandle, SBG_CAN_ID_GPS_OPTIONS, SBG_CAN_FRAME_RECEPTION_TIME_OUT, &receivedLength, receivedData);

		//
		// Check if we were able to receive a valid answer
		//
		if (errorCode == SBG_NO_ERROR)
		{
			//
			// Check if the answer is valid
			//
			if (receivedLength == 2*sizeof(uint8))
			{
				//
				// Return the configuration if needed
				//
				if (pModel)
				{
					*pModel = (SbgCanGpsDynamicModel)receivedData[0];
				}
				if (pOptions)
				{
					*pOptions = receivedData[1];
				}
			}
			else
			{
				errorCode = SBG_INVALID_FRAME;
			}
		}
	}

	return errorCode;
}

