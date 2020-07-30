#include "SDL2Context.hpp"
#include "NE_StringUtils.hpp"

#include <SDL_ttf.h>
#include <cmath>

static const float SafetyTextRatio = 1.2f;

static SDL_Rect CreateRect (const NUIE::Rect& rect)
{
	SDL_Rect sdlRect;
	NUIE::IntRect intRect (rect);
	sdlRect.x = intRect.GetX ();
	sdlRect.y = intRect.GetY ();
	sdlRect.w = intRect.GetWidth ();
	sdlRect.h = intRect.GetHeight ();
	return sdlRect;
}

SDL2Context::SDL2Context (int width, int height, SDL_Renderer* renderer) :
	width (width),
	height (height),
	renderer (renderer)
{

}

void SDL2Context::Resize (int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
}

int SDL2Context::GetWidth () const
{
	return width;
}

int SDL2Context::GetHeight () const
{
	return height;
}

void SDL2Context::BeginDraw ()
{
	SDL_SetRenderDrawColor (renderer, 255, 255, 255, 255);
	SDL_RenderClear (renderer);
}

void SDL2Context::EndDraw ()
{
	SDL_RenderPresent (renderer);
}

bool SDL2Context::NeedToDraw (ItemPreviewMode)
{
	return true;
}

void SDL2Context::DrawLine (const NUIE::Point& /*beg*/, const NUIE::Point& /*end*/, const NUIE::Pen& /*pen*/)
{

}

void SDL2Context::DrawBezier (const NUIE::Point& /*p1*/, const NUIE::Point& /*p2*/, const NUIE::Point& /*p3*/, const NUIE::Point& /*p4*/, const NUIE::Pen& /*pen*/)
{

}

void SDL2Context::DrawRect (const NUIE::Rect& rect, const NUIE::Pen& pen)
{
	SDL_Rect sdlRect = CreateRect (rect);
	const NUIE::Color& color = pen.GetColor ();
	SDL_SetRenderDrawColor (renderer, color.GetR (), color.GetG (), color.GetB (), 255);
	SDL_RenderDrawRect (renderer, &sdlRect);
}

void SDL2Context::FillRect (const NUIE::Rect& rect, const NUIE::Color& color)
{
	SDL_Rect sdlRect = CreateRect (rect);
	SDL_SetRenderDrawColor (renderer, color.GetR (), color.GetG (), color.GetB (), 255);
	SDL_RenderFillRect (renderer, &sdlRect);
}

void SDL2Context::DrawEllipse (const NUIE::Rect& /*rect*/, const NUIE::Pen& /*pen*/)
{

}

void SDL2Context::FillEllipse (const NUIE::Rect& /*rect*/, const NUIE::Color& /*color*/)
{

}

void SDL2Context::DrawFormattedText (const NUIE::Rect& rect, const NUIE::Font& font, const std::wstring& text, NUIE::HorizontalAnchor hAnchor, NUIE::VerticalAnchor vAnchor, const NUIE::Color& textColor)
{
	TTF_Font* ttfFont = TTF_OpenFont ("Assets/FreeSans.ttf", (int) font.GetSize ());

	std::string textStr = NE::WStringToString (text);
	SDL_Color sdlColor = { textColor.GetR (), textColor.GetG (), textColor.GetB (), 255 };
	SDL_Surface* surface = TTF_RenderUTF8_Blended (ttfFont, textStr.c_str (), sdlColor);
	SDL_Texture* texture = SDL_CreateTextureFromSurface (renderer, surface);

	int textWidth = 0;
	int textHeight = 0;
	SDL_QueryTexture (texture, NULL, NULL, &textWidth, &textHeight);

	SDL_Rect originalRect = CreateRect (rect);
	SDL_Rect textRect = { originalRect.x, originalRect.y, textWidth, textHeight };

	if (hAnchor == NUIE::HorizontalAnchor::Left) {
		// nothing to do
	} else if (hAnchor == NUIE::HorizontalAnchor::Center) {
		textRect.x += (int) std::floor (((double) originalRect.w - (double) textRect.w) / 2.0);
	} else if (hAnchor == NUIE::HorizontalAnchor::Right) {
		textRect.x += originalRect.w - textRect.w;
	}

	if (vAnchor == NUIE::VerticalAnchor::Top) {
		// nothing to do
	} else if (vAnchor == NUIE::VerticalAnchor::Center) {
		textRect.y += (int) std::floor (((double) originalRect.h - (double) textRect.h) / 2.0);
	} else if (vAnchor == NUIE::VerticalAnchor::Bottom) {
		textRect.y += originalRect.h - textRect.h;
	}

	SDL_RenderCopy (renderer, texture, NULL, &textRect);

	SDL_DestroyTexture (texture);
	SDL_FreeSurface (surface);
	TTF_CloseFont (ttfFont);
}

NUIE::Size SDL2Context::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	TTF_Font* ttfFont = TTF_OpenFont ("Assets/FreeSans.ttf", (int) font.GetSize ());
	std::string textStr = NE::WStringToString (text);
	int textWidth = 0;
	int textHeight = 0;
	TTF_SizeText (ttfFont, textStr.c_str (), &textWidth, &textHeight);
	TTF_CloseFont (ttfFont);
	return NUIE::Size (textWidth * SafetyTextRatio, textHeight);
}

bool SDL2Context::CanDrawIcon ()
{
	return false;
}

void SDL2Context::DrawIcon (const NUIE::Rect& /*rect*/, const NUIE::IconId& /*iconId*/)
{

}
