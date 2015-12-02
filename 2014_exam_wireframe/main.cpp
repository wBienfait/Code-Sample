#include "flib.h"
#include "build_geometries.h"
#include "geometry.h"
#include "menu.h"

TGfxSprite * g_pLineSprite;
TGfxSprite * g_pLabelSprite;
TGeometry g_tGeometry;

TMenu g_pMenu[] =
{
	"Plane",
	"Pyramid",
	"Helicoid",
	"Sphere",
	"Doughnut",
};

enum EMenu
{
	EMenu_Plane,
	EMenu_Pyramid,
	EMenu_Helicoid,
	EMenu_Sphere,
	EMenu_Doughnut,
	EMenu_COUNT,
};

EMenu g_tCurrentMenu = EMenu_Plane;

void UpdateGeometry()
{
	g_tGeometry.Clear();
	BuildBox( g_tGeometry );

	const TMenu & tMenu = g_pMenu[ g_tCurrentMenu ];

	switch( g_tCurrentMenu )
	{
		case EMenu_Plane :
		{
			BuildPlane( g_tGeometry, tMenu.GetInt( 0 ), tMenu.GetInt( 1 ) );
			break;
		}

		case EMenu_Pyramid :
		{
			BuildPyramid( g_tGeometry, tMenu.GetInt( 0 ) );
			break;
		}

		case EMenu_Helicoid :
		{
			BuildHelicoid( g_tGeometry, tMenu.GetInt( 0 ), tMenu.GetInt( 1 ) );
			break;
		}

		case EMenu_Sphere :
		{
			BuildSphere( g_tGeometry, tMenu.GetInt( 0 ), tMenu.GetInt( 1 ), tMenu.GetInt( 2 ) );
			break;
		}

		case EMenu_Doughnut :
		{
			BuildDoughnut( g_tGeometry, tMenu.GetInt( 0 ), tMenu.GetFloat( 1 ) );
			break;
		}
	}

	GfxTextSpritePrintf( g_pLabelSprite, "%s - %d lines", tMenu.GetName(), g_tGeometry.GetLineCount() );
	GfxSpriteSetPivot( g_pLabelSprite, static_cast< float >( GfxSpriteGetSizeX( g_pLabelSprite ) / 2 ), static_cast< float >( GfxSpriteGetSizeY( g_pLabelSprite ) ) );
	GfxSpriteSetPosition( g_pLabelSprite, static_cast< float >( GfxGetDisplaySizeX() / 2 ), static_cast< float >( GfxGetDisplaySizeY() ) );
}

void Initialize()
{
	g_pMenu[ EMenu_Plane ].AddOption( "tiles X", 5, 1, 20 );
	g_pMenu[ EMenu_Plane ].AddOption( "tiles Z", 5, 1, 20 );

	g_pMenu[ EMenu_Pyramid ].AddOption( "layers", 5, 3, 10 );

	g_pMenu[ EMenu_Helicoid ].AddOption( "segments", 10, 4, 40 );
	g_pMenu[ EMenu_Helicoid ].AddOption( "twist angle", 180, 0, 720, 20 );

	g_pMenu[ EMenu_Sphere ].AddOption( "segments", 20, 4, 40 );
	g_pMenu[ EMenu_Sphere ].AddOption( "meridians", 8, 1, 20 );
	g_pMenu[ EMenu_Sphere ].AddOption( "parallels", 9, 1, 20 );

	g_pMenu[ EMenu_Doughnut ].AddOption( "segments", 12, 4, 30 );
	g_pMenu[ EMenu_Doughnut ].AddOption( "radius", 1 / 4.0f, 0, 0.5f, 1 / 32.0f );

	g_pLineSprite = GfxLineSpriteCreate();
	GfxSpriteSetPosition( g_pLineSprite, 480 / 2, 272 / 2 );
	GfxSpriteSetScale( g_pLineSprite, 100, -100 );

	g_pLabelSprite = GfxTextSpriteCreate();
	UpdateGeometry();
}

void Update()
{
	int iMenuIndex = g_tCurrentMenu;
	const int iOldMenuIndex = iMenuIndex;

	if( GfxInputIsJustPressed( EGfxInputID_AnyPadShoulderL ) || GfxInputIsJustPressed( EGfxInputID_KeyShift ) )
	{
		iMenuIndex--;
	}

	if( GfxInputIsJustPressed( EGfxInputID_AnyPadShoulderR ) || GfxInputIsJustPressed( EGfxInputID_KeyControl ) )
	{
		iMenuIndex++;
	}

	g_tCurrentMenu = static_cast< EMenu >( ( iMenuIndex + EMenu_COUNT ) % EMenu_COUNT );

	if( iMenuIndex != iOldMenuIndex || g_pMenu[ g_tCurrentMenu ].Update() )
	{
		UpdateGeometry();
	}

	GfxLineSpriteReset( g_pLineSprite );
	g_tGeometry.Render( g_pLineSprite );
}

void Render()
{
	GfxClear( EGfxColor_Black );
	GfxSpriteRender( g_pLineSprite );
	g_pMenu[ g_tCurrentMenu ].Render();
	GfxSpriteRender( g_pLabelSprite );
}

void GfxMain( int, char * [] )
{
	GfxCallbacks( Initialize, Update, Render );
}
