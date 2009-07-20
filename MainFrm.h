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

#include "trayicon.h"

class CIEView;

class CMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

// Attributes
protected:
	CSplitterWnd m_wndSplitter;
	CTrayIcon	 m_trayIcon;		// my tray icon
	BOOL         m_bShutdown;		// OK to exit application

public:
	void RemoveMenuItem(CString strSubMenu, UINT nIDBar);

// Operations
public:

// Overrides
public:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// Implementation
public:
	virtual ~CMainFrame();
	CIEView* GetRightPane();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:  // control bar embedded members
	CStatusBar  m_wndStatusBar;
	CToolBar    m_wndToolBar;
	CReBar      m_wndReBar;
	void        SaveWindowsState(void);

// Generated message map functions
protected:
	afx_msg void OnFilePrint();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClose();
	afx_msg void OnAppOpen();
	afx_msg void OnAppQuit();
	afx_msg void OnEditFind();
	afx_msg LONG OnAppShutdown(UINT wParam, LONG lParam);
	afx_msg void OnEndSession(BOOL bEnding);
};


