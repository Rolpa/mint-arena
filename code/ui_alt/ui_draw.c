/*
===========================================================================
Copyright (C) 2014 Zack Middleton

This file is part of Spearmint Source Code.

Spearmint Source Code is free software; you can redistribute it
and/or modify it under the terms of the GNU General Public License as
published by the Free Software Foundation; either version 3 of the License,
or (at your option) any later version.

Spearmint Source Code is distributed in the hope that it will be
useful, but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Spearmint Source Code.  If not, see <http://www.gnu.org/licenses/>.

In addition, Spearmint Source Code is also subject to certain additional terms.
You should have received a copy of these additional terms immediately following
the terms and conditions of the GNU General Public License.  If not, please
request a copy in writing from id Software at the address below.

If you have questions concerning this license or the applicable additional
terms, you may contact in writing id Software LLC, c/o ZeniMax Media Inc.,
Suite 120, Rockville, Maryland 20850 USA.
===========================================================================
*/

#include "ui_local.h"

vec4_t color_header           = {1.00f, 1.00f, 1.00f, 1.00f};
vec4_t color_copyright        = {1.00f, 0.00f, 0.00f, 1.00f};
#ifdef MISSIONPACK
vec4_t color_bigtext          = {1.00f, 1.00f, 1.00f, 1.00f};
vec4_t color_selected         = {1.00f, 0.75f, 0.00f, 1.00f};
#else
// FIXME: I just guessed the colors
vec4_t color_bigtext          = {0.80f, 0.00f, 0.00f, 1.00f};
vec4_t color_selected         = {1.00f, 0.00f, 0.00f, 1.00f};
#endif

typedef struct {
	qhandle_t menuBackground;
	qhandle_t menuBackgroundNoLogo;

#ifdef MISSIONPACK
	qhandle_t menuBackgroundB;
	qhandle_t menuBackgroundC;
	qhandle_t menuBackgroundD;
	qhandle_t menuBackgroundE;
	qhandle_t levelShotDetail;
#else
	qhandle_t bannerModel;

	qhandle_t frameLeft;
	qhandle_t frameLeftFilled; // player model select menu
	qhandle_t frameRight;
#endif

	qhandle_t dialogSmallBackground;
	qhandle_t dialogLargeBackground;

} uiAssets_t;

uiAssets_t uiAssets;

void UI_LoadAssets( void ) {
	Com_Memset( &uiAssets, 0, sizeof ( uiAssets ) );

#ifdef MISSIONPACK
	uiAssets.menuBackground = trap_R_RegisterShaderNoMip( "menuback_a" );
	uiAssets.menuBackgroundB = trap_R_RegisterShaderNoMip( "menuback_b" );
	uiAssets.menuBackgroundC = trap_R_RegisterShaderNoMip( "menuback_c" );
	uiAssets.menuBackgroundD = trap_R_RegisterShaderNoMip( "menuback_d" );
	uiAssets.menuBackgroundE = trap_R_RegisterShaderNoMip( "menuback_e" );
	uiAssets.levelShotDetail = trap_R_RegisterShaderNoMip( "levelshotdetail" );
	uiAssets.menuBackgroundNoLogo = uiAssets.menuBackground;
#else
	uiAssets.bannerModel = trap_R_RegisterModel( "models/mapobjects/banner/banner5.md3" );
	uiAssets.menuBackground = trap_R_RegisterShaderNoMip( "menuback" );
	uiAssets.menuBackgroundNoLogo = trap_R_RegisterShaderNoMip( "menubacknologo" );

	uiAssets.frameLeft = trap_R_RegisterShaderNoMip( "menu/art/frame2_l" );
	uiAssets.frameLeftFilled = trap_R_RegisterShaderNoMip( "menu/art/frame1_l" );
	uiAssets.frameRight = trap_R_RegisterShaderNoMip( "menu/art/frame1_r" );
#endif

	uiAssets.dialogSmallBackground = trap_R_RegisterShaderNoMip( "menu/art/cut_frame" );
	uiAssets.dialogLargeBackground = trap_R_RegisterShaderNoMip( "menu/art/addbotframe" );
}

void UI_DrawMainMenuBackground( void ) {
#ifdef MISSIONPACK
	CG_DrawPic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uiAssets.menuBackground );

	CG_DrawPic( 0, 163, SCREEN_WIDTH, 153, uiAssets.menuBackgroundE ); // light flash
	CG_DrawPic( 0, 163, SCREEN_WIDTH, 153, uiAssets.menuBackgroundB ); // blue
		// FIXME?: menuback_b has bordercolor 0.5 0.5 0.5 .7

	// FIXME: rect 390 163 250 155 cinematic "mpintro.roq" backcolor 1 1 1 .25

	CG_DrawPic( 0, 163, 255, 155, uiAssets.menuBackgroundD ); // flashing team arena text

	CG_DrawPic( 0, 0, SCREEN_WIDTH, 240, uiAssets.levelShotDetail );
	CG_DrawPic( 0, 240, SCREEN_WIDTH, 240, uiAssets.levelShotDetail );

	CG_DrawPic( 205, 123, 235, 235, uiAssets.menuBackgroundC ); // center
#else
	refdef_t		refdef;
	refEntity_t		ent;
	vec3_t			origin;
	vec3_t			angles;
	float			adjust;
	float			x, y, w, h;

	// draw menuback
	CG_DrawPic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uiAssets.menuBackground );

	// setup the refdef

	memset( &refdef, 0, sizeof( refdef ) );

	refdef.rdflags = RDF_NOWORLDMODEL;

	AxisClear( refdef.viewaxis );

	x = 0;
	y = 0;
	w = 640;
	h = 120;
	CG_AdjustFrom640( &x, &y, &w, &h );
	refdef.x = x;
	refdef.y = y;
	refdef.width = w;
	refdef.height = h;

	adjust = 0; // JDC: Kenneth asked me to stop this 1.0 * sin( (float)uis.realtime / 1000 );
	refdef.fov_x = 60 + adjust;
	refdef.fov_y = 19.6875 + adjust;

	refdef.time = cg.realTime;

	origin[0] = 300;
	origin[1] = 0;
	origin[2] = -32;

	trap_R_ClearScene();

	// add the model

	memset( &ent, 0, sizeof(ent) );

	adjust = 5.0 * sin( (float)cg.realTime / 5000 );
	VectorSet( angles, 0, 180 + adjust, 0 );
	AnglesToAxis( angles, ent.axis );
	ent.hModel = uiAssets.bannerModel;
	VectorCopy( origin, ent.origin );
	VectorCopy( origin, ent.lightingOrigin );
	ent.renderfx = RF_LIGHTING_ORIGIN | RF_NOSHADOW;
	VectorCopy( ent.origin, ent.oldorigin );

	trap_R_AddRefEntityToScene( &ent );

	trap_R_RenderScene( &refdef );
#endif

	CG_DrawString( SCREEN_WIDTH / 2, SCREEN_HEIGHT - SMALLCHAR_HEIGHT*2, MENU_COPYRIGHT, UI_CENTER|UI_SMALLFONT, color_copyright );
}

void UI_DrawCurrentMenu( currentMenu_t *current ) {
	int i;
	qboolean drawFramePics = qtrue;
	vec4_t drawcolor;
	int style;

	if ( !current->menu ) {
		return;
	}

	if ( cg.connected ) {
		drawFramePics = qfalse;

		if ( current->menu->menuType != MENUTYPE_DIALOG ) {
			CG_DrawPic( 320-233, 240-166, 466, 332, uiAssets.dialogLargeBackground );
		}
	} else {
		if ( current->menu->menuType == MENUTYPE_MAINMENU ) {
			UI_DrawMainMenuBackground();
			drawFramePics = qfalse;
		} else {
			CG_DrawPic( 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, uiAssets.menuBackgroundNoLogo );
		}
	}

	if ( current->menu->menuType == MENUTYPE_DIALOG ) {
		CG_DrawPic( 142, 118, 359, 256, uiAssets.dialogSmallBackground );
	}
	else if ( drawFramePics ) {
#ifndef MISSIONPACK
		CG_DrawPic( 0, 78, 256, 329, uiAssets.frameLeft );
		// CG_DrawPic( 0, 78, 256, 329, uiAssets.frameLeftFilled ); // for player model select menu
		CG_DrawPic( 376, 76, 256, 334, uiAssets.frameRight );
#endif
	}

	// doesn't have a special header handling, use generic
	if ( current->menu->header ) {
#ifdef Q3UIFONTS
		UI_DrawBannerString( current->header.x, current->header.y, current->menu->header, 0, color_header );
#else
		CG_DrawString( current->header.x, current->header.y, current->menu->header, UI_DROPSHADOW|UI_BIGFONT, color_header );
#endif
	}

	for ( i = 0; i < current->numItems; i++ ) {
		style = 0;

		if ( !( current->items[i].flags & MIF_SELECTABLE ) ) {
			Vector4Copy( color_bigtext, drawcolor );

			VectorScale( drawcolor, 0.7f, drawcolor );
		} else if ( current->selectedItem == i ) {
			style |= UI_PULSE;
			Vector4Copy( color_selected, drawcolor );
		} else {
			Vector4Copy( color_bigtext, drawcolor );
		}

#ifdef Q3UIFONTS
		UI_DrawProportionalString( current->items[i].x, current->items[i].y, current->items[i].caption, style, drawcolor );
#else
		CG_DrawString( current->items[i].x, current->items[i].y, current->items[i].caption, UI_DROPSHADOW|UI_BIGFONT|style, drawcolor );
#endif
	}
}

// TODO: Fix font heights in UI_BuildCurrentMenu with Q3UIFONTS defined
#define MAX_MENU_HEADERS	8
// this is used for drawing and logic. it's closely related to UI_DrawCurrentMenu.
void UI_BuildCurrentMenu( currentMenu_t *current ) {
	int i, x, y = 0, lineHeight, horizontalGap = BIGCHAR_WIDTH;
	int	numHeaders, totalWidth[MAX_MENU_HEADERS] = {0};
	int headerBottom = -1;
	menuitem_t *item;
	qboolean horizontalMenu = qfalse;

	if ( current->menu->header ) {
#ifdef Q3UIFONTS // ug, dialogs need to use prop font
		current->header.width = UI_BannerStringWidth( current->menu->header );
#else
		current->header.width = CG_DrawStrlen( current->menu->header, UI_BIGFONT );
#endif
		current->header.x = ( SCREEN_WIDTH - current->header.width ) / 2;
		current->header.y = 10;
		current->header.height = BIGCHAR_HEIGHT;

		headerBottom = current->header.y + current->header.height;
	} else {
		Com_Memset( &current->header, 0, sizeof ( current->header ) );
		headerBottom = 0;
	}

#ifndef MISSIONPACK
	if ( !cg.connected && current->menu->menuType == MENUTYPE_MAINMENU ) {
		// Q3 banner model
		headerBottom = 120;
	}
#endif

	if ( current->menu->menuType == MENUTYPE_DIALOG ) {
		current->header.y = 204;
		headerBottom = 265;
		horizontalMenu = qtrue;
	}

	lineHeight = BIGCHAR_HEIGHT * 2;

	numHeaders = 0;
	for ( i = 0, item = current->menu->items; i < current->menu->numItems; i++, item++ ) {
#ifdef Q3UIFONTS
		current->items[i].width = UI_ProportionalStringWidth( item->caption );
		current->items[i].height = PROP_HEIGHT;
#else
		current->items[i].width = CG_DrawStrlen( item->caption, UI_BIGFONT );
		current->items[i].height = BIGCHAR_HEIGHT;
#endif

		if ( item->flags & MIF_HEADER ) {
			numHeaders++;

			if ( !( item->flags & MIF_SELECTABLE ) ) {
				continue;
			}
		}

		totalWidth[numHeaders] += current->items[i].width;
		if ( i != current->menu->numItems - 1 && !(current->menu->items[i+1].flags & MIF_HEADER) ) {
			totalWidth[numHeaders] += horizontalGap;
		}
	}

	numHeaders = 0;
	for ( i = 0, item = current->menu->items; i < current->menu->numItems; i++, item++ ) {

		if ( item->flags & MIF_NEXTBUTTON ) {
			// Fixed place in lower right
			current->items[i].flags = item->flags;
			current->items[i].data = item->data;
			current->items[i].caption = item->caption;
			current->items[i].y = SCREEN_HEIGHT - current->items[i].height - 10;
			current->items[i].x = SCREEN_WIDTH - current->items[i].width - 10;
			continue;
		}

		if ( item->flags & MIF_HEADER ) {
			numHeaders++;
			x = (SCREEN_WIDTH - totalWidth[numHeaders]) / 2;

			if ( item->y != 0 ) {
				y = item->y;
			} else if ( i == 0 ) {
				y = headerBottom;
			} else {
				y += lineHeight;
			}

			current->items[i].flags = item->flags;
			current->items[i].data = item->data;
			current->items[i].caption = item->caption;
			current->items[i].y = y;
			current->items[i].x = (SCREEN_WIDTH - current->items[i].width) / 2;

			// put items below header
			y += lineHeight;

			continue;
		} else if ( numHeaders ) {
			if ( i > 0 && !( current->items[i-1].flags & MIF_HEADER ) )
				x += current->items[i-1].width + horizontalGap;

			if ( item->y != 0 ) {
				y = item->y;
			}

			current->items[i].flags = item->flags;
			current->items[i].data = item->data;
			current->items[i].caption = item->caption;
			current->items[i].y = y;
			current->items[i].x = x;
			continue;
		}

		if ( horizontalMenu ) {
			if ( i == 0 ) {
				// center x
				x = (SCREEN_WIDTH - totalWidth[numHeaders]) / 2;

				//if ( current->menu->menuType == MENUTYPE_DIALOG ) {
					y = headerBottom;
				/*} else if ( item->y != 0 ) {
					y = item->y;
				} else {
					// center y
					y = headerBottom + (SCREEN_HEIGHT - headerBottom - lineHeight) / 2;
				}*/
			} else {
				x += current->items[i-1].width + horizontalGap;
				if ( item->y != 0 ) {
					y = item->y;
				}
			}
		} else {
			if ( item->y != 0 ) {
				y = item->y;
			} else if ( i == 0 ) {
				// center Y
				y = headerBottom + (SCREEN_HEIGHT - headerBottom - current->menu->numItems * lineHeight) / 2;
			} else {
				y += lineHeight;
			}

			//if ( item->flags & MIF_CENTER )
				x = (SCREEN_WIDTH - current->items[i].width) / 2;
			//else
			//	x = 50;
		}

		current->items[i].flags = item->flags;
		current->items[i].data = item->data;
		current->items[i].caption = item->caption;
		current->items[i].y = y;
		current->items[i].x = x;
	}

	current->numItems = i;

	// add back button
	if ( current->numStacked && current->menu->menuType != MENUTYPE_DIALOG ) {
		y += lineHeight;
		current->items[i].flags = MIF_POPMENU;
		current->items[i].data = NULL;

#ifdef MISSIONPACK
		if ( current->numStacked == 1 )
			current->items[i].caption = "Exit to Main Menu";
		else
#endif
		current->items[i].caption = "Back";

#ifdef Q3UIFONTS
		current->items[i].width = UI_ProportionalStringWidth( current->items[i].caption );
		current->items[i].height = PROP_HEIGHT;
#else
		current->items[i].width = CG_DrawStrlen( current->items[i].caption, UI_BIGFONT );
		current->items[i].height = BIGCHAR_HEIGHT;
#endif

#if 1 // Fixed place in lower left
		current->items[i].y = SCREEN_HEIGHT - current->items[i].height - 10;
		current->items[i].x = 10;
#else // fake next of menu item (though it's not vertically centered by the above numItems code)
		current->items[i].y = y;
		current->items[i].x = (SCREEN_WIDTH - current->items[i].width) / 2;
#endif

		i++;
		current->numItems++;
	}

}

