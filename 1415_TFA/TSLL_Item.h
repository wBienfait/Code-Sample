#ifndef TSLL_H_INCLUDED
#define TSLL_H_INCLUDED

#include "Constantes.h"
#include "TXML_Root.h"

class TXML_Element;
template<class T>

class TSLLItem
{
protected:
	T * m_pNextItem = nullptr;

public:
	TSLLItem();
	~TSLLItem();

	char m_cName[MAX_CHAR];
	char m_cParentName[MAX_CHAR];


	T * FindItem(T * pItemToFind);

	void InsertAfter(T * pNewItem);
	void DestroyNextItem();
	int GetCount();
};

template<class T>
TSLLItem<T>::TSLLItem()
{
	m_pNextItem = nullptr;
	m_cName[0] = '\0';
	m_cParentName[0] = '\0';
}

template<class T>
TSLLItem<T>::~TSLLItem()
{
}
template<class T>
void TSLLItem<T>::InsertAfter(T * pNewItem)
{
	pNewItem->m_pNextItem = this->m_pNextItem;
	this->m_pNextItem = pNewItem;

}
template<class T>
int TSLLItem<T>::GetCount()
{
	TSLLItem * pActItem = this;
	int iCount = 1;

	while (pActItem->m_pNextItem != nullptr)
	{
		iCount++;
		pActItem = pActItem->m_pNextItem;
	}

	return iCount;
}
template<class T>
T * TSLLItem<T>::FindItem(T * pItemToFind)
{
	TSLLItem * pActItem = this;

	while (pActItem != pItemToFind)
	{
		pActItem = pActItem->m_pNextItem;

		if (pActItem == nullptr)
		{
			return nullptr;
		}
	}

	return pActItem;
}
template<class T>
void TSLLItem<T>::DestroyNextItem()
{
	TXML_Element * pNextItem = m_pNextItem->m_pNextItem;
	delete this->m_pNextItem;
	this->m_pNextItem = pNextItem;
}


template<class T>

class TSLLRoot
{
	T * m_pFirstItem = nullptr;

	T * FindLastItem();

public:
	TSLLRoot();
	~TSLLRoot();

	void Append(T * pNewItem);
	bool RemoveItem(T * ItemToRemove);
	T * GetLastItem();
	T * GetFirstItem();

};



template<class T>
TSLLRoot<T>::TSLLRoot()
{

}
template<class T>
TSLLRoot<T>::~TSLLRoot()
{

}
template<class T>
T * TSLLRoot<T>::FindLastItem()
{
	if (this->m_pFirstItem == nullptr)
	{
		return nullptr;
	}
	else
	{
		T * pActItem = this->m_pFirstItem;

		while (pActItem->FindNextItem() != nullptr)
		{
			pActItem = pActItem->FindNextItem();
		}

		return pActItem;
	}
}
template<class T>
void TSLLRoot<T>::Append(T * pNewItem)
{
	T * pLastItem = FindLastItem();

	if (pLastItem == nullptr)
	{
		this->m_pFirstItem = pNewItem;
	}
	else
	{
		pLastItem->InsertAfter(pNewItem);
	}
}
template<class T>
bool TSLLRoot<T>::RemoveItem(T * pItemToRemove)
{
	T * pItemBefore = this->m_pFirstItem->FindItem(pItemToRemove);

	if (pItemBefore == nullptr)
	{
		return false;
	}
	else
	{
		pItemBefore->DestroyNextItem();
		return true;
	}
}
template<class T>
T * TSLLRoot<T>::GetLastItem()
{
	return this->FindLastItem();
}
template<class T>
T * TSLLRoot<T>::GetFirstItem()
{
	return this->m_pFirstItem;
}

#endif

