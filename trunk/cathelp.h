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

// http://support.microsoft.com/kb/161873

      #include "comcat.h"

      // Helper function to create a component category and associated
      // description
      HRESULT CreateComponentCategory(CATID catid, WCHAR* catDescription);

      // Helper function to register a CLSID as belonging to a component
      // category
      HRESULT RegisterCLSIDInCategory(REFCLSID clsid, CATID catid);
						