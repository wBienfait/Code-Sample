#include "menu.h"
#include <string.h>

const int TEXT_SPACING = 8;

TMenu::TMenu( const char * pName )
	: m_iSelection( -1 )
	, m_pName( pName )
{
}

void TMenu::AddOption( const char * pText, const float fValue, const float fMinValue, const float fMaxValue, const float fIncrement )
{
	TMenuOption tOption;

	tOption.m_pTextLabel = strdup( pText );
	tOption.m_pTextSprite = GfxTextSpriteCreate();
	tOption.m_fValue = fValue;
	tOption.m_fMinValue = fMinValue;
	tOption.m_fMaxValue = fMaxValue;
	tOption.m_fIncrement = fIncrement;

	m_tOptions.PushBack( tOption );
}

int TMenu::GetInt( int iIndex ) const
{
	GfxDbgAssert( iIndex >= 0 && iIndex < m_tOptions.GetCount() );
	return static_cast< int >( m_tOptions[ iIndex ].m_fValue );
}

float TMenu::GetFloat( int iIndex ) const
{
	GfxDbgAssert( iIndex >= 0 && iIndex < m_tOptions.GetCount() );
	return m_tOptions[ iIndex ].m_fValue;
}

const char * TMenu::GetName() const
{
	return m_pName;
}

bool TMenu::Update()
{
	if( m_tOptions.GetCount() > 0 )
	{
		const int iOldSelection = m_iSelection;

		if( GfxInputIsJustPressed( EGfxInputID_AnyPadDown ) || GfxInputIsJustPressed( EGfxInputID_KeyArrowDown ) )
		{
			m_iSelection++;
		}
		if( GfxInputIsJustPressed( EGfxInputID_AnyPadUp ) || GfxInputIsJustPressed( EGfxInputID_KeyArrowUp ) )
		{
			m_iSelection--;
		}

		if( m_iSelection < 0 )
		{
			m_iSelection = 0;
		}
		if( m_iSelection >= m_tOptions.GetCount() )
		{
			m_iSelection = m_tOptions.GetCount() - 1;
		}

		const float fOldValue = m_tOptions[ m_iSelection ].m_fValue;

		if( GfxInputIsJustPressed( EGfxInputID_AnyPadLeft ) || GfxInputIsJustPressed( EGfxInputID_KeyArrowLeft ) )
		{
			m_tOptions[ m_iSelection ].m_fValue -= m_tOptions[ m_iSelection ].m_fIncrement;
		}
		if( GfxInputIsJustPressed( EGfxInputID_AnyPadRight ) || GfxInputIsJustPressed( EGfxInputID_KeyArrowRight ) )
		{
			m_tOptions[ m_iSelection ].m_fValue += m_tOptions[ m_iSelection ].m_fIncrement;
		}

		if( m_tOptions[ m_iSelection ].m_fValue < m_tOptions[ m_iSelection ].m_fMinValue )
		{
			m_tOptions[ m_iSelection ].m_fValue = m_tOptions[ m_iSelection ].m_fMinValue;
		}
		if( m_tOptions[ m_iSelection ].m_fValue > m_tOptions[ m_iSelection ].m_fMaxValue )
		{
			m_tOptions[ m_iSelection ].m_fValue = m_tOptions[ m_iSelection ].m_fMaxValue;
		}

		if( iOldSelection != m_iSelection || fOldValue != m_tOptions[ m_iSelection ].m_fValue )
		{
			for( int i = 0; i < m_tOptions.GetCount(); ++i )
			{
				GfxTextSpritePrintf( m_tOptions[ i ].m_pTextSprite, "%c%s: %g", ( i == m_iSelection ) ? '>' : ' ', m_tOptions[ i ].m_pTextLabel, m_tOptions[ i ].m_fValue );
				GfxSpriteSetPosition( m_tOptions[ i ].m_pTextSprite, 0, static_cast< float >( TEXT_SPACING * i ) );
			}
		}

		return ( fOldValue != m_tOptions[ m_iSelection ].m_fValue );
	}

	return false;
}

void TMenu::Render()
{
	for( int i = 0; i < m_tOptions.GetCount(); ++i )
	{
		GfxSpriteRender( m_tOptions[ i ].m_pTextSprite );
	}
}
