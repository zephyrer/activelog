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
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CLeftView
IMPLEMENT_DYNCREATE(CLeftView, CListView)

BEGIN_MESSAGE_MAP(CLeftView, CListView)
	// Standard printing commands
    ON_NOTIFY_REFLECT (LVN_ITEMCHANGED , OnSelChanged)
	ON_COMMAND(ID_EDIT_COPY, &CLeftView::OnEditCopy)
	ON_WM_CREATE()
	ON_MESSAGE(WM_ON_ITEMDATA, &CLeftView::OnItemData)
	ON_NOTIFY(HDN_ENDTRACKA, 0, &CLeftView::OnHdnEndtrack)
	ON_NOTIFY(HDN_ENDTRACKW, 0, &CLeftView::OnHdnEndtrack)
	ON_WM_SIZE()
	ON_NOTIFY(HDN_DIVIDERDBLCLICKA, 0, &CLeftView::OnHdnDividerdblclick)
	ON_NOTIFY(HDN_DIVIDERDBLCLICKW, 0, &CLeftView::OnHdnDividerdblclick)
END_MESSAGE_MAP()


// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here
   m_pIEView = NULL;
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying the CREATESTRUCT cs
	cs.style |= LVS_REPORT | LVS_SINGLESEL | LVS_NOSORTHEADER | LVS_SHOWSELALWAYS;
	return CListView::PreCreateWindow(cs);
}


// CLeftView printing
void CLeftView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	CListCtrl& ctlList = GetListCtrl();

	ctlList.SetRedraw(FALSE);        // otherwise is very slow on big list
							         // as alternative use callback refresh
	ctlList.DeleteAllItems();	     // Delete all of the items from the tree control.

	// remarks from http://msdn2.microsoft.com/en-us/library/ms834178.aspx
	// 
	// When refreshing the tree, we need to first delete the entire tree. When deleting the tree 
	// with DeleteTree(TVI_ROOT), it's a good idea to disable any processing that occurs in the 
	// OnSelchanged() event because deleting the tree can cause several selection changed events 
	// to occur as selected items are deleted. I accomplished this by setting a flag that tells 
	// my OnSelchanged() to do nothing. This made my refresh about five times faster.

	ASSERT(ctlList.GetItemCount() == 0);
	ctlList.SetRedraw(TRUE); 


	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetListCtrl().
}


// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CListView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CALDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CALDoc)));
	return (CALDoc*)m_pDocument;
}
#endif //_DEBUG

// CLeftView message handlers
void CLeftView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint)
{
/*	// TODO: You may populate your TreeView with items by directly accessing
	//  its tree control through a call to GetListCtrl().
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	CALDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
		
	if(!pDoc->m_listItems.IsEmpty())
	{
		if( itemFilesRoot == NULL )
		{
			CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
			ctlList.SetImageList (&m_ImgList, TVSIL_NORMAL);
			itemFilesRoot = ctlList.InsertItem("XML Files", IID_CLOSEFOLDER, IID_OPENFOLDER);
		}

		HTREEITEM hItem = ctlList.InsertItem(
				pDoc->m_strLabel, 
				IID_XML, 
				IID_XML, 
				itemFilesRoot, 
				TVI_LAST);


		// ISAPI Programming Tips
		// mk:@MSITStore:C:\Program%20Files\Microsoft%20Visual%20Studio\MSDN\2001JUL\1033
		// CString* pItemData = new CString("data");
		// TODO ADD TRY CATCH

		PITEMDATA pData = pDoc->m_listItems.GetTail();
		int nItem = (int) pDoc->m_listItems.GetCount();

		ctlList.InsertItem(nItem, pData->sLabel, pData->nType);
		ctlList.SetItemData(nItem, (DWORD) (pDoc->m_listItems.GetCount() - 1));

		// Fix v.0.05 : first item is not shown after invoking New ( reset list ) 
		if( pDoc->m_listItems.GetCount() == 1 )
		{
			ctlList.SetRedraw(TRUE); 
		}
	}
	else{

		if( ctlList.GetItemCount() != 0 )
		{
			ctlList.SetRedraw(FALSE);        // otherwise is very slow on big list
									         // as alternative use callback refresh
			ctlList.DeleteAllItems();	     // Delete all of the items from the tree control.

			// remarks from http://msdn2.microsoft.com/en-us/library/ms834178.aspx
			// 
			// When refreshing the tree, we need to first delete the entire tree. When deleting the tree 
			// with DeleteTree(TVI_ROOT), it's a good idea to disable any processing that occurs in the 
			// OnSelchanged() event because deleting the tree can cause several selection changed events 
			// to occur as selected items are deleted. I accomplished this by setting a flag that tells 
			// my OnSelchanged() to do nothing. This made my refresh about five times faster.

			ASSERT(ctlList.GetItemCount() == 0);
			ctlList.SetRedraw(TRUE); 
		}
	}*/
}

 
void CLeftView::OnSelChanged (NMHDR* pNMHDR, LRESULT* pResult)
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	     
	if ((pNMListView->uChanged & LVIF_STATE) &&
	   (pNMListView->uNewState & LVIS_SELECTED)) {
	    
		CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
		DWORD nIndex = (DWORD) ctlList.GetItemData(pNMListView->iItem);
		
		// send onupdate event with param 
		CALDoc* pDoc = GetDocument();
		ASSERT_VALID(pDoc);
		pDoc->UpdateAllViews(this, nIndex, NULL);
	}
 
	// Why in some code samples this set to zero ??
	*pResult = 0;
}


void CLeftView::OnEditCopy()
{
	// find selected item
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();

	POSITION pos = ctlList.GetFirstSelectedItemPosition();
	if (pos != NULL )
	{
		int nItem = ctlList.GetNextSelectedItem(pos);
		if(nItem > -1)
		{
			CALDoc* pDoc = GetDocument();
			ASSERT_VALID(pDoc);

			if (!pDoc->m_listItems.IsEmpty())
			{
				PITEMDATA pData = NULL;

				DWORD nIndex = (DWORD) ctlList.GetItemData(nItem);
				POSITION pos = pDoc->m_listItems.FindIndex(nIndex);

				if( pos != NULL)
				{
					pData = pDoc->m_listItems.GetAt(pos);

					if(pData)
						CopyToClipboard(pData->sData);
				}
			}
		}
	}
}

int CLeftView::CopyToClipboard(CString xData)
{
	#ifdef UNICODE
			UINT nFormat = CF_UNICODETEXT;
	#else
			UINT nFormat = CF_TEXT;
	#endif

	if (OpenClipboard())
	{
		EmptyClipboard();

		int cch = xData.GetLength();

		// Note: Clipboard only accepts data that is in a block allocated with 
		//       GlobalAlloc using the GMEM_MOVEABLE and GMEM_DDESHARE flags.
		HGLOBAL hClipData = GlobalAlloc(GMEM_MOVEABLE | GMEM_DDESHARE, sizeof(TCHAR) * ( cch + 1 ));
		if(hClipData)
		{
			LPTSTR pClipData = (LPTSTR) GlobalLock(hClipData);

			memcpy(pClipData, xData.GetBuffer(), sizeof(TCHAR) * cch ); 
			pClipData[cch] = (TCHAR) 0;    // null character 
			GlobalUnlock(hClipData); 

			if( ::SetClipboardData(CF_UNICODETEXT, hClipData) == NULL )
			{
				GlobalFree(hClipData);
				AfxMessageBox(_T("Error putting text on the clipboard"), MB_OK | MB_ICONINFORMATION);
			}
		}

		CloseClipboard();
	}
	return 0;
}

int CLeftView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO:  Add your specialized creation code here
	CALDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	//::InterlockedExchangePointer(&pDoc->m_hLeftView, this->m_hWnd);
	pDoc->m_hLeftView = this->m_hWnd;

	// create icons for view
	m_ImgList.Create(16, 16, ILC_COLOR, 0, 1);
	m_ImgList.SetBkColor(GetSysColor(COLOR_WINDOW));
	
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_TEXT_FILE)); 
	m_ImgList.Add(AfxGetApp()->LoadIcon(IDI_XML_FILE)); 

	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	ctlList.SetImageList (&m_ImgList, LVSIL_SMALL);

	ctlList.SetExtendedStyle(LVS_EX_FULLROWSELECT); //  | LVS_EX_GRIDLINES );

	ctlList.InsertColumn(0, _T("Label"),LVCFMT_LEFT,140, 0);
	ctlList.InsertColumn(1, _T("Time"), LVCFMT_LEFT,50, 0);

	return 0;
}

LRESULT CLeftView::OnItemData(WPARAM wParam, LPARAM lParam)
{
	PITEMDATA pData = (PITEMDATA) lParam;

	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();

	CALDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	pDoc->m_listItems.AddTail(pData);

	int nItem = (int) pDoc->m_listItems.GetCount() - 1;
	ctlList.InsertItem(nItem, pData->sLabel, pData->nType);
    
    TCHAR szLocalTime[255];
	GetTimeFormat( LOCALE_USER_DEFAULT, 0, &pData->stTime, NULL, szLocalTime, 255 );
    ctlList.SetItemText(nItem, 1, szLocalTime);

	ctlList.SetItemData(nItem, (DWORD) nItem );

	return 0L;
}

CString CLeftView::GetHeaderState(void)
{
	CString szHeader;
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	CHeaderCtrl* pHeaderCtrl = ctlList.GetHeaderCtrl();

	HDITEM hdi;
	int   nCount = pHeaderCtrl->GetItemCount();
	for(int i = 0; i < nCount; i++)
	{
		hdi.mask = HDI_WIDTH;
		pHeaderCtrl->GetItem(i, &hdi);
		CString szWidth;
		szWidth.Format( _T("%d,"), hdi.cxy);
		szHeader.Append( szWidth ) ;
	}

	return szHeader;
}

void CLeftView::RestoreHeaderState(CString szHeader)
{
	CListCtrl& ctlList = (CListCtrl&) GetListCtrl();
	CHeaderCtrl* pHeaderCtrl = ctlList.GetHeaderCtrl();

	HDITEM hdi;
	int nNewPos, nPos = 0;
	int   nCount = pHeaderCtrl->GetItemCount();
	for(int i = 0; i < nCount; i++)
	{
		nNewPos = szHeader.Find(_T(","), nPos);
		if( nNewPos != 0)
		{
			CString szWidth = szHeader.Mid(nPos,nNewPos-nPos);
			int nWidth = _ttoi( szWidth );
			if( nWidth != 0 )
			{
				hdi.mask = HDI_WIDTH;
				pHeaderCtrl->GetItem(i, &hdi);
				hdi.cxy = nWidth;
				pHeaderCtrl->SetItem(i, &hdi);
				nPos = nNewPos + 1;
			}
		}
	}
}

// Resize columns in a header
void CLeftView::HeaderAutoFit(int nItem, bool change)
{
	CListCtrl& listctrl = GetListCtrl();
	CHeaderCtrl* pCtrl = listctrl.GetHeaderCtrl();
	
	if(pCtrl != NULL)
	{
		ASSERT(pCtrl != NULL);

		HD_ITEM hi;
		CRect rClient;
		GetClientRect(&rClient); // size without vscroll and hscroll bars

		int nItemCount = pCtrl->GetItemCount();
		int nHBoarder = ::GetSystemMetrics(SM_CXBORDER);

		//CScrollBar* pBar;
		//DWORD dwStyle = GetStyle();
		//int nScrollWidth = ::GetSystemMetrics(SM_CXVSCROLL);

		////If no scroll bars, set width to zero
		//pBar = GetScrollBarCtrl(SB_VERT);
		//if ((pBar != NULL && !pBar->IsWindowEnabled()) ||
		//	(pBar == NULL && !(dwStyle & WS_VSCROLL)))
		//		nScrollWidth = 0;

		//int t_width = 0;
		//
		//hi.mask = HDI_WIDTH;
		//pCtrl->GetItem(0, &hi);
		//t_width = hi.cxy;

		//hi.mask = HDI_WIDTH;
		//pCtrl->GetItem(1, &hi);
		//t_width += hi.cxy;

		hi.mask = HDI_WIDTH;
		pCtrl->GetItem(nItem, &hi);
		hi.mask = HDI_WIDTH;
		int width = hi.cxy;
		hi.cxy = rClient.Width() - hi.cxy + nHBoarder - 1;
		//TRACE("change %d, width %d, t_width %d, ITEM %d, WIDTH %d, O_ITEM %d, O_WIDTH %d\n", (change == true?1:0),  rClient.Width(), t_width, nItem, width, (nItem==0?1:0), hi.cxy);
		if (change == true) pCtrl->SetItem((nItem==0?1:0), &hi);
	}
}

void CLeftView::OnHdnEndtrack(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	HeaderAutoFit(phdr->iItem);
	*pResult = 0;
}

void CLeftView::OnSize(UINT nType, int cx, int cy)
{
	CListView::OnSize(nType, cx, cy);
	HeaderAutoFit(1);
}

void CLeftView::OnHdnDividerdblclick(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
	// TODO: Add your control notification handler code here
	::SendMessage(this->m_hWnd, LVM_SETCOLUMNWIDTH, phdr->iItem, LVSCW_AUTOSIZE);
	HeaderAutoFit(phdr->iItem, true);
	*pResult = 0;
}
