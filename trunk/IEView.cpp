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
#include "IEView.h"
#include "LeftView.h"
#include "cxmldoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CIEView
IMPLEMENT_DYNCREATE(CIEView, CHtmlView)

BEGIN_MESSAGE_MAP(CIEView, CHtmlView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CHtmlView::OnFilePrint)
	ON_COMMAND(ID_EDIT_FIND, &CIEView::OnEditFind)
END_MESSAGE_MAP()

static const GUID CGID_IWebBrowser =
{ 0xED016940L, 0xBD5B, 0x11cf, { 0xBA, 0x4E, 0x00, 0xC0, 0x4F, 0xD7, 0x08, 0x16 } };

//DEFINE_GUID(CGID_IWebBrowser,0xED016940L,
//            0xBD5B,0x11cf,0xBA, 0x4E,0x00,
//            0xC0,0x4F,0xD7,0x08,0x16);

// CIEView construction/destruction

CIEView::CIEView():m_pXmlDoc(NULL)
{
	// TODO: add construction code here
	m_pListView = NULL;
	CxmlDoc* pXmlDoc = NULL;
	CxmlDoc::Create(&pXmlDoc);
	m_pXmlDoc = pXmlDoc;
}

CIEView::~CIEView()
{
	if(m_pXmlDoc)
	{
		m_pXmlDoc->Close();
		m_pXmlDoc = NULL;
	}
}

BOOL CIEView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CHtmlView::PreCreateWindow(cs);
}

void CIEView::OnInitialUpdate()
{
	CHtmlView::OnInitialUpdate();
	Navigate2(_T("about:blank"),NULL,NULL);
}


// CIEView printing



// CIEView diagnostics

#ifdef _DEBUG
void CIEView::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CIEView::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}

CALDoc* CIEView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CALDoc)));
	return (CALDoc*)m_pDocument;
}
#endif //_DEBUG


// CIEView message handlers
void CIEView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
	if(pSender)
	{
		if(pSender->IsKindOf( RUNTIME_CLASS( CLeftView )))
		{
			CALDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);
			if(!pDoc->m_listItems.IsEmpty())
			{
				// TODO Async open

				if (GetBusy())                          // See if it's busy with something else
					Stop();                             // .. and make sure it's not

				// get item index
				DWORD nIndex = (DWORD) lHint;
				POSITION pos = pDoc->m_listItems.FindIndex(nIndex);
				PITEMDATA pData = NULL;

				if( pos != NULL){
					pData = pDoc->m_listItems.GetAt(pos);
				}

				ShowItem(pData);
			}
		}
	}
}

bool CIEView::SaveFile(CString strFileName, CString strData)
{
	// ADD UNICODE BYTES

	CFile myFile;
	CFileException fileException;

	if ( !myFile.Open( strFileName, CFile::modeCreate |   
			CFile::modeWrite | CFile::shareDenyWrite , &fileException ) )
	{
		TRACE( "Can't open file %s, error = %u\n",
		   strFileName, fileException.m_cause );
		
//		if(fileException.m_cause == CFileException::sharingViolation)
//		TCHAR szLastError[1024]; 
//		sprintf_s(szLastError, "Can't open file %s, error = %u\n", pDoc->m_strTempFileName, fileException.m_cause); 
//		OutputDebugString(szLastError );
		
		return false;
	}
	else
	{
#if defined UNICODE
		// if parser is not available
		// other than UTF-16 encoding will fail to show
		const WCHAR UNICODE_BOM             = 0xFEFF;
		myFile.Write( &UNICODE_BOM, sizeof( UNICODE_BOM ));
		myFile.Write(strData, strData.GetLength() * sizeof(TCHAR));
		myFile.Close();
#else
		myFile.Write(strData, strData.GetLength());
		myFile.Close();
#endif
	}
	return true;
}

HRESULT CIEView::NavigateText(CString strText)
{
    // The Document Object Model isn't fully initialized until
    // the browser has finished navigating to a first URL. A quick
    // and handy one is "about:blank" - this has to be done before
	// executing this method

	HRESULT hr = S_OK;

	TRACE("NavigateText before GetHtmlDocument \n");

	//	Get a pointer to our document
	// TODO - Check if this is safe to use
    // Get a pointer to the HTML Document Object Model's interface
    IDispatch * pDisp = GetHtmlDocument();
    if (!pDisp)
        return E_FAIL;

	TRACE("NavigateText before QueryInterface IID_IHTMLDocument2\n");

    IHTMLDocument2* document = NULL;
    if (SUCCEEDED(pDisp->QueryInterface( IID_IHTMLDocument2, (void**)&document )))
    {
		//if (SUCCEEDED(document->open(CComBSTR("text/html"),
		//	CComVariant(CComBSTR("_self")),
		//	CComVariant(CComBSTR("")),
		//	CComVariant((bool)1),
		//	NULL)))   // <- if used busy cursor will be shown

		// IHTMLDocument2::open Method

        // Empty URL and parameters opens the current document
        CComBSTR    bstrURL;
        CComVariant varDummy;
		TRACE("document->opening...\n");
        if (SUCCEEDED(hr = document->open(bstrURL, varDummy, varDummy, varDummy, NULL)))
		{
			TRACE("document->open succeeded\n");

			// Following sample from IHTMLDocument2::write Method
			VARIANT *pVal;
			SAFEARRAY *sfArray;
			//BSTR bstr = SysAllocString(OLESTR("Written by IHTMLDocument2::write()."));

			// Creates a new one-dimensional array
			sfArray = SafeArrayCreateVector(VT_VARIANT, 0, 1);
			
			if (sfArray != NULL && document != NULL) 
			{
				hr = SafeArrayAccessData(sfArray,(LPVOID*) & pVal);
				pVal->vt = VT_BSTR;
				pVal->bstrVal = strText.AllocSysString();
				hr = SafeArrayUnaccessData(sfArray);
				hr = document->write(sfArray);					// Overwrite HTML
				if(FAILED(hr))
					dprintf( "document->open fail  0x%.8x\n", hr );
			}

			//SysFreeString(bstr);
			if (sfArray != NULL) {
				SafeArrayDestroy(sfArray);
			}
		}
		else{
			TRACE("document->open fail\n");
			dprintf( "document->open fail  0x%.8x\n", hr );
		}

		document->close();		// close, Update browser
		document->Release();	// Don't forget to release the reference
    }

	return hr;
}

HRESULT CIEView::NavigateText2(CString strText)
{
	ENSURE(m_pBrowserApp != NULL);
	HRESULT hr = E_FAIL;
	CComPtr<IDispatch> spDispDocument;
	m_pBrowserApp->get_Document(&spDispDocument);
	CComQIPtr<IHTMLDocument2> spDoc = spDispDocument;
	if (spDoc != NULL)
	{
		// open doc and write
		LPDISPATCH lpdRet;
		if (SUCCEEDED(hr = spDoc->open(CComBSTR("text/html"),
			CComVariant(CComBSTR("_self")),
			CComVariant(CComBSTR("")),
			CComVariant((bool)1),
			&lpdRet)))
		{
			COleSafeArray vPostData;
			vPostData.CreateOneDim(VT_UI1, strText.GetLength() * sizeof(TCHAR), strText.GetBuffer());

			hr = spDoc->write(COleVariant(vPostData).parray);	// write contents to doc
			hr = spDoc->close();						// close
		}
	}

	return hr;
}

HRESULT CIEView::LoadContent(CString strText)
{
	ENSURE(m_pBrowserApp != NULL);
	HRESULT hr = E_FAIL;
	CComPtr<IDispatch> spDispDocument;
	m_pBrowserApp->get_Document(&spDispDocument);
	CComQIPtr<IHTMLDocument2> spDoc = spDispDocument;
	if (spDoc != NULL)
	{
		CComQIPtr<IPersistStreamInit> spPSI = spDoc;
		if (spPSI)
		{
			ULONG ulWritten;
			LARGE_INTEGER li = {0, 0};

			//
			// TODO - Check if additionally is required
			// to allocate memory for CreateStreamOnHGlobal
			//
			CComPtr<IStream> spStream;
			CHECKHR(CreateStreamOnHGlobal(NULL, TRUE, &spStream));
			//
			// TODO - Check if verification for ulWritten is required
			//
			CHECKHR(spStream->Write(strText.GetBuffer(), 
					strText.GetLength() * sizeof(TCHAR), &ulWritten));
			// Reset the stream back to the beginning
			CHECKHR(spStream->Seek(li, STREAM_SEEK_SET, NULL));
			CHECKHR(spPSI->Load(spStream));
		}
	}

CleanUp:
	return hr;
}

void CIEView::OnEditFind()
{
	//1 : FIND, 2: VIEWSOURCE, 3 : OPTIONS
	DWORD nCmdID = 1; 

	// Same as CHtmlView::OnFilePrint

	if (m_pBrowserApp != NULL)
	{		
		CComPtr<IDispatch> spDisp;
		m_pBrowserApp->get_Document(&spDisp);
		if (spDisp != NULL)
		{
			// the control will handle all printing UI

			CComQIPtr<IOleCommandTarget> spTarget = spDisp;
			if (spTarget != NULL)
				spTarget->Exec(&CGID_IWebBrowser, nCmdID, 0, NULL, NULL);
		}
	}
}

/*
HRESULT CIEView::NavigateText(CString strText)
{
    // Get a pointer to the HTML Document Object Model's interface
    IDispatch * pDisp = GetHtmlDocument();
    if (!pDisp)
        return E_FAIL;

    IHTMLDocument2* document;
    if (SUCCEEDED(pDisp->QueryInterface( IID_IHTMLDocument2, (void**)&document )))
    {
        // Empty URL and parameters opens the current document
        CComBSTR    bstrURL;
        CComVariant varDummy;
        document->open(bstrURL, varDummy, varDummy, varDummy, NULL);

        // Create a safearray to store the HTML text
        SAFEARRAY      *pSA;
        SAFEARRAYBOUND  saBound = {1, 0};
        pSA = SafeArrayCreate(VT_VARIANT, 1, &saBound);

		if (pSA != NULL ) 
		{
			// Copy the HTML into the one and only element
			VARIANT   *pVar;
			CComBSTR   bstrHTML = strText;              // Load the text
			varDummy = bstrHTML;                        // .. into a variant ( reuse old one )

			SafeArrayAccessData(pSA, (void**)&pVar);    // Access safearray data
			pVar[0] = varDummy;                         // Set the text data
			SafeArrayUnaccessData(pSA);                 // Release access

			// Write the HTML as the document's new text
			document->write(pSA);                       // Overwrite HTML
		}
        
		document->close();                              // Update browser

        SafeArrayDestroy(pSA);                      // Finished with the safearray
        pDoc->Release();                            // Don't forget to release the reference
    }

	return S_OK;
}
*/

int CIEView::ShowItem(PITEMDATA pData)
{
	if(pData)
	{
		CALDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);

		if ( m_pXmlDoc )
		{
			if(m_pXmlDoc->LoadXML(pData->sData))
			{
				if(!m_pXmlDoc->SaveXML(pDoc->m_strTempFileName))
				{
					// possible sharingViolation error
					pDoc->GetNewTempFileName();
					m_pXmlDoc->SaveXML(pDoc->m_strTempFileName);
				}

				Navigate2(pDoc->m_strTempFileName, 
						navNoHistory, // Do not add the resource or file to the history list. 
									  // The new page replaces the current page in the list.
						NULL); 							
			}
			else
			{
				//NavigateText(pData->sData);
				LoadContent(pData->sData);
			}
		}
		else
		{
			if( !SaveFile(pDoc->m_strTempFileName, pData->sData) )
			{
				// possible sharingViolation error
				pDoc->GetNewTempFileName();
				if( !SaveFile(pDoc->m_strTempFileName, pData->sData) ) return -1;
				// OutputDebugString("New temporary file\n");
			}
			
			Navigate2(pDoc->m_strTempFileName, 
					navNoHistory, // Do not add the resource or file to the history list. 
					              // The new page replaces the current page in the list.
					NULL); 							
		}
	}
	else
	{
		// This call is required too much time for execution
		//Navigate2(_T("about:blank"), 
		//			navNoHistory, // Do not add the resource or file to the history list. 
		//						  // The new page replaces the current page in the list.
		//			NULL); 

		//NavigateText(_T(""));
		LoadContent(_T(""));
	}
	return 0;
}
