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

#include "StdAfx.h"
#include "CxmlDoc.h"

CxmlDoc::CxmlDoc(IXMLDOMDocument *pDom, XType xtype): pXMLDom(NULL), xmltype(none)
{
	pXMLDom = pDom;
	xmltype = xtype;
}

CxmlDoc::~CxmlDoc(void)
{
    SAFERELEASE(pXMLDom);
}

// all variables are local in static function. Otherwise next error will popup:
// 'illegal reference to non-static member'
// A nonstatic member is specified in a static member function. To
// access the nonstatic member, you must create an instance of the class
// and use a member-access operator (. or ->).
HRESULT CxmlDoc::Create(CxmlDoc ** ppDoc)
{

	HRESULT hr = S_OK;
	IXMLDOMDocument2 *pDom2 = NULL;
	IXMLDOMDocument  *pDom  = NULL;
	XType xtype = none; 

	if( SUCCEEDED(hr = CoCreateInstance(__uuidof(DOMDocument60),
                      NULL,
                      CLSCTX_INPROC_SERVER,
                      __uuidof(IXMLDOMDocument),
                      (void**)&pDom)))
	{
		xtype = doc60;
		dprintf("DOMDocument60 available\n");
	}
	else
	{
		if( SUCCEEDED(hr = CoCreateInstance(__uuidof(DOMDocument40),
                      NULL,
                      CLSCTX_INPROC_SERVER,
                      __uuidof(IXMLDOMDocument),
                      (void**)&pDom)))
		{
			xtype = doc40;
			dprintf("DOMDocument40 available\n");
		}
	}

	if(SUCCEEDED(hr))
	{
		if (SUCCEEDED(pDom->QueryInterface(IID_IXMLDOMDocument2, (void **)&pDom2))) {
			BSTR bstrNewParser = SysAllocString(L"NewParser");
			VARIANT varNewParser;
			VariantInit(&varNewParser);
			varNewParser.vt = VT_BOOL;
			varNewParser.boolVal = VARIANT_TRUE;
			if( FAILED(pDom2->setProperty(bstrNewParser, varNewParser)))
			{
				dprintf( "New Parser is not available  0x%.8x\n", hr );
			}
			VariantClear(&varNewParser);
			SysFreeString(bstrNewParser);
			dprintf("NewParser available\n");
			SAFERELEASE(pDom2);        
		}
	}
	
	if(FAILED(hr))
	{
		if( SUCCEEDED(hr = CoCreateInstance(__uuidof(DOMDocument30),
						  NULL,
						  CLSCTX_INPROC_SERVER,
						  __uuidof(IXMLDOMDocument),
						  (void**)&pDom)))
		{
			xtype = doc30;
			dprintf("DOMDocument30 available\n");
		}
		else
		{
			if( SUCCEEDED(hr = CoCreateInstance(__uuidof(DOMDocument26),
							  NULL,
							  CLSCTX_INPROC_SERVER,
							  __uuidof(IXMLDOMDocument),
							  (void**)&pDom)))
			{
				xtype = doc26;
				dprintf("DOMDocument26 available\n");
			}
			else
			{
				if( FAILED(hr = CoCreateInstance(CLSID_DOMDocument,
								  NULL,
								  CLSCTX_INPROC_SERVER,
								  __uuidof(IXMLDOMDocument),
								  (void**)&pDom)))
				{
				
					CLSID clsid;
					CLSIDFromProgID(L"msxml.domdocument", &clsid);  
					
					CHECKHR( CoCreateInstance(clsid,
								  NULL,
								  CLSCTX_INPROC_SERVER,
								  __uuidof(IXMLDOMDocument),
								  (void**)&pDom));
				}

				xtype = doc10;
			}
		}
	}

    CHECKHR( pDom->put_async(VARIANT_FALSE));
    CHECKHR( pDom->put_validateOnParse(VARIANT_TRUE));
    CHECKHR( pDom->put_resolveExternals(VARIANT_FALSE));
	
    *ppDoc = new CxmlDoc(pDom, xtype);

	if(*ppDoc == NULL)
	{
		SAFERELEASE(pDom);
		hr = E_FAIL;
	}

	return hr;
CleanUp:
	
	dprintf("DOMDocument is not available\n");

    SAFERELEASE(pDom);        
    return hr;
}

void CxmlDoc::Close(void)
{
    delete this;
}

////////////////////////////////////////////////////////////////////////////
// Function: Load the document from in-memory encoded bytes.
////////////////////////////////////////////////////////////////////////////
HRESULT CxmlDoc::LoadInMemory(PBYTE pData, ULONG ulLen)
{
    IPersistStreamInit     *pPSI = NULL;
    IStream                *pStm = NULL;
    HRESULT hr;
    ULONG ulWritten;
    LARGE_INTEGER li = {0, 0};
	//const WCHAR UNICODE_RBOM            = 0xFFFE;
	//const TCHAR UNICODE_EOF             = (TCHAR) 0;


    // Create a stream and write the bytes to it
    CHECKHR(CreateStreamOnHGlobal(NULL, TRUE, &pStm));
//    pStm->Write(&UNICODE_RBOM, sizeof(WCHAR), &ulWritten);
    pStm->Write(pData, ulLen, &ulWritten);
//    pStm->Write(&UNICODE_EOF, sizeof(TCHAR), &ulWritten);

    // Reset the stream back to the beginning
	// Q255799 - otherwise will fail to load with
	// Microsoft XML, versions 2.0, 2.5, 2.6, 3.0
    CHECKHR(pStm->Seek(li, STREAM_SEEK_SET, NULL));

    // Now, load the document from the stream
    CHECKHR(pXMLDom->QueryInterface(IID_IPersistStreamInit, (void **)&pPSI));
    CHECKHR(pPSI->Load(pStm));

CleanUp:
    SAFERELEASE(pPSI);
    SAFERELEASE(pStm);
    return hr;
}

////////////////////////////////////////////////////////////////////////////
// Function: Load the document from in-memory encoded bytes.
////////////////////////////////////////////////////////////////////////////
HRESULT CxmlDoc::LoadFromArray(PBYTE pData, ULONG ulLen)
{
    VARIANT_BOOL    status;
	VARIANT			var;
    HRESULT         hr;

	CHECKHR(BytesToVariantArray(
			pData, //Pointer to bytes to put in a variant array.
			ulLen, //Size of pValue in bytes.
			&var   //Return variant containing octet string (VT_UI1|VT_ARRAY).
		)) ;

	// Now, load the document from the array
	CHECKHR(pXMLDom->load(var, &status));

	if ( status != VARIANT_TRUE ) {
		hr = E_FAIL;  //CheckLoad();
	}

CleanUp:
    if (&var) VariantClear(&var);
    return hr;
}

////////////////////////////////////////////////////////////////////////////
// Function: Load the document from in-memory encoded bytes.
////////////////////////////////////////////////////////////////////////////
HRESULT CxmlDoc::LoadFromArray2(PBYTE pData, ULONG ulLen)
{
    VARIANT_BOOL    status;
	VARIANT			var;
    HRESULT         hr;

    COleSafeArray vPostData;
    vPostData.CreateOneDim(VT_UI1, ulLen, pData);

	// Now, load the document from the array
	CHECKHR(pXMLDom->load(COleVariant(vPostData), &status));

	if ( status != VARIANT_TRUE ) {
		hr = E_FAIL;  //CheckLoad();
	}

CleanUp:
    if (&var) VariantClear(&var);
    return hr;
}

////////////////////////////////////////////////////////////////////////////
// Function: Load the document from in-memory encoded bytes.
////////////////////////////////////////////////////////////////////////////
HRESULT CxmlDoc::LoadAsBSTR(CString strData)
{
    VARIANT_BOOL    status;
    BSTR			bstr = NULL;
    HRESULT         hr;

	bstr = strData.AllocSysString();
	CHECKHR(pXMLDom->loadXML(bstr, &status));

	if ( status != VARIANT_TRUE ) {
		hr = E_FAIL;  //CheckLoad();
	}

CleanUp:
	if (bstr) SysFreeString(bstr);
    return hr;
}

bool CxmlDoc::LoadXML(CString strData)
{
    PBYTE pXML  = (PBYTE) strData.GetBuffer();  // This is an LPTSTR pointer to the character buffer of the null-terminated object
    ULONG len   = strData.GetLength() * sizeof(TCHAR);  // This is a count of the bytes in the string

	switch( xmltype )
	{
		case doc60:
		case doc40:
		case doc30:
			return(SUCCEEDED(LoadAsBSTR(strData)));
// 		    return(SUCCEEDED(LoadFromArray2(pXML, len)));
		case doc26:
		case doc10:
		default:
			// reuse of same doc could fail to load xml
 		    return(SUCCEEDED(LoadFromArray2(pXML, len)));
	}
}

bool CxmlDoc::SaveXML(CString strFileName)
{
    return(SUCCEEDED(SaveDocument(strFileName)));
}

////////////////////////////////////////////////////////////////////////////
// Helper function: Check load results
////////////////////////////////////////////////////////////////////////////
HRESULT CxmlDoc::CheckLoad()
{
    // And since we don't have the VARIANT_BOOL from load we
    // need to check the parse Error errorCode property to see
    // if everything went ok.
    IXMLDOMParseError  *pXMLError = NULL;
    LONG errorCode = E_FAIL;
    HRESULT hr;

    CHECKHR(pXMLDom->get_parseError(&pXMLError));
    CHECKHR(pXMLError->get_errorCode(&errorCode));
    if (errorCode != 0)
    {
        ReportError(pXMLError);
        goto CleanUp;
    }
CleanUp:
    SAFERELEASE(pXMLError);
    return errorCode;
}

////////////////////////////////////////////////////////////////////////////
// Function: Save document out to specified local file.
// File name could be class property
////////////////////////////////////////////////////////////////////////////
HRESULT CxmlDoc::SaveDocument(CString strFileName)
{
    HRESULT hr = S_OK;
    VARIANT_BOOL    status;
    VARIANT vName;

	VariantInit(&vName);

    vName.vt = VT_BSTR;
    V_BSTR(&vName) = strFileName.AllocSysString();
	
	CHECKHR(pXMLDom->save(vName));

	// reset content
	CHECKHR(pXMLDom->loadXML(NULL,&status));

CleanUp:
    if (&vName) VariantClear(&vName);
    return hr;
}

HRESULT CxmlDoc::BytesToVariantArray(
    PBYTE pValue, //Pointer to bytes to put in a variant array.
    ULONG cValueElements,//Size of pValue in bytes.
    VARIANT *pVariant //Return variant containing octet string (VT_UI1|VT_ARRAY).
    )
{
    HRESULT hr = E_FAIL;
    SAFEARRAY *pArrayVal = NULL;
    SAFEARRAYBOUND arrayBound;
    CHAR HUGEP *pArray = NULL;
 
    //Set bound for array
    arrayBound.lLbound = 0;
    arrayBound.cElements = cValueElements;
 
    //Create the safe array for the octet string. unsigned char elements;single dimension;aBound size.
    pArrayVal = SafeArrayCreate( VT_UI1, 1, &arrayBound );
 
    if (!(pArrayVal == NULL) )
    {
        hr = SafeArrayAccessData(pArrayVal, (void HUGEP * FAR *) &pArray );
        if (SUCCEEDED(hr))
        {
            //Copy the bytes to the safe array.
            memcpy( pArray, pValue, arrayBound.cElements );
            SafeArrayUnaccessData( pArrayVal );
            //Set type to array of unsigned char
            V_VT(pVariant) = VT_ARRAY | VT_UI1;
            //Assign the safe array to the array member.
            V_ARRAY(pVariant) = pArrayVal;
            hr = S_OK;
        }
        else
        {
            //Clean up if array can't be accessed.
            if ( pArrayVal )
               SafeArrayDestroy( pArrayVal );
        }
    }
    else
    {
      hr = E_OUTOFMEMORY;
    }
 
    return hr;
}

////////////////////////////////////////////////////////////////////////////
// Helper function: Report parsing error information
////////////////////////////////////////////////////////////////////////////
HRESULT CxmlDoc::ReportError(IXMLDOMParseError *pXMLError)
{
    long line, linePos;
    LONG errorCode;
    BSTR pBURL, pBReason;
    HRESULT hr;

    CHECKHR(pXMLError->get_line(&line));
    CHECKHR(pXMLError->get_linepos(&linePos));
    CHECKHR(pXMLError->get_errorCode(&errorCode));
    CHECKHR(pXMLError->get_url(&pBURL));
    CHECKHR(pXMLError->get_reason(&pBReason));

    //dprintf("%S", pBReason);
    //if (line > 0)
    //{
    //    dprintf("Error on line %d, position %d in \"%S\".\n", 
    //        line, linePos, pBURL);
    //}

CleanUp:

    SysFreeString(pBURL);
    SysFreeString(pBReason);

    return E_FAIL;
}