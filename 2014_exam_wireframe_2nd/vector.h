#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include "flib.h"

template< typename T >
class Vector
{
public:

	Vector( int iSize = 0 )
		: m_iBufferSize( 0 )
		, m_iBufferCount( 0 )
		, m_pBuffer( 0 )
	{
		if( iSize > 0 )
		{
			IncreaseSize( iSize );
		}
	}

	~Vector()
	{
		delete[] m_pBuffer;
	}

	void Clear()
	{
		m_iBufferCount = 0;
	}

	void PushBack( const T & tValue )
	{
		if( m_iBufferSize == m_iBufferCount )
		{
			IncreaseSize( 32 );
		}

		m_pBuffer[ m_iBufferCount ] = tValue;
		++m_iBufferCount;
	}

	int GetCount() const
	{
		return m_iBufferCount;
	}

	T & operator[]( int iIndex )
	{
		GfxDbgAssert( iIndex >= 0 && iIndex < m_iBufferCount );
		return m_pBuffer[ iIndex ];
	}

	const T & operator[]( int iIndex ) const
	{
		GfxDbgAssert( iIndex >= 0 && iIndex < m_iBufferCount );
		return m_pBuffer[ iIndex ];
	}

private:

	void IncreaseSize( int iAmount )
	{
		m_iBufferSize += iAmount;
		T * pNewBuffer = new T[ m_iBufferSize ];

		for( int i = 0; i < m_iBufferCount; ++i )
		{
			pNewBuffer[ i ] = m_pBuffer[ i ];
		}

		delete[] m_pBuffer;
		m_pBuffer = pNewBuffer;
	}

	int m_iBufferSize;
	int m_iBufferCount;
	T * m_pBuffer;
};

#endif
