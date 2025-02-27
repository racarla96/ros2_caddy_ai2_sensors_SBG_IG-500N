/*!
 *	\file		sbgEComLib.h
 *  \author		SBG Systems (Raphael Siryani)
 *	\date		05 February 2013
 *
 *	\brief		Main header file for the SBG Systems Enhanced Communication Library.
 *
 *	Only this main header file should be included to use the library.
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

/*!
 *	\mainpage SBG Systems Enhanced Communication library documentation
 *	Welcome to the sbgECom library documentation.<br>
 *	This documentation describes all functions implemented in the sbgECom library.
 */

#ifndef __SBG_ECOM_LIB_H__
#define __SBG_ECOM_LIB_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "sbgCommon.h"
#include "sbgEComCmds.h"
#include "interfaces/interface.h"
#include "interfaces/interfaceUdp.h"
#include "interfaces/interfaceSerial.h"
#include "interfaces/interfaceFile.h"
#include "commands/commands.h"
#include "protocol/protocol.h"
#include "binaryLogs/binaryLogs.h"
#include "time/sbgTime.h"
#include "misc/sbgCrc.h"

//----------------------------------------------------------------------//
//- Footer (close extern C block)                                      -//
//----------------------------------------------------------------------//
#ifdef __cplusplus
}
#endif

#endif	/* __SBG_ECOM_LIB_H__ */
