/*!
 *	\file		protocol.h
 *  \author		SBG-Systems (Raphael Siryani)
 *	\date		06/02/13
 *
 *	\brief		Implementation of the Ekinox binary communication protocol.<br>
 *				You can access low-level communication with the device.<br>
 *				Copyright 2007-20011 SBG Systems. All rights reserved.
 *
 *	You will find below, the frame definition used by Ekinox devices.<br>
 *	<br>
 *	<table>
 *	<caption>Frame structure</caption>
 *		<tr align="center"><td>Fields</td>			<td>SYNC 1</td>	<td>SYNC 2</td>	<td>CMD</td>	<td>LEN</td>	<td>DATA</td>		<td>CRC</td>	<td>ETX</td></tr>
 *		<tr align="center"><td>Size in bytes</td>	<td>1</td>		<td>1</td>		<td>2</td>		<td>2</td>		<td>(0-4086)</td>	<td>2</td>		<td>1</td></tr>
 *		<tr align="center"><td>Value</td>			<td>0xFF</td>	<td>0x5A</td>	<td>?</td>		<td>?</td>		<td>?</td>			<td>?</td>		<td>0x33</td></tr>
 *	</table>
 *	<br>
 *	Size in bytes indicates the size of the data field.<br>
 *	The minimum frame size is 9 bytes and the maximum is 512 bytes.<br>
 *	<br>
 *	The CRC is calculated on the whole frame without:<br>
 *	SYNC STX CRC and ETX fields.<br>
 */
#ifndef __PROTOCOL_H__
#define __PROTOCOL_H__

#include "../sbgCommon.h"
#include "../interfaces/interface.h"

//----------------------------------------------------------------------//
//- Global definitions                                                 -//
//----------------------------------------------------------------------//
#define SBG_ECOM_MAX_BUFFER_SIZE				(4096)							/*!< Maximum reception buffer size in bytes. */
#define SBG_ECOM_MAX_PAYLOAD_SIZE				(4086)							/*!< Maximum payload size in bytes. */
#define SBG_ECOM_SYNC_1							(0xFF)							/*!< First synchronization char of the frame. */
#define SBG_ECOM_SYNC_2							(0x5A)							/*!< Second synchronization char of the frame. */
#define SBG_ECOM_ETX							(0x33)							/*!< End of frame byte. */

#define SBG_ECOM_RX_TIME_OUT					(450)							/*!< Default time out for new frame reception. */

//----------------------------------------------------------------------//
//- Communication protocol structs and definitions                     -//
//----------------------------------------------------------------------//

/*!
 *	Struct containing all protocol related data.
 */
typedef struct _SbgEComProtocol
{
	SbgInterface		*pLinkedInterface;							/*!< Associated interface used by the protocol to read/write bytes. */
	uint8				 rxBuffer[SBG_ECOM_MAX_BUFFER_SIZE];		/*!< The reception buffer. */
	uint32				 rxBufferSize;								/*!< The current reception buffer size in bytes. */
} SbgEComProtocol;

//----------------------------------------------------------------------//
//- Communication protocol operations                                  -//
//----------------------------------------------------------------------//

/*!
 *	Initialize the protocol system used to communicate with the product and return the created handle.
 *	\param[in]	pHandle					Pointer on an allocated protocol structure to initialize.
 *	\param[in]	pInterface				Interface to use for read/write operations.
 *	\return								SBG_NO_ERROR if we have initialised the protocol system.
 */
SbgErrorCode sbgEComProtocolInit(SbgEComProtocol *pHandle, SbgInterface *pInterface);

/*!
 *	Close the protocol system.
 *	\param[in]	pHandle					A valid protocol handle to close.
 *	\return								SBG_NO_ERROR if we have closed and released the protocol system.
 */
SbgErrorCode sbgEComProtocolClose(SbgEComProtocol *pHandle);

/*!
 *	Send a frame to the device (size should be less than 4086 bytes).
 *	\param[in]	pHandle					A valid protocol handle.
 *	\param[in]	cmd						Command number to send (0 to 65535).
 *	\param[in]	pData					Pointer on the data payload to send or NULL if no payload.
 *	\param[in]	size					Size in bytes of the data payload (less than 4086).
 *	\return								SBG_NO_ERROR if the frame has been sent.
 */
SbgErrorCode sbgEComProtocolSend(SbgEComProtocol *pHandle, uint16 cmd, const void *pData, uint32 size);

/*!
 *	Try to receive a frame from the device and returns the cmd, data and size of data field.
 *	\param[in]	pHandle					A valid protocol handle.
 *	\param[out]	pCmd					Pointer to hold the returned command.
 *	\param[out]	pData					Allocated buffer used to hold received data field.
 *	\param[out]	pSize					Pointer used to hold the received data field size.
 *	\param[in]	maxSize					Max number of bytes that can be stored in the pData buffer.
 *	\return								SBG_NO_ERROR if we have received a valid frame.<br>
 *										SBG_NOT_READY if we haven't received a valid frame or if the serial buffer is empty.<br>
 *										SBG_INVALID_CRC if the received frame has an invalid CRC.<br>
 *										SBG_NULL_POINTER if an input parameter is NULL.<br>
 *										SBG_BUFFER_OVERFLOW if the received frame payload couldn't fit into the pData buffer.
 */
SbgErrorCode sbgEComProtocolReceive(SbgEComProtocol *pHandle, uint16 *pCmd, void *pData, uint32 *pSize, uint32 maxSize);

#endif
