#include "StdAfx.h"
#include "SAXParser.h"

SAXParser::SAXParser(void):pReader(NULL)
{
    HRESULT hr;
	if (FAILED(hr = CoCreateInstance(CLSID_SAXXMLReader, 
									NULL, 
									CLSCTX_INPROC_SERVER, 
									IID_ISAXXMLReader, 
									(void**) &pReader)))
    {
        //wprintf(L"Error creating sax reader, error is %08.8lx", hr);
        //return -1;
		pReader = NULL;
    }
}

SAXParser::~SAXParser(void)
{
	if(pReader != NULL) pReader->Release();
}
// return 0 if valid xml, or no validation is available
// return 1 if string is not valid xml
int SAXParser::ParseData(CString Data)
{
	if(pReader != NULL) 
	{
		VARIANT vData;
        HRESULT hr;
		VariantInit(&vData);
		vData.vt = VT_BSTR;
		vData.bstrVal = Data.AllocSysString();
		hr = pReader->parse(vData);
        SysFreeString(vData.bstrVal);
        VariantClear(&vData);
		if(FAILED(hr)) return (1);
	}
	//else 
	//{
	//	return -1;
	//}

	return 0;
}
