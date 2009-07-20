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

// http://support.microsoft.com/kb/161873

      #include "comcat.h"

      // Helper function to create a component category and associated
      // description
      HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription)
      {
         ICatRegister* pcr = NULL ;
         HRESULT hr = S_OK ;

         hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
                               NULL,
                               CLSCTX_INPROC_SERVER,
                               IID_ICatRegister,
                               (void**)&pcr);
         if (FAILED(hr))
            return hr;

         // Make sure the HKCR\Component Categories\{..catid...}
         // key is registered
         CATEGORYINFO catinfo;
         catinfo.catid = catid;
         catinfo.lcid = 0x0409 ; // english

         // Make sure the provided description is not too long.
         // Only copy the first 127 characters if it is
         //int len = wcslen(catDescription);
         size_t len = wcslen(catDescription);
         if (len>127)
            len = 127;
         //wcsncpy(catinfo.szDescription, catDescription, len);
         wcsncpy_s(catinfo.szDescription, catDescription, len);
         // Make sure the description is null terminated
         catinfo.szDescription[len] = '\0';

         hr = pcr->RegisterCategories(1, &catinfo);
         pcr->Release();

         return hr;
      }

      // Helper function to register a CLSID as belonging to a component
      // category
      HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid)
      {
         // Register your component categories information.
         ICatRegister* pcr = NULL ;
         HRESULT hr = S_OK ;
         hr = CoCreateInstance(CLSID_StdComponentCategoriesMgr,
                               NULL,
                               CLSCTX_INPROC_SERVER,
                               IID_ICatRegister,
                               (void**)&pcr);
         if (SUCCEEDED(hr))
         {
            // Register this category as being "implemented" by
            // the class.
            CATID rgcatid[1] ;
            rgcatid[0] = catid;
            hr = pcr->RegisterClassImplCategories(clsid, 1, rgcatid);
         }

         if (pcr != NULL)
            pcr->Release();

         return hr;
      }
