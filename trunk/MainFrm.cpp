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

#include "stdafx.h"
#include "VAALOG.h"

#include "MainFrm.h"
#include "LeftView.h"
#include "IEView.h"
#include "LimitSingleInstance.h"
//#include "ALDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_MY_TRAY_NOTIFICATION WM_USER+0

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_COMMAND(ID_FILE_PRINT, &CMainFrame::OnFilePrint)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_APP_OPEN, &CMainFrame::OnAppOpen)
	ON_COMMAND(ID_APP_QUIT, &CMainFrame::OnAppQuit)
	ON_COMMAND(ID_EDIT_FIND, &CMainFrame::OnEditFind)
	ON_REGISTERED_MESSAGE(CALApp::m_nShutdownMsg, &CMainFrame::OnAppShutdown)
	ON_WM_ENDSESSION()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame construction/destruction

CMainFrame::CMainFrame() : m_trayIcon(IDR_TRAYICON)
{
	// TODO: add member initialization code here
	m_bShutdown = FALSE;
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT | TBSTYLE_TRANSPARENT) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	// TODO: Remove this if you don't want tool tips
	m_wndToolBar.SetBarStyle(m_wndToolBar.GetBarStyle() |
		CBRS_TOOLTIPS | CBRS_FLYBY);

	// Set up tray icon
	m_trayIcon.SetNotificationWnd(this, WM_MY_TRAY_NOTIFICATION);
	//m_trayIcon.SetIcon(IDR_MAINFRAME, _T("Double-click to activate ActiveLog"));
	m_trayIcon.SetIcon(IDR_TRAYICON);

	return 0;
}

BOOL CMainFrame::OnCreateClient(LPCREATESTRUCT /*lpcs*/,
	CCreateContext* pContext)
{
	// create splitter window
	if (!m_wndSplitter.CreateStatic(this, 1, 2))
		return FALSE;

	// use saved splitter position
	int splitPos = AfxGetApp()->GetProfileInt(_T("Settings"),_T("SplitterPos"), 100);
	
	// validate saved value with margin 20
	CRect rect;
	GetClientRect(&rect);
	if( (splitPos <= 20) || ( splitPos >= rect.Width() - 20 ))
	{
		splitPos = 100; // default value
		if ( rect.Width() > splitPos ) splitPos = rect.Width() / 2;
	}

	if (!m_wndSplitter.CreateView(0, 0, RUNTIME_CLASS(CLeftView), CSize(splitPos, 100), pContext) ||
		!m_wndSplitter.CreateView(0, 1, RUNTIME_CLASS(CIEView), CSize(100, 100), pContext))
	{
		m_wndSplitter.DestroyWindow();
		return FALSE;
	}

	CIEView* pIEView = STATIC_DOWNCAST(CIEView, m_wndSplitter.GetPane(0,1));
	ASSERT(pIEView);
	CLeftView* pLeftView = STATIC_DOWNCAST(CLeftView, m_wndSplitter.GetPane(0,0));
	ASSERT(pLeftView);

	// Connect views to each other
	pIEView->SetListView(pLeftView);
	pLeftView->SetIEView(pIEView);

	pLeftView->RestoreHeaderState( AfxGetApp()->GetProfileString(_T("Settings"),_T("HeaderList")) );

	// Activate the input view. This is important for it to receive commands.
	// SetActiveView(pIEView);

	return TRUE;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	
	// Changing the Styles of a Window Created by MFC
	// http://msdn2.microsoft.com/en-gb/library/x07xt611(VS.80).aspx
    cs.style &= ~FWS_ADDTOTITLE;

	// this is required for FindWindow
	cs.lpszClass = CLASS_SOFT;

	// clone of MTC2 code
	WINDOWPLACEMENT wp;
	if (ReadWindowPlacement(&wp))
	{
		if( (wp.flags & WPF_RESTORETOMAXIMIZED) != WPF_RESTORETOMAXIMIZED )
		{
			CRect rect = wp.rcNormalPosition;

			if (rect.Width() > 0 && rect.Height() > 0)
			{
				// make sure window will be visible
				CDC dc; 
				dc.CreateIC(_T("DISPLAY"), NULL, NULL, NULL);
				CRect rectDisplay(0, 0, dc.GetDeviceCaps(HORZRES),
					dc.GetDeviceCaps(VERTRES));

				if (rectDisplay.PtInRect(rect.TopLeft()) &&
					rectDisplay.PtInRect(rect.BottomRight()))
				{
					cs.x = rect.left;
					cs.y = rect.top;
					cs.cx = rect.Width();
					cs.cy = rect.Height();
				}
			}
		}
	}

	return TRUE;
}


// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

// CMainFrame message handlers

//
// check for more code with splitterwnd
// http://www.microsoft.com/msj/0998/c0998.aspx
// http://www.microsoft.com/msj/0795/dilascia/dilascia.aspx
// http://www.microsoft.com/msj/0698/c0698.aspx
//
void CMainFrame::OnFilePrint() 
{
	CIEView* pView = STATIC_DOWNCAST(CIEView, m_wndSplitter.GetPane(0,1));
	ASSERT(pView);
	if (pView != NULL) pView->OnFilePrint();
}

void CMainFrame::RemoveMenuItem(CString strSubMenu, UINT nIDBar)
{
	// The return value is a handle to the menu. If the specified window has no menu, 
	// the return value is NULL. If the window is a child window, the return value is undefined. - ???
	CMenu* pMenu = GetMenu();
	if ( ! pMenu ) return;

	if(!::IsMenu(pMenu->GetSafeHmenu())) return;

	UINT nIndex = (UINT)-1;
	UINT nCount = pMenu->GetMenuItemCount();
	for (UINT i = 0; i < nCount; i++)
	{
		CString strThisMenu;
		if (pMenu->GetMenuString(i, strThisMenu, MF_BYPOSITION) &&
		 ( strSubMenu.CompareNoCase( strThisMenu ) == 0))
		{
			nIndex = i;
			break;
		}
	}

	if ( nIndex == (UINT)-1 ) return;

	CMenu* pSubMenu = pMenu->GetSubMenu(nIndex);
	if(!::IsMenu(pMenu->GetSafeHmenu())) return;

    nIndex = (UINT)-1;
	nCount = pSubMenu->GetMenuItemCount();
	for(UINT i=0; i < nCount; i++)
	{
		if ( pSubMenu->GetMenuItemID(i) == nIDBar )
		{
			nIndex = i;
			break;
		}
	}

	if ( nIndex == (UINT)-1 ) return;

	UINT nStateAbove = pSubMenu->GetMenuState(nIndex-1, MF_BYPOSITION);
	UINT nStateBelow = pSubMenu->GetMenuState(nIndex+1, MF_BYPOSITION);

	pSubMenu->RemoveMenu(nIndex, MF_BYPOSITION);
	if ( nStateAbove & nStateBelow & MF_SEPARATOR )
	{
		if ( nStateAbove != (UINT)-1 )
			pSubMenu->RemoveMenu(nIndex-1, MF_BYPOSITION);
		else if ( nStateBelow != (UINT)-1 )
			pSubMenu->RemoveMenu(nIndex, MF_BYPOSITION);
	}
}

void CMainFrame::OnClose()
{
	if (m_bShutdown)
	{
		//CALDoc* pDoc = (CALDoc*)GetActiveDocument();
		int iUserChice = 0;

		AFX_MODULE_STATE* pModuleState = AfxGetModuleState();
		if (pModuleState->m_nObjectCount > 1 )
		{
			// code extract from vc6 tracer sample
			CString strMsg;
			// warning C6031: Return value ignored: 'ATL::CStringT<wchar_t,StrTraitMFC<wchar_t,ATL::ChTraitsCRT<wchar_t> > >::LoadStringW'
			strMsg.LoadString(IDS_APP_IS_LOCKED);
			iUserChice = AfxMessageBox(strMsg, MB_OKCANCEL);
			switch (iUserChice)
			{
				case IDOK:
					// prompt registration dialog
					// ???
					break;
				case IDCANCEL:
					return;
			}
		}

		SaveWindowsState();

		CFrameWnd::OnClose();

		// copy code from void CFrameWnd::OnClose()
		if (AfxGetApp()->m_pMainWnd == NULL && iUserChice == IDOK)
		{
			AfxPostQuitMessage(0);
			return;
		}
	}
	else
	{
		ShowWindow(SW_HIDE);
	}
}

void CMainFrame::OnAppOpen()
{
	ShowWindow(SW_NORMAL);	
	SetForegroundWindow();
}

void CMainFrame::OnAppQuit()
{
	m_bShutdown = TRUE;		// really exit
	SendMessage(WM_CLOSE);	
}

void CMainFrame::OnEditFind()
{
	// TODO: Add your command handler code here
	CIEView* pView = STATIC_DOWNCAST(CIEView, m_wndSplitter.GetPane(0,1));
	ASSERT(pView);
	if (pView != NULL) pView->OnEditFind();
}

LONG CMainFrame::OnAppShutdown(UINT wParam, LONG lParam)
{
	OnAppQuit();
	return FALSE;
}

// if windows is shut down, a running application doesn't recieve
// a WM_CLOSE message, but it does recieve a WM_ENDSESSION message
void CMainFrame::OnEndSession(BOOL bEnding)
{
	if(bEnding)
		SaveWindowsState();

	CFrameWnd::OnEndSession(bEnding);
	// TODO: Add your message handler code here
}

void CMainFrame::SaveWindowsState(void)
{
	// before it is destroyed, save the position of the window
	WINDOWPLACEMENT wp;
	wp.length = sizeof wp;
	if (GetWindowPlacement(&wp))
	{
		wp.flags = 0;
		if (IsZoomed())
			wp.flags |= WPF_RESTORETOMAXIMIZED;
		// and write it to the .INI file
		WriteWindowPlacement(&wp);
	}

	CLeftView* pView = STATIC_DOWNCAST(CLeftView, m_wndSplitter.GetPane(0,0));
	ASSERT(pView);
	if (pView != NULL)
	{
		// save splitter position
		CRect rect;
		pView->GetClientRect(&rect);
		AfxGetApp()->WriteProfileInt(_T("Settings"),_T("SplitterPos"),rect.Width());
		AfxGetApp()->WriteProfileString(_T("Settings"),_T("HeaderList"), pView->GetHeaderState());
	}
}