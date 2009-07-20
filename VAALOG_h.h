

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 6.00.0366 */
/* at Sun Jul 19 21:27:06 2009
 */
/* Compiler settings for .\VAALOG.idl:
    Oicf, W1, Zp8, env=Win32 (32b run)
    protocol : dce , ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
//@@MIDL_FILE_HEADING(  )

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __VAALOG_h_h__
#define __VAALOG_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IActiveLog_FWD_DEFINED__
#define __IActiveLog_FWD_DEFINED__
typedef interface IActiveLog IActiveLog;
#endif 	/* __IActiveLog_FWD_DEFINED__ */


#ifndef __ActiveLog_FWD_DEFINED__
#define __ActiveLog_FWD_DEFINED__

#ifdef __cplusplus
typedef class ActiveLog ActiveLog;
#else
typedef struct ActiveLog ActiveLog;
#endif /* __cplusplus */

#endif 	/* __ActiveLog_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 

void * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void * ); 


#ifndef __VAALOG_LIBRARY_DEFINED__
#define __VAALOG_LIBRARY_DEFINED__

/* library VAALOG */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_VAALOG;

#ifndef __IActiveLog_DISPINTERFACE_DEFINED__
#define __IActiveLog_DISPINTERFACE_DEFINED__

/* dispinterface IActiveLog */
/* [uuid] */ 


EXTERN_C const IID DIID_IActiveLog;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("F5BB9079-4A35-414C-A785-E774BDBB17EE")
    IActiveLog : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IActiveLogVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IActiveLog * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IActiveLog * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IActiveLog * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IActiveLog * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IActiveLog * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IActiveLog * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IActiveLog * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } IActiveLogVtbl;

    interface IActiveLog
    {
        CONST_VTBL struct IActiveLogVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IActiveLog_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IActiveLog_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IActiveLog_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IActiveLog_GetTypeInfoCount(This,pctinfo)	\
    (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo)

#define IActiveLog_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo)

#define IActiveLog_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)

#define IActiveLog_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IActiveLog_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_ActiveLog;

#ifdef __cplusplus

class DECLSPEC_UUID("0ACCCD94-CC09-46C2-803D-CB303F25C583")
ActiveLog;
#endif
#endif /* __VAALOG_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


