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


// ActiveLog command target

class ActiveLog : public CCmdTarget
{
	DECLARE_DYNCREATE(ActiveLog)

public:
	ActiveLog();
	virtual ~ActiveLog();

	virtual void OnFinalRelease();
	int ReleaseExternal(void);
	int LockExternal(void);

	DWORD GetRefCount();
	void RegisterAsActive();
	BOOL m_bLockedExternal;
	DWORD m_dwCookie;

protected:
	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(ActiveLog) //declares a COleObjectFactory (MFC's support for class factories) object and a guid for the CServerDialog class
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()

	SHORT LogData(LPCTSTR xData, LPCTSTR xLabel);

	enum 
	{
		dispidLogData = 1L
	};
};


