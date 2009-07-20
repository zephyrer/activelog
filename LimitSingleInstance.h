/*******************************************************************
		
 ActiveLog is a utility developed by VATech Systems 
 Copyright (C) 2006 - 2009 VATech Systems.

 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.

 You can contact VATech Systems at email address info@vatechsystems.com.

***********************************************************************/

#pragma once

const TCHAR CLASS_SOFT[] = _T("VAALog_ActiveLog_Class");

class CLimitSingleInstance
{
protected:
	BOOL m_bClassRegistered;
	DWORD  m_dwLastError;
	HANDLE m_hMutex;
  

public:
	CLimitSingleInstance(TCHAR *strMutexName) : m_bClassRegistered(FALSE)
	{
		//Make sure that you use a name that is unique for this application otherwise
		//two apps may think they are the same if they are using same name for
		//3rd parm to CreateMutex
		m_hMutex = CreateMutex(NULL, FALSE, strMutexName); //do early
		m_dwLastError = GetLastError(); //save for use later...
	}
   
	~CLimitSingleInstance() 
	{
		if (m_hMutex)  //Do not forget to close handles.
		{
			CloseHandle(m_hMutex); //Do as late as possible.
			m_hMutex = NULL; //Good habit to be in.
		}
		if ( m_bClassRegistered ){
			::UnregisterClass(CLASS_SOFT, AfxGetInstanceHandle());
		}
	}

	//
	//	http://support.microsoft.com/kb/243953
	//	How to limit 32-bit applications to one instance in Visual C++
	//	Q243953
	//
	//  Based on Q261997
	//  HOWTO: Obtain the Window Handle for an Office Automation Server with MFC
	//
	//  And Q238100
	//	HOWTO: Limit 32-Bit MFC SDI Applications to a Single Instance on WinCE 
	//
	BOOL IsAnotherInstanceRunning() 
	{
		if (ERROR_ALREADY_EXISTS == m_dwLastError)
		{
			HWND hWnd = ::FindWindow(CLASS_SOFT, NULL);
			if ( hWnd )
			{
				//CWnd* pWnd = CWnd::FromHandle(hWnd);
				::ShowWindow (hWnd, SW_SHOWNORMAL);
				// This method is not yet available for VAALog app
				// You can make the Office application visible using:
				// app.SetVisible(TRUE);
			}
			return TRUE;
		}
		return FALSE;
	}

	BOOL RegisterInstance() 
	{
		WNDCLASS wndclass;

		memset(&wndclass, 0, sizeof(WNDCLASS));		// start with NULL
													// defaults
		//wndclass.cbSize = sizeof(wndclass);
		wndclass.style         = CS_DBLCLKS | CS_HREDRAW | CS_VREDRAW;
		wndclass.lpfnWndProc   = ::DefWindowProc;
		wndclass.hInstance     = AfxGetInstanceHandle();
		wndclass.hIcon         = AfxGetApp()->LoadIcon(IDR_MAINFRAME); 
		wndclass.hCursor       = AfxGetApp()->LoadCursor(IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOWFRAME);
		wndclass.lpszMenuName  = NULL;

		// Specify your own class name for using FindWindow later
		wndclass.lpszClassName = CLASS_SOFT;

		// line below has to be inserted into
		// BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
		//     cs.lpszClass = CLASS_SOFT;

		m_bClassRegistered = AfxRegisterClass(&wndclass);

		return m_bClassRegistered;
	}
};
