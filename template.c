// ____________________________
// ██▀▀█▀▀██▀▀▀▀▀▀▀█▀▀█        │   ▄▄▄                ▄▄
// ██  ▀  █▄  ▀██▄ ▀ ▄█ ▄▀▀ █  │  ▀█▄  ▄▀██ ▄█▄█ ██▀▄ ██  ▄███
// █  █ █  ▀▀  ▄█  █  █ ▀▄█ █▄ │  ▄▄█▀ ▀▄██ ██ █ ██▀  ▀█▄ ▀█▄▄
// ▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀▀────────┘                 ▀▀
//  Game module sample
//─────────────────────────────────────────────────────────────────────────────

//=============================================================================
// INCLUDES
//=============================================================================
#include "msxgl.h"
#include "game.h"
#include "math.h"
#include "debug.h"
#include "string.h"

#include "entity.h"
#include "player.h"
#include "bullet.h"
#include "scrolling.h"

// Prototypes
bool State_Initialize();
bool State_Game();
bool State_Pause();

//=============================================================================
// READ-ONLY DATA
//=============================================================================

// Fonts
#include "font/font_carwar.h"
// Sprites by GrafxKid (https://opengameart.org/content/super-random-sprites)
#include "content/data_sprt_layer.h"
#include "content/data_sprt_ball.h"
#include "content/data_bg.h"
#include "content/data_map_gm2.h"

//-----------------------------------------------------------------------------
// Load pattern data into VRAM
void VDP_LoadSpritePattern16_VSym(const u8* src, u8 index, u8 count)
{
	u16 dest = g_SpritePatternLow + (index * 8);
	for (u16 i = 0; i < count * 8; i++)
	{
		u8 val = Math_Flip(*src++);
		u16 addr = dest++;
		if (i & 0x10) // Shift every 16 bytes to flip 16x16 sprite's 4 blocks
			addr -= 16;
		else
			addr += 16;
		VDP_Poke_16K(val, addr);
	}
}

//-----------------------------------------------------------------------------
//
bool State_Initialize()
{
	// Initialize display
	VDP_EnableDisplay(FALSE);
	VDP_SetColor(COLOR_CYAN);
	
	// Initialize pattern
	VDP_FillVRAM(32, g_ScreenLayoutLow, 0, 32*24);
	VDP_FillVRAM(0, g_ScreenPatternLow, 0, 256*8); // Clear pattern
	VDP_WriteVRAM(g_DataBackground, g_ScreenPatternLow, 0, 24*8);

	VDP_SetLayoutTable(0x3800);
	VDP_SetColorTable(0x2000);
	VDP_SetPatternTable(0x0000);
	VDP_SetSpritePatternTable(0x1800);
	VDP_SetSpriteAttributeTable(0x3E00);

	// Initialize text
	Print_SetTextFont(g_Font_Carwar, 65);
	Print_SetColor(0xF, 0x1);
	Print_SetPosition(0, 0);

	/*
	// Initialize color
	VDP_FillVRAM(0xF0, g_ScreenColorLow, 0, 32); // Clear color
	VDP_Poke_16K(0x7F, g_ScreenColorLow + 0);
	VDP_Poke_16K(0x5F, g_ScreenColorLow + 1);
	VDP_Poke_16K(0xF5, g_ScreenColorLow + 2);
	VDP_Poke_16K(0x99, g_ScreenColorLow + 3);
	*/

	// Load tiles pattern
	VDP_LoadPattern_GM2(g_DataMapGM2_Patterns, 94, 0);
	VDP_LoadColor_GM2(g_DataMapGM2_Colors, 94, 0);

	ScrollInit(g_DataMapGM2_Names);

	// Initialize sprite
	VDP_SetSpriteFlag(VDP_SPRITE_SIZE_16);
	//                           Source data              SprtID  Num
	VDP_LoadSpritePattern16_VSym(g_DataSprtLayer+8*4*4*1, 4*4*0,  4*4*2);
	VDP_LoadSpritePattern16_VSym(g_DataSprtLayer+8*4*4*4, 4*4*2,  4*4*3);
	VDP_LoadSpritePattern16_VSym(g_DataSprtLayer+8*4*4*8, 4*4*5,  4*4*2);
	//                    Source data              SprtID  Num
	VDP_LoadSpritePattern(g_DataSprtLayer+8*4*4*1, 4*4*7,  4*4*2);
	VDP_LoadSpritePattern(g_DataSprtLayer+8*4*4*4, 4*4*9,  4*4*3);
	VDP_LoadSpritePattern(g_DataSprtLayer+8*4*4*8, 4*4*12, 4*4*2);
	//                    Source data              SprtID  Num
	VDP_LoadSpritePattern(g_DataSprtBall,          4*4*14, 4*2*3);
	// VDP_SetSpriteSM1(6, 0, 208, 0, 0); // hide

	EntitiesSetup();

	EntityInit(ENTITY_PLAYER, 0, 80, 60);
	EntityInit(ENTITY_BULLET, BULLET_RIGHT, 80, 60);
	EntityInit(ENTITY_BULLET, BULLET_TOP_RIGHT, 80, 60);
	EntityInit(ENTITY_BULLET, BULLET_TOP, 80, 60);
	EntityInit(ENTITY_BULLET, BULLET_TOP_LEFT, 80, 60);
	EntityInit(ENTITY_BULLET, BULLET_LEFT, 80, 60);
	EntityInit(ENTITY_BULLET, BULLET_BOTTOM_LEFT, 80, 60);
	EntityInit(ENTITY_BULLET, BULLET_BOTTOM, 80, 60);
	EntityInit(ENTITY_BULLET, BULLET_BOTTOM_RIGHT, 80, 60);
	

	VDP_EnableDisplay(TRUE);

	Game_SetState(State_Game);
	return FALSE; // Frame finished
}

#define S_DRAW		0
#define S_UPDATE	1
#define S_INPUT		2

//-----------------------------------------------------------------------------
//
bool State_Game()
{
	PROFILE_FRAME_START();

	PROFILE_SECTION_START(S_UPDATE, 100);
	EntitiesUpdate();
	PROFILE_SECTION_END(S_UPDATE, 100);

	PROFILE_SECTION_START(S_INPUT, 100);
	if (Keyboard_IsKeyPressed(KEY_SPACE))
	{
		EntityInit(ENTITY_BULLET, BULLET_RIGHT, 64, 128);
	}
	PROFILE_SECTION_END(S_INPUT, 100);

	PROFILE_FRAME_END();

	Halt(); // Wait V-Blank

	ScrollUpdate();

	PROFILE_SECTION_START(S_DRAW, 100);
	EntitiesDraw();
	PROFILE_SECTION_END(S_DRAW, 100);

	return TRUE; // Frame finished
}

//-----------------------------------------------------------------------------
//
bool State_Pause()
{
	return TRUE; // Frame finished
}

//=============================================================================
// MAIN LOOP
//=============================================================================

//-----------------------------------------------------------------------------
// Program entry point
void main()
{
	DEBUG_INIT();
	DEBUG_LOG("Start debug session!");

	Game_SetState(State_Initialize);
	Game_MainLoop(VDP_MODE_GRAPHIC3);

	DEBUG_LOG("End debug session!");
}