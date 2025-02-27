/*!
 *	\file		interface.h
 *  \author		SBG Systems (Raphael Siryani)
 *	\date		10 December 2012
 *
 *	\brief		This file implements the base interface for all Serial and Ethernet ports.
 *
 *	An interface is used to provide a common API for both serial and ethernet ports.
 *	An interface can be opened/closed and some data can be written or read from it.
 *
 *	\section CodeCopyright Copyright Notice 
 *	Copyright (C) 2007-2013, SBG Systems SAS. All rights reserved.
 *	
 *	This source code is intended for use only by SBG Systems SAS and
 *	those that have explicit written permission to use it from
 *	SBG Systems SAS.
 *	
 *	THIS CODE AND INFORMATION IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 *	KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 *	IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 *	PARTICULAR PURPOSE.
 */

#ifndef __INTERFACE_H__
#define __INTERFACE_H__

#include "../sbgCommon.h"

//----------------------------------------------------------------------//
//- Structure definitions                                              -//
//----------------------------------------------------------------------//

/*!
 * Define the interface type.
 */
typedef enum _SbgInterfaceType
{
	SBG_IF_TYPE_UNKNOW,								/*!< The interface type is not defined. */
	SBG_IF_TYPE_SERIAL,								/*!< The interface is a serial com port. */
	SBG_IF_TYPE_ETH_UDP,							/*!< The interface is an UDP one. */
	SBG_IF_TYPE_ETH_TCP_IP,							/*!< The interface is an TCP/IP one. */
	SBG_IF_TYPE_FILE								/*!< The interface is a file. */
} SbgInterfaceType;

//----------------------------------------------------------------------//
//- Predefinitions                                                     -//
//----------------------------------------------------------------------//

/*!
 * Interface structure pre-definition.
 */
typedef struct _SbgInterface SbgInterface;

/*!
 * Handle that stores the internal interface handle (ie Serial or Ethernet)
 */
typedef void* SbgInterfaceHandle;

//----------------------------------------------------------------------//
//- Callbacks definitions                                              -//
//----------------------------------------------------------------------//

/*!
 * Try to write some data to an interface.
 * \param[in]	pHandle									Valid handle on an initialized interface.
 * \param[in]	pBuffer									Pointer on an allocated buffer that contains the data to write
 * \param[in]	bytesToWrite							Number of bytes we would like to write.
 * \return												SBG_NO_ERROR if some bytes have been written successfully.
 */
typedef SbgErrorCode (*SbgInterfaceWriteFunc)(SbgInterface *pHandle, const void *pBuffer, uint32 bytesToWrite);

/*!
 * Try to read some data from an interface.
 * \param[in]	pHandle									Valid handle on an initialized interface.
 * \param[in]	pBuffer									Pointer on an allocated buffer that can hold at least bytesToRead bytes of data.
 * \param[out]	pReadBytes								Pointer on an uint32 used to return the number of read bytes.
 * \param[in]	bytesToRead								Number of bytes we would like to read.
 * \return												SBG_NO_ERROR if some bytes have been read successfully.
 */
typedef SbgErrorCode (*SbgInterfaceReadFunc)(SbgInterface *pHandle, void *pBuffer, uint32 *pReadBytes, uint32 bytesToRead);

/*!
 * Returns true if the interface is working correctly.
 * \param[in]	pHandle									Valid handle on an initialized interface.
 * \return												TRUE if this interface is working correctly.
 */
typedef bool (*SbgInterfaceIsValidFunc)(SbgInterface *pHandle);

//----------------------------------------------------------------------//
//- Structures definitions                                             -//
//----------------------------------------------------------------------//

/*!
 * Interface definition that stores methods used to communicate on the interface.
 */
struct _SbgInterface
{
	SbgInterfaceHandle			 handle;							/*!< Internal interface handle used to access the media. */
	SbgInterfaceType			 type;								/*!< The interface type. */

	SbgInterfaceWriteFunc		pWriteFunc;							/*!< Pointer on the method used to write some data to this interface. */
	SbgInterfaceReadFunc		pReadFunc;							/*!< Pointer on the method used to read some data to this interface. */
	SbgInterfaceIsValidFunc		pIsValidFunc;						/*!< Pointer on the method used to test if the interface is working correctly.*/
};

//----------------------------------------------------------------------//
//- Inline operations methods                                          -//
//----------------------------------------------------------------------//

/*!
 *	Initialize an interface strcture to zero.
 *	\param[in]	pHandle									Handle on an allocated interface to initialize to zero.
 */
SBG_INLINE void sbgInterfaceZeroInit(SbgInterface *pHandle)
{
	//
	// Initialize all fields to NULL
	//
	pHandle->handle			= NULL;
	pHandle->type			= SBG_IF_TYPE_UNKNOW;
	pHandle->pWriteFunc		= NULL;
	pHandle->pReadFunc		= NULL;
	pHandle->pIsValidFunc	= NULL;
}

/*!
 * Returns true if this interface seems to be up and running.
 * \param[in]	pHandle									Handle on an interface to test (if pHandle == NULL, retruns FALSE).
 * \return												TRUE if this interface seems to be up and running.
 */
SBG_INLINE bool sbgInterfaceIsValid(SbgInterface *pHandle)
{
	//
	// Call the correct valid method according to the interface
	//
	return pHandle->pIsValidFunc(pHandle);
}

/*!
 * Try to write some data to an interface.
 * \param[in]	pHandle									Valid handle on an initialized interface.
 * \param[in]	pBuffer									Pointer on an allocated buffer that contains the data to write
 * \param[in]	bytesToWrite							Number of bytes we would like to write.
 * \return												SBG_NO_ERROR if all bytes have been written successfully.
 */
SBG_INLINE SbgErrorCode sbgInterfaceWrite(SbgInterface *pHandle, const void *pBuffer, uint32 bytesToWrite)
{
	//
	// Call the correct write method according to the interface
	//
	return pHandle->pWriteFunc(pHandle, pBuffer, bytesToWrite);
}

/*!
 * Try to read some data from an interface.
 * \param[in]	pHandle									Valid handle on an initialized interface.
 * \param[in]	pBuffer									Pointer on an allocated buffer that can hold at least bytesToRead bytes of data.
 * \param[out]	pReadBytes								Pointer on an uint32 used to return the number of read bytes.
 * \param[in]	bytesToRead								Number of bytes we would like to read.
 * \return												SBG_NO_ERROR if no error occurs, please check the number of received bytes.
 */
SBG_INLINE SbgErrorCode sbgInterfaceRead(SbgInterface *pHandle, void *pBuffer, uint32 *pReadBytes, uint32 bytesToRead)
{
	//
	// Call the correct read method according to the interface
	//
	return pHandle->pReadFunc(pHandle, pBuffer, pReadBytes, bytesToRead);
}


#endif /* __INTERFACE_H__ */
