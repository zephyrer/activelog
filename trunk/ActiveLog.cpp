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
#include "ActiveLog.h"
#include "ALDoc.h"


// ActiveLog

IMPLEMENT_DYNCREATE(ActiveLog, CCmdTarget)


ActiveLog::ActiveLog():
m_dwCookie(0) , 
m_bLockedExternal(FALSE)
{
	EnableAutomation();
	
	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.
	TRACE1("ActiveLog::ActiveLog refCount = 0x%04X\n", m_dwRef);
	AfxOleLockApp();
}

ActiveLog::~ActiveLog()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	
	//IUnknown* pUnk=0;
	//HRESULT hr = this->ExternalDisconnect QueryInterface( IID_IUnknown, (LPVOID*)&pUnk );
	//if( SUCCEEDED(hr) )
	//	CoDisconnectObject( pUnk, 0 );  // disconnect from any remaining clients

	// make sure all outside connections are disconnected
	//ExternalDisconnect();
	
	TRACE1("ActiveLog::~ActiveLog refCount = 0x%04X\n", m_dwRef);
	AfxOleUnlockApp();
}


void ActiveLog::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.
	TRACE1("ActiveLog::OnFinalRelease refCount = 0x%04X\n", m_dwRef);
	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(ActiveLog, CCmdTarget)
END_MESSAGE_MAP()


BEGIN_DISPATCH_MAP(ActiveLog, CCmdTarget)
	DISP_FUNCTION_ID(ActiveLog, "LogData", dispidLogData, LogData, VT_I2, VTS_BSTR VTS_BSTR)
END_DISPATCH_MAP()

// Note: we add support for IID_IActiveLog to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

//// {62489B91-5633-4C35-8E2B-80E3E9B44844}
//static const IID IID_IActiveLog =
//{ 0x62489B91, 0x5633, 0x4C35, { 0x8E, 0x2B, 0x80, 0xE3, 0xE9, 0xB4, 0x48, 0x44 } };

// {F5BB9079-4A35-414C-A785-E774BDBB17EE}
static const IID IID_IActiveLog =
{ 0xF5BB9079, 0x4A35, 0x414C, { 0xA7, 0x85, 0xE7, 0x74, 0xBD, 0xBB, 0x17, 0xEE } };

BEGIN_INTERFACE_MAP(ActiveLog, CCmdTarget)
	INTERFACE_PART(ActiveLog, IID_IActiveLog, Dispatch)
END_INTERFACE_MAP()

//// {3265FE87-F94C-486F-9C22-E377B6B4C756}
//IMPLEMENT_OLECREATE_FLAGS(ActiveLog, "VAALOG.ActiveLog", afxRegApartmentThreading, 0x3265fe87, 0xf94c, 0x486f, 0x9c, 0x22, 0xe3, 0x77, 0xb6, 0xb4, 0xc7, 0x56)

// {0ACCCD94-CC09-46C2-803D-CB303F25C583}
IMPLEMENT_OLECREATE_FLAGS(ActiveLog, "VAALOG.ActiveLog", afxRegApartmentThreading, 0xacccd94, 0xcc09, 0x46c2, 0x80, 0x3d, 0xcb, 0x30, 0x3f, 0x25, 0xc5, 0x83 )


// ActiveLog message handlers
DWORD ActiveLog::GetRefCount()
{
	return m_dwRef;
}

void ActiveLog::RegisterAsActive()
{
	//This helper function actually registers our
	//object as running in the running object table (ROT).
	//Register the object by passing the IUnknown pointer
	//to the object and the CLSID from our CWinApp object
	//which was given to us by AppWizard
	HRESULT hr;
	CALApp *pApp = (CALApp*)AfxGetApp();
	hr=RegisterActiveObject(GetControllingUnknown(),
							pApp->GetAppCLSID(),
							ACTIVEOBJECT_WEAK,
							&m_dwCookie);
	if(FAILED(hr))
	{
		AfxMessageBox(_T("Didn't register Active Object"));
	}
}

SHORT ActiveLog::LogData(LPCTSTR xData, LPCTSTR xLabel)
{
	// TODO CHECK WHY IS COMMENTED IN REGOUT
	//AFX_MANAGE_STATE(AfxGetAppModuleState());
	__try
	{
		CFrameWnd* pFrame = (CFrameWnd*) AfxGetMainWnd ();
		if(pFrame != NULL)
		{
			CALDoc* pDoc = (CALDoc*) pFrame->GetActiveDocument ();
			ASSERT_VALID(pDoc);
			pDoc->LogData (xData, xLabel);
		}
		else
		{
			return -1;
		}
	}
	__except (EXCEPTION_EXECUTE_HANDLER) // warning C6320: Exception-filter expression is the constant EXCEPTION_EXECUTE_HANDLER. This might mask exceptions that were not intended to be handled
	{
		// memory exception processed here
		// warning C6322: Empty _except block
	}

	return 0;
}

int ActiveLog::ReleaseExternal(void)
{
	//If we didn't throw an external lock on, we can just
	//close the document normally, otherwise, we should
	//unlock our document using CoLockObjectExternal.  We
	//are registered as ACTIVEOBJECT_WEAK so in this instance
	//unlocking the object with CoLockObjectExternal will be
	//the last reference count on the object which will remove
	//it from the ROT.
	m_bLockedExternal = FALSE;
	HRESULT hr = CoLockObjectExternal(GetControllingUnknown(),FALSE,TRUE);
	if (m_dwCookie != 0)
		hr = RevokeActiveObject(m_dwCookie,NULL);
	hr = CoDisconnectObject(GetControllingUnknown(),0);
	return 0;
}

// http://support.microsoft.com/kb/q155690/
// The calls to AfxSetUserControl and CoLockObjectExternal ensure that the automation 
// server behaves correctly if the application window becomes visible.
int ActiveLog::LockExternal(void)
{
	//Make sure the framework knows the user took control.
	//When an automation method makes an applications main window
	//visible, then the user is in control and the app should not
	//be shut down when the client disconnects from it
	AfxOleSetUserCtrl(TRUE);

	//We've put our user in control of our application, now we
	//should make sure our automation object, which is our document
	//in this case, doesn't get destroyed when the last user
	//disconnects from us.
	if(!m_bLockedExternal) //only lock external once
	{
		//CoLockObjectExternal adds a strong lock, thereby preventing the object's 
		//reference count from reaching zero.
		HRESULT HR = CoLockObjectExternal(GetControllingUnknown(),TRUE,TRUE);
		if(FAILED(HR))
		{
			AfxMessageBox(_T("Didn't CoLockObjectExternal"));
			return -1;
		}
		m_bLockedExternal = TRUE;
	}

	return 0;
}
