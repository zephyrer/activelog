#pragma once

class CRegUser
{
public:
	CRegUser(void);
	~CRegUser(void);

public:
	int CheckUserKey(CString szKey, LPCTSTR szUser);
	int CheckExpiery(CString szKey);
};
