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
#include "stdafx.h"
#include "trayicon.h"
#include <afxpriv.h>		// for AfxLoadString

#define countof(x)	(sizeof(x)/sizeof(x[0]))

//////////////////
// Windows sends this message when the taskbar is created. This can happen
// if it crashes and Windows has to restart it. CTrayIcon responds by
// re-installing its icon.
//
const UINT WM_TASKBARCREATED = ::RegisterWindowMessage(_T("TaskbarCreated"));

IMPLEMENT_DYNAMIC(CTrayIcon, CCmdTarget)

CTrayIcon::CTrayIcon(UINT uID)
{
	// Initialize NOTIFYICONDATA
	memset(&m_nid, 0 , sizeof(m_nid));
	m_nid.cbSize = sizeof(m_nid);
	m_nid.uID = uID;	// never changes after construction

	m_notifyHook.m_pTrayIcon = this; // notification window hook
	m_parentHook.m_pTrayIcon = this;	// parent window hook

	// Use resource string as tip if there is one
	// Buffer overrun due to number of characters/number of bytes mismatch in call to 'AfxLoadString'
	// Buffer overrun: accessing 'argument 2', the writable size is '256' bytes, but '512' bytes might be written: Lines: 23, 25, 26, 27, 29, 30, 33
	AfxLoadString(uID, m_nid.szTip, sizeof(m_nid.szTip));
}

CTrayIcon::~CTrayIcon()
{
	SetIcon(0); // remove icon from system tray
}

BOOL CTrayIcon::SetVersion(UINT uVersion)
{
	m_nid.uVersion = uVersion;
	return Shell_NotifyIcon(NIM_SETVERSION, &m_nid);
}

//////////////////
// Set notification window. It must be created already.
//
void CTrayIcon::SetNotificationWnd(CWnd* pNotifyWnd, UINT uCbMsg)
{
	// If the following assert fails, you're probably
	// calling me before you created your window. Oops.
	// warning C6011: Dereferencing NULL pointer 'pNotifyWnd': Lines: 54, 55
	ASSERT(pNotifyWnd==NULL || ::IsWindow(pNotifyWnd->GetSafeHwnd()));
	m_nid.hWnd = pNotifyWnd->GetSafeHwnd();

	ASSERT(uCbMsg==0 || uCbMsg>=WM_USER);
	m_nid.uCallbackMessage = uCbMsg;

	CWnd* pParentWnd = pNotifyWnd ? pNotifyWnd->GetTopLevelParent() : NULL;

	// Install window hooks. Must be different because
	// taskbar creation message only goes to top-level parent.
	m_notifyHook.HookWindow(pNotifyWnd);
	if (pParentWnd!=pNotifyWnd)
		m_parentHook.HookWindow(pParentWnd);
}

//////////////////
// This is the main variant for setting the icon.
// Sets both the icon and tooltip from resource ID
// To remove the icon, call SetIcon(0)
//
BOOL CTrayIcon::SetIcon(UINT uID)
{ 
	HICON hicon=NULL;
	if (uID) {
		// warning C6057: Buffer overrun due to number of characters/number of bytes mismatch in call to 'AfxLoadString'
		// warning C6386: Buffer overrun: accessing 'argument 2', the writable size is '256' bytes, but '512' bytes might be written: Lines: 76, 77, 78
		AfxLoadString(uID, m_nid.szTip, sizeof(m_nid.szTip));
		hicon = AfxGetApp()->LoadIcon(uID);
	}
	return SetIcon(hicon, NULL);
}

//////////////////
// Common SetIcon for all overloads. 
//
BOOL CTrayIcon::SetIcon(HICON hicon, LPCTSTR lpTip) 
{
	UINT msg;
	m_nid.uFlags = 0;

	// Set the icon
	if (hicon) {
		// Add or replace icon in system tray
		msg = m_nid.hIcon ? NIM_MODIFY : NIM_ADD;
		m_nid.hIcon = hicon;
		m_nid.uFlags |= NIF_ICON;
	} else { // remove icon from tray
		if (m_nid.hIcon==NULL)
			return TRUE;		// already deleted
		msg = NIM_DELETE;
	}

	// Use the tip, if any
	if (lpTip)
		_tcsncpy_s(m_nid.szTip, countof(m_nid.szTip), lpTip, countof(m_nid.szTip));
	if (m_nid.szTip[0])
		m_nid.uFlags |= NIF_TIP;

	// Use callback if any
	if (m_nid.uCallbackMessage && m_nid.hWnd)
		m_nid.uFlags |= NIF_MESSAGE;

	// Do it
	BOOL bRet = Shell_NotifyIcon(msg, &m_nid);
	if (msg==NIM_DELETE || !bRet)
		m_nid.hIcon = NULL;	// failed
	return bRet;
}

//////////////////
// Show balloon tip: args give message, timeout, etc.
//
BOOL CTrayIcon::ShowBalloonTip(LPCTSTR szMsg, LPCTSTR szTitle,
	UINT uTimeout, DWORD dwInfoFlags)
{ 
	m_nid.cbSize=sizeof(NOTIFYICONDATA);
	m_nid.uFlags = NIF_INFO;
	m_nid.uTimeout = uTimeout;
	m_nid.dwInfoFlags = dwInfoFlags;
	lstrcpy(m_nid.szInfo, szMsg ? szMsg : _T(""));
	lstrcpy(m_nid.szInfoTitle, szTitle ? szTitle : _T(""));
	return Shell_NotifyIcon(NIM_MODIFY, &m_nid);
}

BOOL CTrayIcon::ShowBalloonTip(UINT uID, LPCTSTR szTitle,
	UINT uTimeout, DWORD dwInfoFlags)
{
	CString s;
	return s.LoadString(uID) ?
		ShowBalloonTip(s, szTitle, uTimeout, dwInfoFlags) : FALSE;
}

//////////////////
// Same hook class used for both notification window and top-level
// parent; hook function determines which.
//
LRESULT CTrayIcon::CTrayHook::WindowProc(UINT msg, WPARAM wp, LPARAM lp)
{
	if (msg==m_pTrayIcon->m_nid.uCallbackMessage &&
		 m_hWnd==m_pTrayIcon->m_nid.hWnd) {

		m_pTrayIcon->OnTrayNotify(wp, lp);

//	} else if (msg==WM_DESTROY) {
//		m_pTrayIcon->SetIcon(NULL);

	} else if (msg==WM_TASKBARCREATED) {
		m_pTrayIcon->OnTaskBarCreate(wp, lp);
	}
	return CSubclassWnd::WindowProc(msg, wp, lp);
}

/////////////////
// Default event handler handles right-menu and doubleclick.
// Override to do something different.
//
LRESULT CTrayIcon::OnTrayNotify(WPARAM wID, LPARAM lEvent)
{
	if (wID!=m_nid.uID ||
		(lEvent!=WM_RBUTTONUP && lEvent!=WM_LBUTTONDBLCLK))
		return 0;

	// If there's a resource menu with the same ID as the icon, use it as 
	// the right-button popup menu. CTrayIcon will interprets the first
	// item in the menu as the default command for WM_LBUTTONDBLCLK
	// 
	CMenu menu;
	if (!menu.LoadMenu(m_nid.uID))
		return 0;
	CMenu* pSubMenu = menu.GetSubMenu(0);
	if (!pSubMenu) 
		return 0;

	if (lEvent==WM_RBUTTONUP) {

		// Make first menu item the default (bold font)
		::SetMenuDefaultItem(pSubMenu->m_hMenu, 0, TRUE);

		// Display the menu at the current mouse location. There's a "bug"
		// (Microsoft calls it a feature) in Windows 95 that requires calling
		// SetForegroundWindow. To find out more, search for Q135788 in MSDN.
		//
		CPoint mouse;
		GetCursorPos(&mouse);
		::SetForegroundWindow(m_nid.hWnd);	
		::TrackPopupMenu(pSubMenu->m_hMenu, 0, mouse.x, mouse.y, 0,
			m_nid.hWnd, NULL);
		::PostMessage(m_nid.hWnd, WM_NULL, 0, 0);

	} else  // double click: execute first menu item
		::SendMessage(m_nid.hWnd, WM_COMMAND, pSubMenu->GetMenuItemID(0), 0);

	return 1; // handled
}

//////////////////
// Explorer had to restart the taskbar: add icons again
//
LRESULT CTrayIcon::OnTaskBarCreate(WPARAM, LPARAM)
{
	// Reinstall taskbar icon
	HICON hIcon = m_nid.hIcon;
	m_nid.hIcon = NULL;
	if (hIcon)
		SetIcon(hIcon, NULL); // will reuse current tip
	return 0;
}
