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

#include "ActiveLog.h"

const UINT		PWM_NEWDATA			=	WM_USER+1;		// new data msg
const UINT		WM_ON_ITEMDATA		=	WM_USER+2;

class CALDoc : public CDocument
{
protected: // create from serialization only
	CALDoc();
	DECLARE_DYNCREATE(CALDoc)

// Attributes
public:
	CTypedPtrList<CPtrList, PITEMDATA> m_listItems;
	CString m_strTempFileName;
	HWND		m_hLeftView;

// Operations
public:
	SHORT LogData(LPCTSTR xData, LPCTSTR xLabel);	// TODO VERIFY IF PARAMETERS ARE CORRECT

// Overrides
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	virtual BOOL IsModified( ); 

// Implementation
public:
	virtual ~CALDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
	ActiveLog	m_ActiveLog;
    HANDLE		m_hCloseEvent;		
    CWinThread*	m_pValidatorThread;
	static CALDoc *m_pThis;

protected:
	static UINT ValidatorThread(LPVOID pParam);
	BOOL DoValidate(IxmlDoc* pXmlDoc);

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnCloseDocument();
	// create a new temp file name
	bool GetNewTempFileName(void);
	virtual void DeleteContents();
	//bool LockedExternal(void);
};


