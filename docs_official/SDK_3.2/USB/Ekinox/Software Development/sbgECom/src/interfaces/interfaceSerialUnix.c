#include "interfaceSerial.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <sys/ioctl.h>
#include "../time/sbgTime.h"

//----------------------------------------------------------------------//
//- Private methods declarations                                       -//
//----------------------------------------------------------------------//


/*!
 *	Returns the right unix baud rate const according to a baud rate value.
 *	\param[in] baudRate		The baud rate value (ie 115200).
 *	\return					The Unix baud rate constant.
 */
uint32 sbgInterfaceSerialGetBaudRateConst(uint32 baudRate)
{
	uint32 baudRateConst;
	
	//
	// Create the right baud rate value for unix platforms
	//
	switch (baudRate)
	{
		case 9600:
			baudRateConst = B9600;
			break;
		case 19200:
			baudRateConst = B19200;
			break;
#ifdef B38400
		case 38400:
			baudRateConst = B38400;
			break;
#endif
#ifdef B57600
		case 57600:
			baudRateConst = B57600;
			break;
#endif
#ifdef B115200
		case 115200:
			baudRateConst = B115200;
			break;
#endif
#ifdef B230400
		case 230400:
			baudRateConst = B230400;
			break;
#endif
#ifdef B460800
		case 460800:
			baudRateConst = B460800;
			break;
#endif
#ifdef B921600
		case 921600:
			baudRateConst = B921600;
			break;
#endif
		default:
			baudRateConst = baudRate;
	}

	return baudRateConst;
}

//----------------------------------------------------------------------//
//- Operations methods declarations                                    -//
//----------------------------------------------------------------------//

/*!
 *	Initialize a serial interface for read and write operations.
 *	\param[in]	pHandle							Pointer on an allocated interface instance to initialize.
 *	\param[in]	deviceName						Serial interface location (COM21 , /dev/ttys0, depending on platform).
 *	\param[in]	baudRate						Serial interface baud rate in bps.
 *	\return										SBG_NO_ERROR if the interface has been created.
 */
SbgErrorCode sbgInterfaceSerialCreate(SbgInterface *pHandle, const char *deviceName, uint32 baudRate)
{
    int32               hSerialHandle;
    struct termios      options;
    uint32              baudRateConst;
    
	//
	// First check if we have a valid pHandle
	//
	if (pHandle)
	{
		//
		// Check if the device name is valid
		//
		if (deviceName)
		{
            //
            // Get our baud rate const for our Unix platform
            //
            baudRateConst = sbgInterfaceSerialGetBaudRateConst(baudRate);
            
            //
            // Init the com port
            //
            hSerialHandle = open(deviceName, O_RDWR | O_NOCTTY | O_NDELAY);
			
			//
			// Test that the port has been initialized
			//
			if (hSerialHandle != -1)
			{
                //
                // Don't block on read call if no data are available
                //
                if (fcntl(hSerialHandle, F_SETFL, O_NONBLOCK) != -1)
                {
                    //
                    // Retreive current options
                    //
                    if (tcgetattr(hSerialHandle, &options) != -1)
                    {
                        //
                        // Define com port options
                        //
                        options.c_cflag |=  (CLOCAL | CREAD);		// Enable the receiver and set local mode...
                        options.c_cflag &= ~(PARENB|CSTOPB|CSIZE);	// No parity, 1 stop bit, mask character size bits
                        options.c_cflag |= CS8;						// Select 8 data bits
                        options.c_cflag &= ~CRTSCTS;				// Disable Hardware flow control
                        
                        //
                        // Disable software flow control
                        //
                        options.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL|IXON);
                        
                        //
                        // We would like raw input
                        //
                        options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG /*| IEXTEN | ECHONL*/);
                        options.c_oflag &= ~OPOST;

                        //
                        // Set our timeout to 0
                        //
                        options.c_cc[VMIN]     = 0;
                        options.c_cc[VTIME]    = 1;
                        
                        //
                        // Set both input and output baud
                        //
                        if ( (cfsetispeed(&options, baudRateConst) != -1)  && (cfsetospeed(&options, baudRateConst) != -1) )
                        {
                            //
                            // Define options
                            //
                            if (tcsetattr(hSerialHandle, TCSANOW, &options) != -1)
                            {								
                                //
                                // The serial port is ready so create a new serial interface
                                //
                                pHandle->handle = (void*)hSerialHandle;
                                pHandle->type = SBG_IF_TYPE_SERIAL;
                                pHandle->pReadFunc = sbgInterfaceSerialRead;
                                pHandle->pWriteFunc = sbgInterfaceSerialWrite;
                                
                                //
                                // Purge the communication
                                //
                                return sbgInterfaceSerialFlush(pHandle);
                            }
                            else
                            {
                                fprintf(stderr, "sbgInterfaceSerialCreate: tcsetattr fails.\n");
                            }
                        }
                        else
                        {
                            fprintf(stderr, "sbgInterfaceSerialCreate: Unable to set speed.\n");
                        }
                    }
                    else
                    {
                        fprintf(stderr, "sbgInterfaceSerialCreate: tcgetattr fails.\n");
                    }
                }
                else
                {
                    fprintf(stderr, "sbgInterfaceSerialCreate: fcntl fails\n");
                }
            }
            else
            {
                fprintf(stderr, "sbgInterfaceSerialCreate: Unable to open the com port: %s\n", deviceName);
            }
            
            return SBG_ERROR;
		}
		else
		{
			//
			// Invalid device name
			//
			return SBG_INVALID_PARAMETER;
		}
	}
	else
	{
		return SBG_NULL_POINTER;
	}
}

/*!
 *	Destroy an interface initialized using sbgInterfaceSerialCreate.
 *	\param[in]	pInterface						Valid handle on an initialized interface.
 *	\return										SBG_NO_ERROR if the interface has been closed and released.
 */
SbgErrorCode sbgInterfaceSerialDestroy(SbgInterface *pHandle)
{
    int32   hSerialHandle;
    
    //
	// Test that we have a valid interface
	//
	if (pHandle)
	{
		//
		// Get the internal serial handle
		//
		hSerialHandle = (int32)pHandle->handle;
		
		//
		// Close the port com
		//
		close(hSerialHandle);
		pHandle->handle = NULL;

		return SBG_NO_ERROR;
	}
	else
	{
		return SBG_NULL_POINTER;
	}
}

/*!
 * Flush the RX and TX buffers (remove all old data)
 * \param[in]	handle				Valid handle on an initialized interface.
 * \return							SBG_NO_ERROR if everything is OK
 */
SbgErrorCode sbgInterfaceSerialFlush(SbgInterface *pHandle)
{
	int32   hSerialHandle;
	
	//
	// Test that we have a valid interface
	//
	if (pHandle)
	{
		//
		// Get the internal serial handle
		//
		hSerialHandle = (int32)pHandle->handle;
		
		//
		// Flush our port
		//
		if (tcflush(hSerialHandle, TCIOFLUSH) == 0)
		{
		  return SBG_NO_ERROR;
        }
        else
        {
            return SBG_ERROR;
        }
	}
	else
	{
		return SBG_NULL_POINTER;
	}
}

/*!
 * Change the serial interface baud rate immediatly.
 * \param[in]	handle				Valid handle on an initialized interface.
 * \param[in]	baudRate			The new baudrate to apply in bps.
 * \return							SBG_NO_ERROR if everything is OK
 */
SbgErrorCode sbgInterfaceSerialChangeBaudrate(SbgInterface *pHandle, uint32 baudRate)
{
	int32   hSerialHandle;
	
	//
	// Test that we have a valid interface
	//
	if (pHandle)
	{
		//
		// Get the internal serial handle
		//
		hSerialHandle = (int32)pHandle->handle;
		
		//
		// Flush our port
		//
		if (tcflush(hSerialHandle, TCIOFLUSH) == 0)
		{
		  return SBG_NO_ERROR;
        }
        else
        {
            return SBG_ERROR;
        }
	}
	else
	{
		return SBG_NULL_POINTER;
	}
}

int32 fileId = *((int32*)&handle);
	struct termios options;
	uint32 baudRateConst;

	if (handle != SBG_INVALID_DEVICE_HANDLE)
	{
		//
		// Get our baud rate const for our Unix platform
		//
		baudRateConst = sbgDeviceGetBaudRateConst(baudRate);
		
		//
		// Retreive current options
		//
		if (tcgetattr(fileId, &options) != -1)
		{	
			//
			// Set both input and output baud
			//
			if ( (cfsetispeed(&options, baudRateConst) == -1)  || (cfsetospeed(&options, baudRateConst) == -1) )
			{
				fprintf(stderr, "sbgDeviceChangeBaud: Unable to set speed.\n");
				return SBG_ERROR;
			}
			
			//
			// Define options
			//
			if (tcsetattr(fileId, TCSADRAIN, &options) != -1)
			{
				return SBG_NO_ERROR;
			}
			else
			{
				fprintf(stderr, "sbgDeviceChangeBaud: tcsetattr fails.\n");
				return SBG_ERROR;
			}
		}
		else
		{
			fprintf(stderr, "sbgDeviceChangeBaud: tcgetattr fails.\n");
			return SBG_ERROR;
		}
	}
	else
	{
		return SBG_NULL_POINTER;
	}

//----------------------------------------------------------------------//
//- Internal interfaces write/read implementations                     -//
//----------------------------------------------------------------------//

/*!
 * Try to write some data to an interface.
 * \param[in]	pHandle									Valid handle on an initialized interface.
 * \param[in]	pBuffer									Pointer on an allocated buffer that contains the data to write
 * \param[in]	bytesToWrite							Number of bytes we would like to write.
 * \return												SBG_NO_ERROR if all bytes have been written successfully.
 */
SbgErrorCode sbgInterfaceSerialWrite(SbgInterface *pHandle, const void *pBuffer, uint32 bytesToWrite)
{
	uint32          numBytesLeftToWrite = bytesToWrite;
	uint8          *pCurrentBuffer = (uint8*)pBuffer;
	uint32          numBytesWritten;
	int32           hSerialHandle;
	
	//
	// Test input parameters
	//
	if ( (pHandle) && (pBuffer) )
	{
		//
		// Get the internal serial handle
		//
		hSerialHandle = (int32)pHandle->handle;

		//
		// Write the whole buffer
		//
		while (numBytesLeftToWrite > 0)
		{
			//
			// Write these bytes to the serial interface
			//
			numBytesWritten = write(hSerialHandle, pCurrentBuffer, numBytesLeftToWrite);
			
			//
			// Test the there is no error
			//	
            if (numBytesWritten == -1)
            {
                 //
				// An error has occured during the write
				//
                fprintf(stderr, "sbgDeviceWrite: Unable to write to our device: %s\n", strerror(errno));
                return SBG_WRITE_ERROR;
            }
            
			//
			// Update the buffer pointer and the number of bytes to write
			//
			numBytesLeftToWrite -= numBytesWritten;
			pCurrentBuffer += numBytesWritten;
		}

		return SBG_NO_ERROR;
	}
	else
	{
		return SBG_NULL_POINTER;
	}
}

/*!
 * Try to read some data from an interface.
 * \param[in]	pHandle									Valid handle on an initialized interface.
 * \param[in]	pBuffer									Pointer on an allocated buffer that can hold at least bytesToRead bytes of data.
 * \param[out]	pReadBytes								Pointer on an uint32 used to return the number of read bytes.
 * \param[in]	bytesToRead								Number of bytes we would like to read.
 * \return												SBG_NO_ERROR if no error occurs, please check the number of received bytes.
 */
SbgErrorCode sbgInterfaceSerialRead(SbgInterface *pHandle, void *pBuffer, uint32 *pReadBytes, uint32 bytesToRead)
{
    SbgErrorCode    errorCode;
    int32           hSerialHandle;
    int32           numBytesRead;
    
	//
	// Test input parameters
	//
	if ( (pHandle) && (pBuffer) && (pReadBytes) )
	{
		//
		// Get the internal serial handle
		//
		hSerialHandle = (int32)pHandle->handle;
		
		//
        // Read our buffer
        //
        numBytesRead = read(hSerialHandle, pBuffer, bytesToRead);
        
        //
        // Check if we have read at least one byte
        //
        if (numBytesRead > 0)
        {
            errorCode = SBG_NO_ERROR;
        }
        else
        {
            errorCode = SBG_READ_ERROR;
            numBytesRead = 0;
        }
        
        //
        // If we can, returns the number of read bytes
        //
        if (pReadBytes)
        {
            *pReadBytes = numBytesRead;
        }
	}
	else
	{
		errorCode = SBG_NULL_POINTER;
	}
	
	return errorCode;
}
