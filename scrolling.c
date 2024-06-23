#include "scrolling.h"
#include "scroll.h"
#include "string.h"
#include "debug.h"
#include "input.h"
#include "math.h"

static VectorU16 gScreenPosition;
static u8 gScrollSpriteIndex;

void ScrollInit(const c8* tilemap)
{
    gScrollSpriteIndex = Scroll_Initialize((u16)tilemap);
    gScreenPosition.x = 0;
    gScreenPosition.y = 0;

    c8 buffer[10];
    String_Format(buffer, "%i", gScrollSpriteIndex);
    DEBUG_LOG(buffer);
}

void ScrollUpdate(void)
{
    Scroll_Update();

    if (Keyboard_IsKeyPressed(KEY_F))
    {
        ScrollX(-1);
    }
    else if (Keyboard_IsKeyPressed(KEY_H))
    {
        ScrollX(1);
    }

    if (Keyboard_IsKeyPressed(KEY_T))
    {
        ScrollY(-1);
    }
    else if (Keyboard_IsKeyPressed(KEY_G))
    {
        ScrollY(1);
    }
}

void ScrollX(i16 offset)
{
    gScreenPosition.x += offset;
    Scroll_SetOffsetH(offset);
}

void ScrollY(i16 offset)
{
    gScreenPosition.y += offset;
    Scroll_SetOffsetV(offset);
}

u16 ScrollGetX(void)
{
    return gScreenPosition.x;
}

u16 ScrollGetY(void)
{
    return gScreenPosition.y;
}

u8 ScrollGetSpriteIndex(void)
{
    return gScrollSpriteIndex;
}
