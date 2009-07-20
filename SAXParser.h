#pragma once

class SAXParser
{
public:
	SAXParser(void);
	~SAXParser(void);
public:
	int ParseData(CString Data);
private:
    ISAXXMLReader* pReader;
};
