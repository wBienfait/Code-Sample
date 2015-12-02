#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED

#include "flib.h"
#include "menu_option.h"
#include "vector.h"

class TMenu
{
public:

	TMenu( const char * pName );

	void AddOption( const char * pText, const float fValue, const float fMinValue, const float fMaxValue, const float fIncrement = 1 );

	int GetInt( int iIndex ) const;
	float GetFloat( int iIndex ) const;
	const char * GetName() const;

	bool Update();
	void Render();

private:

	TMenu & operator=( const TMenu & );

	Vector< TMenuOption > m_tOptions;
	int m_iSelection;
	const char * m_pName;
};

#endif
