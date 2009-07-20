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

#include "ALDoc.h"
#include "LeftView.h"

// Safe for scripting interface
#include "CatHelp.h"

#include "LimitSingleInstance.h"
#include "RegUser.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CALApp

BEGIN_MESSAGE_MAP(CALApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CALApp::OnAppAbout)
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()


// CALApp construction

CALApp::CALApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}


// The one and only CALApp object

CALApp theApp;

// The one and only CLimitSingleInstance object.
CLimitSingleInstance g_SingleInstanceObj(TEXT("Global\\{0DE8FB46-C119-43a4-A209-7696D788AE56}"));

// This identifier was generated to be statistically unique for your app
// You may change it if you prefer to choose a specific identifier

// {0ACCCD94-CC09-46C2-803D-CB303F25C583}
static const CLSID clsid =
{ 0xACCCD94, 0xCC09, 0x46C2, { 0x80, 0x3D, 0xCB, 0x30, 0x3F, 0x25, 0xC5, 0x83 } };

const GUID CDECL BASED_CODE _tlid =
		{ 0xC658A703, 0xE1FA, 0x43B5, { 0x95, 0x36, 0x4, 0x20, 0xA, 0x18, 0x6B, 0xE1 } };
const WORD _wVerMajor = 1;
const WORD _wVerMinor = 0;

const CATID CATID_SafeForScripting     =
		{0x7dd95801, 0x9882, 0x11cf, {0x9f, 0xa9, 0x00, 0xaa, 0x00, 0x6c, 0x42, 0xc4}};
const CATID CATID_SafeForInitializing  =
		{0x7dd95802, 0x9882, 0x11cf, {0x9f, 0xa9, 0x00, 0xaa, 0x00, 0x6c, 0x42, 0xc4}};

//const GUID CDECL BASED_CODE _ctlid =
//		{ 0xACCCD94, 0xCC09, 0x46C2, { 0x80, 0x3D, 0xCB, 0x30, 0x3F, 0x25, 0xC5, 0x83 } };

UINT CALApp::m_nShutdownMsg = RegisterWindowMessage(_T("TryVAALogShutdown"));

// CALApp initialization
BOOL CALApp::InitInstance()
{
	// this call will popup onother instance if is running   
    if (g_SingleInstanceObj.IsAnotherInstanceRunning())
	{
		return FALSE; 
	}
	
	// Register and exit if it fails
	if (!g_SingleInstanceObj.RegisterInstance())
	{
		TRACE("Class Registration Failed\n");
		return FALSE; 
	}

	// InitCommonControlsEx() is required on Windows XP if an application
	// manifest specifies use of ComCtl32.dll version 6 or later to enable
	// visual styles.  Otherwise, any window creation will fail.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// Set this to include all the common control classes you want to use
	// in your application.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	// Initialize OLE libraries
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	// of your final executable, you should remove from the following
	// the specific initialization routines you do not need
	// Change the registry key under which our settings are stored
	// TODO: You should modify this string to be something appropriate
	// such as the name of your company or organization
	SetRegistryKey(_T("ActiveLog"));

	// DO WE HAVE THIS ???
	//LoadStdProfileSettings(4);  // Load standard INI file options (including MRU)
	//If nMaxMRU is 0, no MRU list will be maintained.
	LoadStdProfileSettings(0);

	WINDOWPLACEMENT wp;
	if (ReadWindowPlacement(&wp))
	{
		if (wp.flags & WPF_RESTORETOMAXIMIZED)
			m_nCmdShow = SW_SHOWMAXIMIZED;
	}

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CALDoc),
		RUNTIME_CLASS(CMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CLeftView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);

	// Parse command line for standard shell commands, DDE, file open
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// http://support.microsoft.com/kb/q129095/
	// info about reg type and names

	// App was launched with /Embedding or /Automation switch.
	// Run app as automation server.
	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Register all OLE server factories as running.  This enables the
		//  OLE libraries to create objects from other applications
		COleTemplateServer::RegisterAll();

		// WORKAROUND - TO INITIALIZE DOC-VIEW
		// Don't show the main window
		//return TRUE;
	}
	// App was launched with /Unregserver or /Unregister switch.  Unregister
	// typelibrary.  Other unregistration occurs in ProcessShellCommand().
	else if (cmdInfo.m_nShellCommand == CCommandLineInfo::AppUnregister)
	{
		//FALSE results in unregistering all COM objects implemented as
		//CCmdTarget derived classes.
		COleObjectFactory::UpdateRegistryAll(FALSE);
		AfxOleUnregisterTypeLib(_tlid, _wVerMajor, _wVerMinor);
	}
	// App was launched standalone or with other switches (e.g. /Register
	// or /Regserver).  Update registry entries, including typelibrary.
	else
	{
		COleObjectFactory::UpdateRegistryAll();
		// Register all OLE server factories as running.  This enables the
		//  OLE libraries to create objects from other applications
		COleTemplateServer::RegisterAll();
		AfxOleRegisterTypeLib(AfxGetInstanceHandle(), _tlid);
	}

	// warning C6214: Cast between semantically different integer types: HRESULT to a Boolean type
	// move this to setup application ???

	// v.0.09.02 - fix for Vista
	// do not exit application, this job has been moved to installation
	RegisterSafeScripting();

	// Dispatch commands specified on the command line.  Will return FALSE if
	// app was launched with /RegServer, /Register, /Unregserver or /Unregister.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// call DragAcceptFiles only if there's a suffix
	//  In an SDI app, this should occur after ProcessShellCommand
	return TRUE;
}

// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// App command to run the dialog
void CALApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

CLSID CALApp::GetAppCLSID()
{
	//helper function to return the CLSID that ClassWizard
	//gave us.
	return clsid;
}

BOOL CAboutDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CALApp message handlers
HRESULT CALApp::RegisterSafeScripting()
{
	// attach Safe for scripting interface - START
	if (FAILED( CreateComponentCategory(
		  CATID_SafeForScripting,
		  L"Controls that are safely scriptable") ))
		return ResultFromScode(SELFREG_E_CLASS);

	if (FAILED( CreateComponentCategory(
		  CATID_SafeForInitializing,
		  L"Controls safely initializable from persistent data") ))
		return ResultFromScode(SELFREG_E_CLASS);

	if (FAILED( RegisterCLSIDInCategory(
		  clsid, CATID_SafeForScripting) ))
		return ResultFromScode(SELFREG_E_CLASS);

	if (FAILED( RegisterCLSIDInCategory(
		  clsid, CATID_SafeForInitializing) ))
		return ResultFromScode(SELFREG_E_CLASS);
	// attach Safe for scripting interface - END

	return S_OK;
}