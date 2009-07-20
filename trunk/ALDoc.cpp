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

#include "ALDoc.h"
//#include "SAXParser.h"
#include "CxmlDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CALDoc* CALDoc::m_pThis = NULL;

// CALDoc

IMPLEMENT_DYNCREATE(CALDoc, CDocument)

BEGIN_MESSAGE_MAP(CALDoc, CDocument)
END_MESSAGE_MAP()


// CALDoc construction/destruction

CALDoc::CALDoc()
{
TRACE("CALDoc::CALDoc\n");

	GetNewTempFileName();

	//set security attributes to inherit handle
	SECURITY_ATTRIBUTES sa = {sizeof(SECURITY_ATTRIBUTES), NULL, TRUE};

	// Create the event that will signal the threads to close. 
	m_hCloseEvent = CreateEvent(&sa, TRUE, FALSE, NULL); 

	// create the validator thread 
	m_pThis = this;

	m_pValidatorThread = AfxBeginThread(&CALDoc::ValidatorThread, this, THREAD_PRIORITY_NORMAL, 0, 0, &sa);
}

CALDoc::~CALDoc()
{
TRACE("CALDoc::~CALDoc\n");

    if (m_hCloseEvent != NULL) 
		SetEvent(m_hCloseEvent);
}

BOOL CALDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

    if (m_hCloseEvent == NULL) 
		return FALSE;
         
    if (m_pValidatorThread == NULL) 
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	//Register object as running so controllers
	//can connect to it.
	m_ActiveLog.RegisterAsActive();

	return TRUE;
}

// CALDoc serialization

void CALDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}


// CALDoc diagnostics

#ifdef _DEBUG
void CALDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CALDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CALDoc commands
// ActiveLog message handlers
SHORT CALDoc::LogData(LPCTSTR xData, LPCTSTR xLabel)
{
	// TODO CHECK WHY IS COMMENTED IN REGOUT
	//AFX_MANAGE_STATE(AfxGetAppModuleState());

	PITEMDATA pData = new ITEMDATA;

	// MFC automatically converts from Unicode BSTR to 
    // Ansi CString, if necessary...
	// So a TCHAR is a char in MBCS builds, and a wchar_t in Unicode builds

	pData->sLabel	= xLabel;
	pData->sData	= xData;
	pData->nType	= dttext;	//default
	GetLocalTime(&pData->stTime);

	// instance of XML Parser
//	SAXParser m_Parser;
//	pData->nType = m_Parser.ParseData(pData->sData);

    m_pValidatorThread->PostThreadMessage(PWM_NEWDATA, (WPARAM) 0, (LPARAM) pData);


//	m_listItems.AddTail(pData);
//	UpdateAllViews(NULL);



	// Code below come from regaut sample and is for MDI app
	// where multiple windows could be 
    // So client window has to be bring to the front

	////First, show the window
	//POSITION pos = GetFirstViewPosition();
	//CView* pView = GetNextView(pos);
	//if (pView != NULL)
	//{
	//	CFrameWnd* pFrameWnd = pView->GetParentFrame();
	//	pFrameWnd->ActivateFrame(SW_SHOW);
	//	pFrameWnd = pFrameWnd->GetParentFrame();
	//	if (pFrameWnd != NULL)
	//		pFrameWnd->ActivateFrame(SW_SHOW);
	//}
	
	m_ActiveLog.LockExternal();

	return 0;
}

// Comment below is not for this case, this app will never make second call to open document

// http://www.microsoft.com/msj/1197/c1197.aspx
// SDI - if User opens a document and then opens another one, MFC never calls OnCloseDocument for 
// the first document. Surprise! That's because MFC reuses the same CDocument object for the new file. 
// It never calls OnCloseDocument; it just opens another document. If the user selects File | New, 
// MFC calls OnNewDocument directly, without ever calling OnCloseDocument. What all this means is that 
// there are three places you must close the file: OnCloseDocument, OnOpenDocument, and OnNewDocument
void CALDoc::OnCloseDocument()
{
	if(!m_ActiveLog.m_bLockedExternal)
	{
		CDocument::OnCloseDocument();
	}
	else
	{
		m_ActiveLog.ReleaseExternal();
		CDocument::OnCloseDocument();
	}
}

BOOL CALDoc::IsModified()
{
	// Do not show Save message on application close
	return FALSE;
}


// create a new temp file name
bool CALDoc::GetNewTempFileName(void)
{
	// Get temporary file name in the user's temp directory
	TCHAR szPathOrig[_MAX_PATH];
	GetTempPath(_MAX_PATH, szPathOrig);
	GetTempFileName(szPathOrig, _T("tmpXML"), 0, szPathOrig); 

    m_strTempFileName = szPathOrig;

	//append XML extension
	m_strTempFileName = m_strTempFileName + _T(".xml");

	TRACE("m_strTempFileName=%s\n",m_strTempFileName);

	return true;
}

void CALDoc::DeleteContents()
{
	// based on collect vs8 project

	// Delete all of the CMyStruct objects pointed to
	// by the CTypedPtrList. Then remove all of the
	// CMyStruct pointers from the CTypedPtrList, which
	// is faster than removing each individually.
	POSITION pos = m_listItems.GetHeadPosition();
	while (pos != NULL)
	{
		delete m_listItems.GetNext(pos);
	}
	m_listItems.RemoveAll();


	//while (!m_listItems.IsEmpty()) {
	//	delete m_listItems.RemoveHead();
	//}

	TRACE("DeleteContents\n");
	CDocument::DeleteContents();
}

//bool CALDoc::LockedExternal(void)
//{
//	DWORD refCount = m_ActiveLog.GetRefCount();
//	TRACE1("LockedExternal refCount = 0x%04X\n", refCount);
//	return (refCount>0) ;
//}

UINT CALDoc::ValidatorThread(LPVOID)
{
	ASSERT(m_pThis != NULL);

	HRESULT hRes = OleInitialize(NULL);
	ASSERT(hRes == S_OK || hRes == S_FALSE);

	//// create validator
	CxmlDoc* pXmlDoc = NULL;
	CxmlDoc::Create(&pXmlDoc);

	m_pThis->DoValidate(pXmlDoc);

	if(pXmlDoc)
	{
		pXmlDoc->Close();
		pXmlDoc = NULL;
	}

	OleUninitialize();

	return 0;
}

BOOL CALDoc::DoValidate(IxmlDoc* pXmlDoc)
{
    // This is the main loop.  Loop until we break out. 
	// while event not signalled -- process messages
	while (MsgWaitForMultipleObjects(
					1,					// number of handles in array
					&m_hCloseEvent,		// object-handle array
					FALSE,				// If FALSE, the function returns when any objects is set to signaled, or an input event has been received
					INFINITE,			// time-out interval
					QS_ALLINPUT			// Any message is in the queue		
		) != WAIT_OBJECT_0)
	{
		MSG msg;

		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{ 
			//if (msg.message == WM_QUIT) { 
			//    return 0; 
			//} 
			//else 
			if(msg.message == PWM_NEWDATA) 
			{ 
				// Handle the message. 

 				PITEMDATA pData = (PITEMDATA) msg.lParam;

				// instance of XML Parser
				if( pXmlDoc->LoadXML(pData->sData))
				{
					pData->nType = dtxml;
				}

				// post message to cLeftView
				PostMessage(m_hLeftView, WM_ON_ITEMDATA, (WPARAM) 0, (LPARAM) pData); 
			}
			else{
				TranslateMessage(&msg); 
				DispatchMessage(&msg); 
			}
        } 

	}

	return 0; 
}
