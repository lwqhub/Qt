/*----------------------------------------------------------------------
*  sbtest32.h  -- Basic Program to init 3D mouse, read its data and 
*                  print it out.
*
*  Originally written by Elio Querze
*  
*
*----------------------------------------------------------------------
*
* Copyright notice:
* Copyright (c) 1998-2015 3Dconnexion. All rights reserved. 
* 
* This file and source code are an integral part of the "3Dconnexion
* Software Developer Kit", including all accompanying documentation,
* and is protected by intellectual property laws. All use of the
* 3Dconnexion Software Developer Kit is subject to the License
* Agreement found in the "LicenseAgreementSDK.txt" file.
* All rights not expressly granted by 3Dconnexion are reserved.
*
*/
#pragma comment  (lib,"User32.lib")
#pragma comment  (lib,"Gdi32.lib")/*该语句添加至头文件*/
static char SbTestCvsId[]="(C) 1998-2015 3Dconnexion: $Id: 3DxTEST32.H 13020 2016-05-25 09:51:06Z jwick $";
/* Global Variables */

#ifndef SBTEST32_H
#define SBTEST32_H

/* Functions --See cpp file for additional details */

void  CreateSPWindow(int, int, int, int, TCHAR *);
int   SbInit();
void  SbMotionEvent(SiSpwEvent *pEvent,long &n1,long &n2,long &n3,long &n4,long &n5,long &n6);
void  SbZeroEvent();
void  SbButtonPressEvent(int buttonnumber);
void  SbButtonReleaseEvent(int buttonnumber);
void  HandleDeviceChangeEvent(SiSpwEvent *pEvent);
void  HandleV3DCMDEvent(SiSpwEvent *pEvent);
void  HandleAppEvent(SiSpwEvent *pEvent);
int   Max(long,long,long,long,long,long);
#endif
