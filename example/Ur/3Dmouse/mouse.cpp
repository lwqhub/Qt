#include "mouse.h"
#include "stdafx.h"
#include <tchar.h>
#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h>
#include <crtdbg.h>
#include "spwmacro.h"  /* Common macros used by SpaceWare functions. */
#include "si.h"        /* Required for any SpaceWare support within an app.*/
#include "siapp.h"     /* Required for siapp.lib symbols */
#include "virtualkeys.hpp"
#include "3DxTest32.h"

/* Global variables */
HDC          hdc;         /* Handle to Device Context used to draw on screen */
HWND         hWndMain;    /* Handle to Main Window */

SiHdl        devHdl;      /* Handle to 3D Mouse Device */
TCHAR devicename[100] = _T("");

Mouse::Mouse(QObject *parent)
{
    n1=0;
    n2=0;
    n3=0;
    n4=0;
    n5=0;
    n6=0;
}

Mouse::~Mouse()
{
}

void Mouse::run()
{
    while(1)
    {
        int  res;            /* SbInits result..if>0 it worked, if=0 it didnt work   */
        MSG            msg;      /* incoming message to be evaluated */
        BOOL           handled;  /* is message handled yet */
        SiSpwEvent     Event;    /* SpaceWare Event */
        SiGetEventData EData;    /* SpaceWare Event Data */

        /* create our apps window */
        CreateSPWindow(0, 0, 0, 0,(TCHAR*) _T(" "));

        /* update screen */
        InvalidateRect(hWndMain, NULL, FALSE);

        /* Initialize 3D mouse */
        res = SbInit();

        /* if 3D mouse was not detected then print error, close win., exit prog. */
        if (res < 1)
        {
          MessageBox(hWndMain,
            _T("Sorry - No supported 3Dconnexion device available.\n"),
            NULL, MB_OK);
          if (hWndMain != NULL)
          {
            DestroyWindow(hWndMain);    /* destroy window */
          }

          ExitProcess(1);                /* exit program */
        }

        handled = SPW_FALSE;     /* init handled */

        /* start message loop */
        while ( GetMessage( &msg, NULL, 0, 0 ) )
        {
          handled = SPW_FALSE;

          /* init Window platform specific data for a call to SiGetEvent */
          SiGetEventWinInit(&EData, msg.message, msg.wParam, msg.lParam);

          /* check whether msg was a 3D mouse event and process it */
          if (SiGetEvent (devHdl, SI_AVERAGE_EVENTS, &EData, &Event) == SI_IS_EVENT)
          {
            if (Event.type == SI_MOTION_EVENT)
            {
              SbMotionEvent(&Event,n1,n2,n3,n4,n5,n6);        /* process 3D mouse motion event */
              switch (Max(n1,n2,n3,n4,n5,n6))
              {
              case 0:
                  n1=0;n2=0;n3=0;n4=0;n5=0;n6=0;break;
              case 1:
                  if(n1>0)
                  {n1=1;n2=0;n3=0;n4=0;n5=0;n6=0;break;}
                  if(n1<0)
                  {n1=-1;n2=0;n3=0;n4=0;n5=0;n6=0;break;}
              case 2:
                  if(n2>0)
                  {n1=0;n2=1;n3=0;n4=0;n5=0;n6=0;break;}
                  if(n2<0)
                  {n1=0;n2=-1;n3=0;n4=0;n5=0;n6=0;break;}
              case 3:
                  if(n3>0)
                  {n1=0;n2=0;n3=1;n4=0;n5=0;n6=0;break;}
                  if(n3<0)
                  {n1=0;n2=0;n3=-1;n4=0;n5=0;n6=0;break;}
              case 4:
                  if(n4>0)
                  {n1=0;n2=0;n3=0;n4=1;n5=0;n6=0;break;}
                  if(n4<0)
                  {n1=0;n2=0;n3=0;n4=-1;n5=0;n6=0;break;}
              case 5:
                  if(n5>0)
                  {n1=0;n2=0;n3=0;n4=0;n5=1;n6=0;break;}
                  if(n5<0)
                  {n1=0;n2=0;n3=0;n4=0;n5=-1;n6=0;break;}
              case 6:
                  if(n6>0)
                  {n1=0;n2=0;n3=0;n4=0;n5=0;n6=1;break;}
                  if(n6<0)
                  {n1=0;n2=0;n3=0;n4=0;n5=0;n6=-1;break;}
              }
              emit UpdateSignal(n1,n2,n3,n4,n5,n6);
            }
            else if (Event.type == SI_ZERO_EVENT)
            {
              emit UpdateSignal(0,0,0,0,0,0);               /* process 3D mouse zero event */
            }
            handled = SPW_TRUE;              /* 3D mouse event handled */
          }

          /* not a 3D mouse event, let windows handle it */
          if (handled == SPW_FALSE)
          {
            TranslateMessage( &msg );
            DispatchMessage( &msg );
          }
        }
    }
}

void Mouse::ResetSlot()
{
    n1=0;
    n2=0;
    n3=0;
    n4=0;
    n5=0;
    n6=0;
    emit UpdateSignal(n1,n2,n3,n4,n5,n6);
}

int SbInit()
    {
      int res;                             /* result of SiOpen, to be returned  */
      SiOpenDataEx oData;                    /* OS Independent data to open ball  */

      /*init the SpaceWare input library */
      if (SiInitialize() == SPW_DLL_LOAD_ERROR)
      {
        MessageBox(hWndMain,_T("Error: Could not load SiAppDll dll files"),
          NULL, MB_ICONEXCLAMATION);
      }

      SiOpenWinInitEx (&oData, hWndMain);    /* init Win. platform specific data  */

      // Tell the driver we want to receive V3DCMDs instead of V3DKeys
      SiOpenWinAddHintBoolEnum(&oData, SI_HINT_USESV3DCMDS, SPW_TRUE);

      // Tell the driver we need a min driver version of 17.5.5.
      // This could be used to tell the driver that it needs to be upgraded before it can run this application correctly.
      SiOpenWinAddHintStringEnum(&oData, SI_HINT_DRIVERVERSION,(TCHAR*) L"17.5.5");

      /* open data, which will check for device type and return the device handle
      to be used by this function */
      if ((devHdl = SiOpenEx(L"3DxTest", SI_ANY_DEVICE, SI_NO_MASK, SI_EVENT, &oData)) == NULL)
      {
        SiTerminate();  /* called to shut down the SpaceWare input library */
        res = 0;        /* could not open device */
        return res;
      }
      else
      {
        SiDeviceName devName;
        SiGetDeviceName(devHdl, &devName);
        _stprintf_s(devicename,SPW_NUM_ELEMENTS_IN(devicename),_T("%S"),devName.name);

        res = 1;        /* opened device succesfully */
        return res;
      }

    }

LRESULT WINAPI HandleNTEvent ( HWND hWnd, unsigned msg, WPARAM wParam,
                                  LPARAM lParam )
    {
      PAINTSTRUCT ps;           /* used to paint the client area of a window */
      LONG addr;                /* address of our window */

      addr = GetClassLong(hWnd, 0);  /* get address */

      switch ( msg )
      {
      case WM_ACTIVATEAPP:
      case WM_KEYDOWN:
      case WM_KEYUP:
        /* user hit a key to close program */
        if (wParam == VK_ESCAPE)
        {
          SendMessage ( hWndMain, WM_CLOSE, 0, 0l );
        }
        break;

      case WM_PAINT:
      case WM_CLOSE:
        /* cleanup the object info created */

        break;

      case WM_DESTROY :
        PostQuitMessage (0);
        return (0);
      }
      return DefWindowProc ( hWnd, msg, wParam, lParam );

    }

void CreateSPWindow (int atx, int aty, int hi, int wid, TCHAR *string)
    {
      WNDCLASS  wndclass;     /* our own instance of the window class */
      HINSTANCE hInst;        /* handle to our instance */

      hInst = NULL;             /* init handle */

      /* Register display window class */
      wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
      wndclass.lpfnWndProc   = (WNDPROC)HandleNTEvent ;
      wndclass.cbClsExtra    = 8 ;
      wndclass.cbWndExtra    = 0 ;
      wndclass.hInstance     = hInst;
      wndclass.hIcon         = NULL;
      wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
      wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
      wndclass.lpszMenuName  = NULL ;
      wndclass.lpszClassName = _T("3DxTest32");

      RegisterClass (&wndclass) ;

      /* create the window */
      hWndMain = CreateWindow ( _T("3DxTest32"),           /*Window class name*/
        string,              /*Window caption*/
        WS_OVERLAPPEDWINDOW, /*Window Style*/
        atx,aty,wid,hi,
        NULL,	               /*parent window handle*/
        NULL,                /*window menu handle*/
        hInst,		         /*program instance handle*/
        NULL);               /*creation parameters*/
      /*release our window handle */
      ReleaseDC(hWndMain,hdc);

      UpdateWindow ( hWndMain );

    } /* end of CreateWindow */

void SbMotionEvent(SiSpwEvent *pEvent,long &n1,long &n2,long &n3,long &n4,long &n5,long &n6)
    {
      n1=pEvent->u.spwData.mData[SI_TX];
      n2=pEvent->u.spwData.mData[SI_TY];
      n3=pEvent->u.spwData.mData[SI_TZ];
      n4=pEvent->u.spwData.mData[SI_RX];
      n5=pEvent->u.spwData.mData[SI_RY];
      n6=pEvent->u.spwData.mData[SI_RZ];
    }

int Max(long n1,long n2 ,long n3,long n4,long n5,long n6)
{
  long n[6]={abs(n1),abs(n2),abs(n3),abs(n4),abs(n5),abs(n6)};
  long m=n[0];
  int t=1;
  for (int i=1;i<=5;i++)
  {
      if(n[i]>m)
      {
          m=n[i];
          t=i+1;
      }
  }
 if(n[t-1]>=100)
     return t;
 return 0;
}
