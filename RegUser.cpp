#include "StdAfx.h"

// Crypto++ Includes
#include "cryptlib.h"
#include "aes.h"        // AES
#include "modes.h"      // CBC_Mode< >
#include "filters.h"    // StringSource
#include "osrng.h"      // PRNG
#include "Base32.h"     // Base32

#include "RegUser.h"

CRegUser::CRegUser(void)
{
}

CRegUser::~CRegUser(void)
{
}

#define PREPARED_NAME_LEN  15

void PrepareName(
	unsigned char* pucPrepNameArea,  /* must be PREPARED_NAME_LEN bytes */
	const char*    szName)
{
	const unsigned char  ToUpperTable[256] =
		{ 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
		  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F,
		  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F,
		  0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B, 0x3C, 0x3D, 0x3E, 0x3F,
		  0x40,  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O',
		   'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z', 0x5B, 0x5C, 0x5D, 0x5E, 0x5F,
		  0x60,  'A',  'B',  'C',  'D',  'E',  'F',  'G',  'H',  'I',  'J',  'K',  'L',  'M',  'N',  'O', /* 'a' -> 'A', ... */
		   'P',  'Q',  'R',  'S',  'T',  'U',  'V',  'W',  'X',  'Y',  'Z', 0x7B, 0x7C, 0x7D, 0x7E, 0x7F,
		  0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8A, 0x8B, 0x8C, 0x8D, 0x8E, 0x8F,
		  0x90, 0x91, 0x92, 0x93, 0x94, 0x95, 0x96, 0x97, 0x98, 0x99, 0x9A, 0x9B, 0x9C, 0x9D, 0x9E, 0x9F,
		  0xA0, 0xA1, 0xA2, 0xA3, 0xA4, 0xA5, 0xA6, 0xA7, 0xA8, 0xA9, 0xAA, 0xAB, 0xAC, 0xAD, 0xAE, 0xAF,
		  0xB0, 0xB1, 0xB2, 0xB3, 0xB4, 0xB5, 0xB6, 0xB7, 0xB8, 0xB9, 0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF,
		  0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC6, 0xC7, 0xC8, 0xC9, 0xCA, 0xCB, 0xCC, 0xCD, 0xCE, 0xCF,
		  0xD0, 0xD1, 0xD2, 0xD3, 0xD4, 0xD5, 0xD6, 0xD7, 0xD8, 0xD9, 0xDA, 0xDB, 0xDC, 0xDD, 0xDE, 0xDF,
		  0xE0, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5, 0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xEE, 0xEF,
		  0xF0, 0xF1, 0xF2, 0xF3, 0xF4, 0xF5, 0xF6, 0xF7, 0xF8, 0xF9, 0xFA, 0xFB, 0xFC, 0xFD, 0xFE, 0xFF };

	const char*  pcCurr;
	int          i;

//	for (i=0; i<PREPARED_NAME_LEN; i++)
//		pucPrepNameArea[i] = (unsigned char) i;

	i = 0;
	for (pcCurr = szName; *pcCurr && i < PREPARED_NAME_LEN ; pcCurr++)
	{
		if (*pcCurr == ' ')
			continue;
		// warning C6385: Invalid data: accessing 'ToUpperTable', the readable size is '256' bytes, but '-24' bytes might be read: Lines: 27, 45, 46, 51, 52, 54, 56
		pucPrepNameArea[i++] = ToUpperTable[*pcCurr];
	}
}

int CRegUser::CheckUserKey(CString szKey, LPCTSTR szUser)
{
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszAnsiKey (szKey);

    // Recovered Text Sink
    //std::string EncodedText   = szKey;
    std::string EncodedText(pszAnsiKey);

	CT2CA pszAnsiUser (szUser);
	//std::string PlainTextIn( szUser );
	std::string PlainTextIn( pszAnsiUser );
	BYTE sThis[16];
	memset(sThis,0,16);
	PrepareName( sThis ,PlainTextIn.c_str());
	std::string UserText( (char*) sThis );
    std::string RecoveredText = "";

	byte key[ CryptoPP::AES::DEFAULT_KEYLENGTH ] =
		{ 0xD8, 0xD8, 0x0D, 0xAF, 0x97, 0x9D, 0x4A, 0x20, 
		  0x79, 0x66, 0xCC, 0x6B, 0x1D, 0xB6, 0xB4, 0xA1 };

	byte  iv[ CryptoPP::AES::BLOCKSIZE ] =
		{ 0x32, 0xB3, 0x02, 0x32, 0x81, 0xA3, 0xB9, 0x7B, 
		  0x5A, 0xB6, 0x79, 0x25, 0xB5, 0xAB, 0x6D, 0xEB };

	try
	{

		// Pseudo Random Number Generator
		//CryptoPP::AutoSeededRandomPool rng;

		// Decryptor
		CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption
			Decryptor( key, sizeof(key), iv );

		CryptoPP::StringSource( EncodedText, true,
			new CryptoPP::Base32Decoder(
				new CryptoPP::StreamTransformationFilter( Decryptor,
					new CryptoPP::StringSink( RecoveredText )
				) // StreamTransformationFilter
			) // Base32Decoder
		); // StringSource
	    
		return ( UserText.compare( RecoveredText ));
	}
	catch(CryptoPP::Exception &ce)
	{
		//cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		TRACE1("CryptoPP::Exception caught: %s" , ce.what());
		return -1;
	}
	catch(std::exception &se)
	{
		//cout << "\nstd::exception caught: " << e.what() << endl;
		TRACE1("std::exception caught:  %s" , se.what());
		return -2;
	}
	catch(...)
	{
		TRACE("unknown exception");
		return -3;
	}

}

int CRegUser::CheckExpiery(CString szKey)
{
	// Timer unit is 100-nanoseconds.
	const int nTimerUnitsPerSecond	= 10000000;
	const int nSecInOneDay			= 24 * 60 * 60;			
	// v0.09.02 - expiery changed from 10 to 15 days
	const int nSecInExpiery			= 15 * nSecInOneDay;	// EXPIERY PERIOD - 15 days

	SYSTEMTIME		st, stNow;
	FILETIME		ftInstall, ftNow, ftExpiry;
	LARGE_INTEGER	liInstall, liNow, liExpiry;
    
	// Convert a TCHAR string to a LPCSTR
	CT2CA pszAnsiKey (szKey);

	// Recovered Text Sink
    //std::string EncodedText   = szKey;
    std::string EncodedText (pszAnsiKey);
    std::string RecoveredText = "";

	BYTE key[ CryptoPP::AES::DEFAULT_KEYLENGTH ] =
        { 0xA2, 0xC6, 0x08, 0x1B, 0xFE, 0xF4, 0x9F, 0x0F, 
		  0xC3, 0x78, 0x01, 0x74, 0x83, 0x3A, 0x57, 0x41 };

	BYTE  iv[ CryptoPP::AES::BLOCKSIZE ] =
        { 0x79, 0xEA, 0x50, 0x87, 0x80, 0x3F, 0x21, 0xE7, 
		  0xCD, 0x12, 0x7B, 0x10, 0x30, 0x17, 0x3D, 0x43 };

	try
	{
		// Decryptor
		CryptoPP::CBC_Mode<CryptoPP::AES>::Decryption
			Decryptor( key, sizeof(key), iv );

		CryptoPP::StringSource( EncodedText, true,
			new CryptoPP::Base32Decoder(
				new CryptoPP::StreamTransformationFilter( Decryptor,
					new CryptoPP::StringSink( RecoveredText )
				) // StreamTransformationFilter
			) // Base32Decoder
		); // StringSource
	    
		if ( (RecoveredText.length () == 15) || ( RecoveredText.length () == 20 ) )
		{
			// TODO ADD SALT

			// YYYYMMDD HHMMSS
			// 20061230 040511			
			st.wYear			= atoi(RecoveredText.substr(0,4).c_str());		// Year
			st.wMonth			= atoi(RecoveredText.substr(4,2).c_str());		// January
			st.wDayOfWeek		= 0;											// Ignored
			st.wDay				= atoi(RecoveredText.substr(6,2).c_str());		// The first of the month
			st.wHour			= atoi(RecoveredText.substr(9,2).c_str());		// 1PM
			st.wMinute			= atoi(RecoveredText.substr(11,2).c_str());	// minutes into the hour
			st.wSecond			= atoi(RecoveredText.substr(13,2).c_str());	// seconds into the minute
			st.wMilliseconds	= 0;											// 0 milliseconds into the second
				
			// Convert to File Time	
			SystemTimeToFileTime(&st, &ftInstall);
						
			// Convert FILETIME to LARGE_INTEGER because of different alignment.
			liInstall.LowPart	= ftInstall.dwLowDateTime;
			liInstall.HighPart	= ftInstall.dwHighDateTime;

			// It is not recommended that you add and subtract values from the SYSTEMTIME structure 
			// to obtain relative times. Instead, you should 

			// - Convert the SYSTEMTIME structure to a FILETIME structure. 
			// - Copy the resulting FILETIME structure to a ULARGE_INTEGER structure. 
			// - Use normal 64-bit arithmetic on the ULARGE_INTEGER value. 

			liExpiry.QuadPart = liInstall.QuadPart + Int32x32To64( nSecInExpiery, nTimerUnitsPerSecond );

			// Convert FILETIME to LARGE_INTEGER because of different alignment.
			ftExpiry.dwLowDateTime  = liExpiry.LowPart;
			ftExpiry.dwHighDateTime = liExpiry.HighPart;

			// get current time 
			GetLocalTime ( &stNow );
			
			// GET CURRENT TIME AS LARGE INTEGER	
			SystemTimeToFileTime(&stNow, &ftNow);

			// CompareFileTime return value
			// Value Meaning 
			// –1 First file time is less than second file time. 
			// 0 First file time is equal to second file time. 
			// 1 First file time is greater than second file time. 
			if( CompareFileTime (&ftNow, &ftExpiry) == 1 )
			{
				return (-3);
			}

			// Convert FILETIME to LARGE_INTEGER because of different alignment.
			liNow.LowPart	= ftNow.dwLowDateTime;
			liNow.HighPart	= ftNow.dwHighDateTime;
			
			// return number of days left in evaluation perion
			// warning C4244: 'initializing' : conversion from '__int64' to 'int', possible loss of data
			int nDaysLeft = (liExpiry.QuadPart - liNow.QuadPart) / 
				Int32x32To64(nSecInOneDay,nTimerUnitsPerSecond);

			// return number of days left to expiery
			return nDaysLeft;

			// inside evaluation period
			// return 0;
		}

		return (-4);
	}
	catch(CryptoPP::Exception &ce)
	{
		//cout << "\nCryptoPP::Exception caught: " << e.what() << endl;
		TRACE1("CryptoPP::Exception caught: %s" , ce.what());
		return -1;
	}
	catch(std::exception &se)
	{
		//cout << "\nstd::exception caught: " << e.what() << endl;
		TRACE1("std::exception caught:  %s" , se.what());
		return -2;
	}
	catch(...)
	{
		TRACE("unknown exception");
		return -3;
	}

}
