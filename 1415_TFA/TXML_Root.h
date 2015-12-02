#ifndef TXML_H_INCLUDED
#define TXML_H_INCLUDED

#include "TSLL_Item.h"
#include "flib.h"


class TXML_Attributes : public TSLLItem<TXML_Attributes>
{
public:

	TXML_Attributes();
	~TXML_Attributes();

	char m_cAttName[MAX_CHAR];
	char m_cAttValue[MAX_CHAR];

	TSLLRoot<TXML_Attributes> * m_pAttributesList;

	TXML_Attributes * FindNextItem();
	void Print();
};

class TXML_Element : public TSLLItem<TXML_Element>
{
public:
	TXML_Element();
	~TXML_Element();

	char m_cElementName[MAX_CHAR];
	char m_cElementValue[MAX_CHAR];

	TSLLRoot<TXML_Attributes> * m_pAttributesList;
	TSLLRoot<TXML_Element> * m_pElementsChildList;

	TXML_Element * FindNextItem();
	TXML_Element * FindFirstSiblingElement(char * pNameFile, TXML_Element * pElement);
	TXML_Element * FindNextSiblingElement(char * pNameFile);
	void Print();
};

class TXML_Root : public TSLLItem<TXML_Root>
{
public:
	TXML_Root();
	~TXML_Root();

	char * FindBalise(char * pBuffer, TXML_Element * pParent);
	void findName(char * pBegin, TXML_Element * pElement);
	void FindAttribute(char * pBegin, TXML_Element * pElement);

	TSLLRoot<TXML_Element> * m_pRootElementList;

	void ParseXml(char * pName);
	void DebugList(TXML_Element * pElement);
};




#endif

