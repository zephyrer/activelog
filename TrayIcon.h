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

////////////////////////////////////////////////////////////////
// MSDN Magazine -- November 2002
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
// Compiles with Visual Studio 6.0 on Windows XP. Tab size=3.
//
#pragma once
#include "Subclass.h"

////////////////
// CTrayIcon manages an icon in the Windows system tray.
// The sample program TRAYTEST shows how to use it.
// 
class CTrayIcon : public CCmdTarget {
public:
	CTrayIcon(UINT uID);
	~CTrayIcon();

	// Call this to receive tray notifications
	void SetNotificationWnd(CWnd* pNotifyWnd, UINT uCbMsg);

	// SetIcon functions. To remove icon, call SetIcon(0)
	//
	BOOL SetIcon(UINT uID); // main variant you want to use
	BOOL SetIcon(HICON hicon, LPCTSTR lpTip);
	BOOL SetIcon(LPCTSTR lpResName, LPCTSTR lpTip)
		{ return SetIcon(lpResName ? 
			AfxGetApp()->LoadIcon(lpResName) : NULL, lpTip); }

	BOOL SetVersion(UINT uVersion);

	// Show balloon tip
	BOOL ShowBalloonTip(LPCTSTR szMsg, LPCTSTR szTitle,
		UINT uTimeout, DWORD dwInfoFlags=NIIF_INFO);

	// Show balloon tip: use resource ID instead of LPCSTR.
	BOOL ShowBalloonTip(UINT uID, LPCTSTR szTitle,
		UINT uTimeout, DWORD dwInfoFlags=NIIF_INFO);

	BOOL SetStandardIcon(LPCTSTR lpszIconName, LPCTSTR lpTip)
		{ return SetIcon(::LoadIcon(NULL, lpszIconName), lpTip); }

// Following is obsolete. CTrayIcon does default handling auotmatically.
//	virtual LRESULT OnTrayNotification(WPARAM uID, LPARAM lEvent);
	virtual LRESULT OnTrayNotify(WPARAM uID, LPARAM lEvent);
	virtual LRESULT OnTaskBarCreate(WPARAM wp, LPARAM lp);

protected:
	NOTIFYICONDATA m_nid;		  // struct for Shell_NotifyIcon args

	// private class used to hook tray notification and taskbarcreated
	class CTrayHook : public CSubclassWnd {
	private:
		CTrayIcon* m_pTrayIcon;
		virtual LRESULT WindowProc(UINT msg, WPARAM wp, LPARAM lp);
		friend CTrayIcon;
	};
	friend CTrayHook;
	CTrayHook m_notifyHook; // trap tray notifications
	CTrayHook m_parentHook; // trap taskbarcreated message
	DECLARE_DYNAMIC(CTrayIcon)
};
