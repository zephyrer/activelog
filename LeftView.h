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
#include "afxcmn.h"

class CALDoc;
class CIEView;

class CLeftView : public CListView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	CALDoc* GetDocument();

// Operations
public:
	enum ImageIDS{
		IID_TEXT = 0,
		IID_XML  = 1
	};

// Overrides
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);

// Implementation
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:
   CIEView* m_pIEView;

// Generated message map functions
protected:
    afx_msg void OnSelChanged (NMHDR*, LRESULT*);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEditCopy();
	int CopyToClipboard(CString xData);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	LRESULT OnItemData(WPARAM wParam, LPARAM lParam);
    void SetIEView(CIEView* pIEView) { m_pIEView = pIEView; }
private:
	CImageList m_ImgList;
public:
	CString GetHeaderState(void);
	void RestoreHeaderState(CString szHeader);
protected:
	// Resize columns in a header
	void HeaderAutoFit(int nItem, bool change = true);
public:
	afx_msg void OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult);
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CALDoc* CLeftView::GetDocument()
   { return reinterpret_cast<CALDoc*>(m_pDocument); }
#endif

