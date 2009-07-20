//
// based on code taken from
// http://msdn2.microsoft.com/en-us/library/ms753125.aspx
//

#pragma once

//implement MemoryStream that derives from IStream
class MemoryStream : public IStream
{
	MemoryStream(LPTSTR lpszMemData, ULONG nMax): _lpszMemData(NULL)
    { 
        _refcount = 1; 
        _lpszMemData = lpszMemData;
    }

    ~MemoryStream() 
    { 
        if (_lpszMemData != NULL)
        {
			delete [] _lpszMemData;
        }
    }

public:
    HRESULT static Initialize(LPCTSTR lpszData, IStream ** ppStream)
    {
		ULONG nMax = lstrlen(lpszData);
		LPTSTR lpszMemData = new TCHAR[nMax];

        if (lpszMemData == NULL)
            return HRESULT_FROM_WIN32(GetLastError());
		else
		{
			lstrcpy(lpszMemData, lpszData);
		}
        
        *ppStream = new MemoryStream(lpszMemData);
        
        if(*ppStream == NULL)
            delete(pBuffer);
            
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE QueryInterface(REFIID iid, void ** ppvObject)
    { 
        if (iid == __uuidof(IUnknown)
            || iid == __uuidof(IStream)
            || iid == __uuidof(ISequentialStream))
        {
            *ppvObject = static_cast<IStream*>(this);
            AddRef();
            return S_OK;
        } else
            return E_NOINTERFACE; 
    }

    virtual ULONG STDMETHODCALLTYPE AddRef(void) 
    { 
        return (ULONG)InterlockedIncrement(&_refcount); 
    }

    virtual ULONG STDMETHODCALLTYPE Release(void) 
    {
        ULONG res = (ULONG) InterlockedDecrement(&_refcount);
        if (res == 0) 
            delete this;
        return res;
    }

    // ISequentialStream Interface
public:
    virtual HRESULT STDMETHODCALLTYPE Read(void* pv, ULONG cb, ULONG* pcbRead)
    {
        //BOOL rc = ReadFile(_hFile, pv, cb, pcbRead, NULL);
		LPTSTR lpszLoc = lstrcpyn( pv, _lpszMemData, cb);
        return ( lpszLoc != NULL) ? S_OK : HRESULT_FROM_WIN32(GetLastError());
    }

    virtual HRESULT STDMETHODCALLTYPE Write(void const* pv, ULONG cb, ULONG* pcbWritten)
    {
		// bug in sax writer
		if( pcbWritten == NULL ) return S_OK;
        //BOOL rc = WriteFile(_hFile, pv, cb, pcbWritten, NULL);
		// TODO
        return rc ? S_OK : HRESULT_FROM_WIN32(GetLastError());
    }

    // IStream Interface
public:
    virtual HRESULT STDMETHODCALLTYPE SetSize(ULARGE_INTEGER)
    { 
        return E_NOTIMPL;   
    }
    
    virtual HRESULT STDMETHODCALLTYPE CopyTo(IStream*, ULARGE_INTEGER, ULARGE_INTEGER*,
        ULARGE_INTEGER*) 
    { 
        return E_NOTIMPL;   
    }
    
    virtual HRESULT STDMETHODCALLTYPE Commit(DWORD)                                      
    { 
        return E_NOTIMPL;   
    }
    
    virtual HRESULT STDMETHODCALLTYPE Revert(void)                                       
    { 
        return E_NOTIMPL;   
    }
    
    virtual HRESULT STDMETHODCALLTYPE LockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)              
    { 
        return E_NOTIMPL;   
    }
    
    virtual HRESULT STDMETHODCALLTYPE UnlockRegion(ULARGE_INTEGER, ULARGE_INTEGER, DWORD)            
    { 
        return E_NOTIMPL;   
    }
    
    virtual HRESULT STDMETHODCALLTYPE Clone(IStream **)                                  
    { 
        return E_NOTIMPL;   
    }

    virtual HRESULT STDMETHODCALLTYPE Seek(LARGE_INTEGER liDistanceToMove, DWORD dwOrigin,
        ULARGE_INTEGER* lpNewFilePointer)
    { 
        DWORD dwMoveMethod;

        switch(dwOrigin)
        {
        case STREAM_SEEK_SET:
            dwMoveMethod = FILE_BEGIN;
            break;
        case STREAM_SEEK_CUR:
            dwMoveMethod = FILE_CURRENT;
            break;
        case STREAM_SEEK_END:
            dwMoveMethod = FILE_END;
            break;
        default:   
            return STG_E_INVALIDFUNCTION;
            break;
        }

        //if (SetFilePointerEx(_hFile, liDistanceToMove, (PLARGE_INTEGER) lpNewFilePointer,
        //                     dwMoveMethod) == 0)
            return HRESULT_FROM_WIN32(GetLastError());
        return S_OK;
    }

    virtual HRESULT STDMETHODCALLTYPE Stat(STATSTG* pStatstg, DWORD grfStatFlag) 
    {
        //if (GetFileSizeEx(_hFile, (PLARGE_INTEGER) &pStatstg->cbSize) == 0)
        //    return HRESULT_FROM_WIN32(GetLastError());
        (PLARGE_INTEGER) &pStatstg->cbSize = lstrlen(_lpszMemData);
		return S_OK;
    }

private:
	LPTSTR _lpszMemData;
	CString _sData;
    LONG	_refcount;
};
