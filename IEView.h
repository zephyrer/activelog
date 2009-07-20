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

class CListView;

class CIEView : public CHtmlView
{
protected: // create from serialization only
	CIEView();
	DECLARE_DYNCREATE(CIEView)

// Attributes
public:
	CALDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
    void SetListView(CListView* pListView) { m_pListView = pListView; }
protected:
	virtual void OnInitialUpdate(); // called first time after construct
    CListView* m_pListView;

// Implementation
public:
	virtual ~CIEView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	DECLARE_MESSAGE_MAP()
public:
	bool SaveFile(CString strFileName, CString strData);
	HRESULT NavigateText(CString strText);
	HRESULT NavigateText2(CString strText);
	// Instance of MSXML document object for validating and savind data
	IxmlDoc* m_pXmlDoc;
	afx_msg void OnEditFind();
	HRESULT LoadContent(CString strText);
	int ShowItem(PITEMDATA pData);
};

#ifndef _DEBUG  // debug version in IEView.cpp
inline CALDoc* CIEView::GetDocument() const
   { return reinterpret_cast<CALDoc*>(m_pDocument); }
#endif

