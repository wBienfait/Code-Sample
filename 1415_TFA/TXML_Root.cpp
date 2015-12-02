#include "TXML_Root.h"
#include <cstring>
#include <ctype.h>


TXML_Root::TXML_Root()
{
	m_pRootElementList = new TSLLRoot<TXML_Element>;
}


TXML_Root::~TXML_Root()
{
}

void TXML_Root::DebugList(TXML_Element * pElement)
{
	TXML_Element * pActItem = pElement;


	while (pActItem != nullptr)
	{
		pActItem->Print();

		TXML_Attributes * pAttributes = static_cast<TXML_Attributes *>(pActItem->m_pAttributesList->GetFirstItem());

		while (pAttributes != nullptr)
		{
			pAttributes->Print();
			pAttributes = pAttributes->FindNextItem();
		}

		TXML_Element * pChild = static_cast<TXML_Element *>(pActItem->m_pElementsChildList->GetFirstItem());
		if (pChild != nullptr)
		{
			//pChild->Print();
			DebugList(pChild);
			//pChild = pChild->FindNextItem();

		}


		pActItem = pActItem->FindNextItem();

	}
	return;
}


void TXML_Root::FindAttribute(char * pBegin, TXML_Element * pElement)
{
	int iCount = 0;
	char * pActPosition = pBegin;
	TXML_Attributes * pNewAttributes = new TXML_Attributes;
	for (;;)
	{
		if (pActPosition[iCount] != ' ')
		{

			char * pBeginName = pActPosition + iCount;
			char * pEndName = strchr(pBeginName, ' ');
			char * pEndName2 = strchr(pBeginName, '=');
			if (pEndName2 < pEndName)
			{
				pEndName = pEndName2;
			}

			const int iNameSize = pEndName - pBeginName;
			strncpy(pNewAttributes->m_cAttName, pBeginName, iNameSize);
			pNewAttributes->m_cAttName[iNameSize] = 0;
			pActPosition = strchr(pActPosition, '=');
			break;
		}
		iCount++;

	}
	iCount = 1;

	for (;;)
	{
		if (pActPosition[iCount] == '"' || pActPosition[iCount] == '\'')
		{
			char * pBeginName = pActPosition + iCount + 1;
			char * pEndName = strchr(pBeginName, '"');
			char * pEndName2 = strchr(pBeginName, '\'');
			if ((pEndName > pEndName2&& pEndName2 != nullptr) || pEndName == nullptr)
			{
				pEndName = pEndName2;
			}

			const int iNameSize = pEndName - pBeginName;
			strncpy(pNewAttributes->m_cAttValue, pBeginName, iNameSize);
			pNewAttributes->m_cAttValue[iNameSize] = 0;

			strncpy(pNewAttributes->m_cName, pBeginName, iNameSize);
			pNewAttributes->m_cName[iNameSize] = 0;

			pActPosition = pEndName + 1;
			pElement->m_pAttributesList->Append(pNewAttributes);
			//GfxDbgPrintf("Attribute Name : %s Value : %s ", pNewAttributes->m_cName, pNewAttributes->m_cAttValue);
			bool bNum = true;
			for (int i = 0; i < iNameSize; i++)
			{
				int bDigit = isdigit(pNewAttributes->m_cAttValue[i]);
				int bAlnum = isalnum(pNewAttributes->m_cAttValue[i]);
				int bIsPunct = ispunct(pNewAttributes->m_cAttValue[i]);
				if (bDigit == 0 && bAlnum != 0)
				{
					bNum = false;
					break;
				}
				if (bIsPunct != 0 && pNewAttributes->m_cAttValue[i] != ',' && pNewAttributes->m_cAttValue[i] != '.')
				{
					bNum = false;
					break;
				}
			}


			iCount = 0;

			if (strchr(pActPosition, '=') == nullptr || strchr(pActPosition, '=') > strchr(pActPosition, '>'))
			{
				return;
			}
			FindAttribute(pActPosition, pElement);
			return;
		}
		iCount++;
	}
}

void TXML_Root::findName(char * pBegin, TXML_Element * pElement)
{
	int iCount = 0;
	//char cName[MAX_CHAR];

	char * pActPosition = pBegin;

	for (;;)
	{
		if (pActPosition[iCount] != ' ' && pActPosition[iCount] != '/')
		{

			pActPosition = pActPosition + iCount;
			char * pEndName = strchr(pActPosition, ' ');
			char * pEndName2 = strchr(pActPosition, '>');

			if (pEndName > pEndName2 || pEndName == nullptr)
			{
				pEndName = pEndName2;
			}


			const int iNameSize = pEndName - pActPosition;
			//strncpy(cName, pActPosition, iNameSize);
			//cName[iNameSize] = 0;
			strncpy(pElement->m_cName, pActPosition, iNameSize);
			pElement->m_cName[iNameSize] = 0;

			strncpy(pElement->m_cElementName, pActPosition, iNameSize);
			pElement->m_cElementName[iNameSize] = 0;

			pActPosition = pEndName;
			//GfxDbgPrintf("Element %s\n", pElement->m_cName[iNameSize]);
			break;
		}
		iCount++;

	}

	iCount = 0;

	if (strchr(pActPosition, '=') != nullptr || strchr(pActPosition, '=') > strchr(pActPosition, '>'))
	{
		FindAttribute(pActPosition, pElement);
	}
}

char * TXML_Root::FindBalise(char * pBuffer, TXML_Element * pParent)
{
	char * pEndContent = pBuffer;
	for (;;)
	{
		char * pBegin = strchr(pEndContent, '<');
		char * pEnd = strchr(pEndContent, '>');

		TXML_Element * pNewElement = new TXML_Element;

		if (pBegin == nullptr || pEnd == nullptr)
		{
			//GfxDbgPrintf("return\n");
			return nullptr;
		}

		if (pEnd == nullptr)
		{
			GfxDbgPrintf("Bad Xml : unclosed balise");
		}


		if (pBegin[1] == '?' || pBegin[1] == '!')
		{
			pEndContent = pEnd + 1;
			delete pNewElement;
		}

		else if (pBegin[1] != '/' && pEnd[-1] != '/')
		{
			//GfxDbgPrintf("normale\n");

			findName(pBegin + 1, pNewElement);

			pParent->m_pElementsChildList->Append(pNewElement);
			for (int i = 0; i < MAX_CHAR; i++)
			{
				if (pParent == nullptr)
				{
					break;
				}

				pNewElement->m_cParentName[i] = pParent->m_cName[i];
				if (pParent->m_cName[i] == 0)
				{
					break;
				}
			}
			pEndContent = FindBalise(pEnd + 1, pNewElement);

		}

		if (pEnd[-1] == '/')
		{
			//GfxDbgPrintf("AutoFermante\n");
			findName(pBegin + 1, pNewElement);
			pParent->m_pElementsChildList->Append(pNewElement);

			for (int i = 0; i < MAX_CHAR; i++)
			{
				if (pParent == nullptr)
				{
					break;
				}

				pNewElement->m_cParentName[i] = pParent->m_cName[i];
				if (pParent->m_cName[i] == 0)
				{
					break;
				}
			}
			//GfxDbgPrintf("return\n");
			pEndContent = pEnd + 1;
			//pEndContent = FindBalise(pEnd + 1);

		}

		if (pBegin[1] == '/')
		{
			//GfxDbgPrintf("Fermante\n");
			findName(pBegin + 1, pNewElement);

			if (pParent->m_pElementsChildList->GetLastItem() == nullptr)
			{
				const int iValueSize = pBegin - pBuffer;
				strncpy(pParent->m_cElementValue, pBuffer, iValueSize);
				pParent->m_cElementValue[iValueSize] = 0;
			}

			if (*pNewElement->m_cName != *pParent->m_cName)
			{
				GfxDbgPrintf("Bad Xml : Not the Expected End\n");
			}
			delete pNewElement;
			//GfxDbgPrintf("return\n");
			return pEnd + 1;
			//pEndContent = FindBalise(pEnd + 1);

		}

		if (pEndContent == nullptr)
		{
			//GfxDbgPrintf("return\n");
			return pEndContent;
		}
	}

}

void TXML_Root::ParseXml(char * pName)
{
	TGfxFile * pFile = GfxFileOpenRead(pName);
	const int iSize = GfxFileSize(pFile);

	char * pBuffer = static_cast<char*>(GfxMemAlloc(iSize + 1));
	pBuffer[iSize] = 0;
	GfxFileRead(pFile, pBuffer, iSize);
	char * pActPosition = pBuffer;

	TXML_Element * pElement = new TXML_Element;
	this->m_pRootElementList->Append(pElement);

	pActPosition = FindBalise(pBuffer, static_cast<TXML_Element *>(this->m_pRootElementList->GetLastItem()));
	if (pActPosition != nullptr)
	{
		GfxDbgPrintf("not the end of xml");
	}
}

TXML_Element::TXML_Element()
{
	m_pAttributesList = new TSLLRoot<TXML_Attributes>;
	m_pElementsChildList = new TSLLRoot<TXML_Element>;
}

TXML_Element::~TXML_Element()
{
	m_cElementName[0] = '\0';
	m_cElementValue[0] = '\0';
}

void TXML_Element::Print()
{
	GfxDbgPrintf("Element : %s\n", m_cElementName);
	GfxDbgPrintf("Value : %s\n", m_cElementValue);
	GfxDbgPrintf("Parent : %s\n", m_cParentName);
}

TXML_Element * TXML_Element::FindNextItem()
{
	return static_cast<TXML_Element *>(this->m_pNextItem);
}

TXML_Element * TXML_Element::FindFirstSiblingElement(char * pNameFile, TXML_Element * pElement)
{
	TXML_Element * pActItem = pElement;
	TXML_Element * pItemFound = nullptr;
	//if (pActItem->m_cName[0] == 'p')
	//{
	//	TXML_Element * pNextSibling = pActItem->FindNextSiblingElement(pActItem->m_cName);
	//	TXML_Element * pNextSibling2 = pActItem->FindNextSiblingElement(pActItem->m_cName);
	//}

	while (pActItem != nullptr)
	{
		bool bSameName = true;

		for (int i = 0; i < MAX_CHAR; i++)
		{
			if (pActItem->m_cElementName[i] != pNameFile[i])
			{
				bSameName = false;
				break;
			}

			if (pActItem->m_cElementName[i] == 0)
			{
				break;
			}
		}
		if (bSameName == true)
		{
			return pActItem;
		}
		TXML_Element * pChild = static_cast<TXML_Element *>(pActItem->m_pElementsChildList->GetFirstItem());
		if (pChild != nullptr)
		{
			//pChild->Print();
			pItemFound = FindFirstSiblingElement(pNameFile, pChild);
			if (pItemFound != nullptr)
			{
				return pItemFound;
			}
			//pChild = pChild->FindNextItem();

		}


		pActItem = pActItem->FindNextItem();

	}
	return nullptr;
}

TXML_Element * TXML_Element::FindNextSiblingElement(char * pNameFile)
{

	TXML_Element * pNextItem = static_cast<TXML_Element *>(this->FindNextItem());
	while (pNextItem != nullptr)
	{
		for (int i = 0; i < MAX_CHAR; i++)
		{
			if (pNextItem->m_cElementName[i] == 0)
			{
				if (pNameFile[i] == 0)
				{
					return pNextItem;
				}
				pNextItem = pNextItem->FindNextItem();
				break;
			}
			if (pNextItem->m_cElementName[i] != pNameFile[i])
			{
				pNextItem = pNextItem->FindNextItem();
				break;
			}
		}
	}
	return nullptr;
}

TXML_Attributes::TXML_Attributes()
{
	m_pAttributesList = new TSLLRoot<TXML_Attributes>;
	m_cAttName[0] = '\0';
	m_cAttValue[0] = '\0';
}

TXML_Attributes::~TXML_Attributes()
{

}

TXML_Attributes * TXML_Attributes::FindNextItem()
{
	return static_cast<TXML_Attributes *>(this->m_pNextItem);
}

void TXML_Attributes::Print()
{
	GfxDbgPrintf("Attributes : %s\n", m_cAttName);
	GfxDbgPrintf("Value : %s\n", m_cAttValue);
	GfxDbgPrintf("Parent : %s\n", m_cParentName);
}
