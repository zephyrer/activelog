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
#include <msxml6.h>				// MSXML6 parser
#include "ixmldoc.h"

class CxmlDoc : public IxmlDoc
{
public:
	enum XType {
	   none,
       doc10,
       doc26,
       doc30,
       doc40,
       doc60
    };

	virtual bool LoadXML(CString strData);
	virtual bool SaveXML(CString strFileName);
	HRESULT static Create(CxmlDoc ** ppDoc);
    virtual void Close(); // Could be alternative solution

private:
    CxmlDoc(IXMLDOMDocument *pDom, XType xtype);
	~CxmlDoc(void);	

	HRESULT ReportError(IXMLDOMParseError *pXMLError);
	HRESULT LoadAsBSTR(CString strData);
	HRESULT CreateInstance(IN REFCLSID rclsid);
	HRESULT BytesToVariantArray(PBYTE pValue, ULONG cValueElements,	VARIANT *pVariant);
	HRESULT LoadFromArray(PBYTE pData, ULONG ulLen);
	HRESULT LoadFromArray2(PBYTE pData, ULONG ulLen);
	HRESULT SaveDocument(CString strFileName);
	HRESULT CheckLoad();
	HRESULT LoadInMemory(PBYTE pData, ULONG ulLen);
    IXMLDOMDocument *pXMLDom;
	XType xmltype;
};
