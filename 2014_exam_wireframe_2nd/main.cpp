#include "flib.h"
#include "build_geometries.h"
#include "geometry.h"
#include "menu.h"

TGfxSprite * g_pLineSprite;
TGfxSprite * g_pLabelSprite;
TGeometry g_tGeometry;

TMenu g_pMenu[] =
{
	"Orbit",
	"Honeycomb",
	"Helix",
	"Brick Wall",
};

enum EMenu
{
	EMenu_Orbit,
	EMenu_Honeycomb,
	EMenu_Helix,
	EMenu_BrickWall,
	EMenu_COUNT,
};

EMenu g_tCurrentMenu = EMenu_Orbit;

void UpdateGeometry()
{
	g_tGeometry.Clear();
	BuildBox( g_tGeometry );

	const TMenu & tMenu = g_pMenu[ g_tCurrentMenu ];

	switch (g_tCurrentMenu)
	{
		case EMenu_Honeycomb:
		{
			BuildHoneycomb(g_tGeometry, tMenu.GetInt(0), tMenu.GetInt(1));
			break;
		}

		case EMenu_Helix:
		{
			BuildHelix(g_tGeometry, tMenu.GetFloat(0), tMenu.GetInt(1), tMenu.GetFloat(2), tMenu.GetInt(3), tMenu.GetInt(4));
			break;
		}

		case EMenu_Orbit:
		{
			BuildOrbit(g_tGeometry, tMenu.GetInt(0), tMenu.GetInt(1), tMenu.GetInt(2));
			break;
		}

		case EMenu_BrickWall:
		{
			BuildBrickWall(g_tGeometry, tMenu.GetInt(0), tMenu.GetInt(1), tMenu.GetFloat(2));
			break;
		}

		default:
		{
		}
	}

	GfxTextSpritePrintf( g_pLabelSprite, "%s - %d lines", tMenu.GetName(), g_tGeometry.GetLineCount() );
	GfxSpriteSetPivot( g_pLabelSprite, static_cast< float >( GfxSpriteGetSizeX( g_pLabelSprite ) / 2 ), static_cast< float >( GfxSpriteGetSizeY( g_pLabelSprite ) ) );
	GfxSpriteSetPosition( g_pLabelSprite, static_cast< float >( GfxGetDisplaySizeX() / 2 ), static_cast< float >( GfxGetDisplaySizeY() ) );
}

void Initialize()
{
	g_pMenu[EMenu_Honeycomb].AddOption("rows", 5, 1, 20);
	g_pMenu[EMenu_Honeycomb].AddOption("turns", 5, 1, 20);

	g_pMenu[EMenu_Helix].AddOption("turns", 3, 0.5f, 5, 0.25f);
	g_pMenu[EMenu_Helix].AddOption("twists per turn", 5, 0, 10);
	g_pMenu[EMenu_Helix].AddOption("radius", 1 / 16.0f, 1 / 16.0f, 0.25f, 1 / 32.0f);
	g_pMenu[EMenu_Helix].AddOption("segments per turn", 256, 32, 256, 16);
	g_pMenu[EMenu_Helix].AddOption("link step", 5, 1, 16, 1);

	g_pMenu[EMenu_Orbit].AddOption("twist angle", 45, 0, 720, 15);
	g_pMenu[EMenu_Orbit].AddOption("segments per circle", 32, 4, 64, 2);
	g_pMenu[EMenu_Orbit].AddOption("circle count", 20, 2, 50, 2);

	g_pMenu[EMenu_BrickWall].AddOption("rows", 7, 1, 20);
	g_pMenu[EMenu_BrickWall].AddOption("bricks per row/face", 5, 1, 20);
	g_pMenu[EMenu_BrickWall].AddOption("wall thickness", 0.125f, 1.0f / 16.0f, 0.5f, 1.0f / 16.0f);

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

	if( GfxInputIsJustPressed( EGfxInputID_AnyPadShoulderR ) || GfxInputIsJustPressed( EGfxInputID_KeyControl ) || GfxInputIsJustPressed( EGfxInputID_MouseLeft ) )
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
