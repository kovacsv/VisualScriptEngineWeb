#include "SDL2Context.hpp"
#include "SDL2_gfxPrimitives.h"
#include "NE_StringUtils.hpp"

#include <vector>
#include <cmath>

static const bool UseSDLGfx = false;

static const float TextRatioX = 1.1f;
static const float TextRatioY = 1.0f;

static SDL_Point CreatePoint (const NUIE::Point& point)
{
	SDL_Point sdlPoint;
	NUIE::IntPoint intPoint (point);
	sdlPoint.x = intPoint.GetX ();
	sdlPoint.y = intPoint.GetY ();
	return sdlPoint;
}

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

SDL2Context::SDL2Context (SDL_Renderer* renderer, const std::string& fontPath) :
	width (0),
	height (0),
	renderer (renderer),
	fontPath (fontPath),
	fontController (fontPath),
	fontCache (128, &fontController),
	fontTextureController (renderer, fontCache),
	fontTextureCache (128, &fontTextureController)
{
	SDL_Rect viewRect;
	SDL_RenderGetViewport (renderer, &viewRect);
	width = viewRect.w;
	height = viewRect.h;
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

void SDL2Context::DrawLine (const NUIE::Point& beg, const NUIE::Point& end, const NUIE::Pen& pen)
{
	const NUIE::Color& color = pen.GetColor ();
	SDL_SetRenderDrawColor (renderer, color.GetR (), color.GetG (), color.GetB (), 255);
	SDL_Point sdlBeg = CreatePoint (beg);
	SDL_Point sdlEnd = CreatePoint (end);
	SDL_RenderDrawLine (renderer, sdlBeg.x, sdlBeg.y, sdlEnd.x, sdlEnd.y);
}

void SDL2Context::DrawBezier (const NUIE::Point& p1, const NUIE::Point& p2, const NUIE::Point& p3, const NUIE::Point& p4, const NUIE::Pen& pen)
{
	if (UseSDLGfx) {
		std::vector<SDL_Point> sdlPoints = {
			CreatePoint (p1),
			CreatePoint (p2),
			CreatePoint (p3),
			CreatePoint (p4)
		};
		std::vector<double> xPoints;
		std::vector<double> yPoints;
		for (const SDL_Point& point : sdlPoints) {
			xPoints.push_back (point.x);
			yPoints.push_back (point.y);
		}
		const NUIE::Color& color = pen.GetColor ();
		aaBezierRGBA (renderer, &xPoints[0], &yPoints[0], 4, 20, (float) pen.GetThickness (), color.GetR (), color.GetG (), color.GetB (), 255);
	} else {
		static const bool UseDrawLines = true;
		std::vector<NUIE::Point> points = NUIE::SegmentBezier (20, p1, p2, p3, p4);
		if (UseDrawLines) {
			std::vector<SDL_Point> sdlPoints;
			for (const NUIE::Point& point : points) {
				sdlPoints.push_back (CreatePoint (point));
			}
			const NUIE::Color& color = pen.GetColor ();
			SDL_SetRenderDrawColor (renderer, color.GetR (), color.GetG (), color.GetB (), 255);
			SDL_RenderDrawLines (renderer, &sdlPoints[0], (int) sdlPoints.size ());
		} else {
			for (size_t i = 0; i < points.size () - 1; i++) {
				DrawLine (points[i], points[i + 1], pen);
			}
		}
	}
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
	FontTextureCacheKey key (text, textColor, (int) font.GetSize ());
	SDL_Texture* texture = fontTextureCache.Get (key);

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
}

NUIE::Size SDL2Context::MeasureText (const NUIE::Font& font, const std::wstring& text)
{
	TTF_Font* ttfFont = fontCache.Get ((int) font.GetSize ());
	std::string textStr = NE::WStringToString (text);
	int textWidth = 0;
	int textHeight = 0;
	TTF_SizeText (ttfFont, textStr.c_str (), &textWidth, &textHeight);
	return NUIE::Size (textWidth * TextRatioX, textHeight * TextRatioY);
}

bool SDL2Context::CanDrawIcon ()
{
	return false;
}

void SDL2Context::DrawIcon (const NUIE::Rect& /*rect*/, const NUIE::IconId& /*iconId*/)
{
	// SDL_Surface* surface = IMG_Load ("Assets/Image.png");
	// SDL_Texture* texture = SDL_CreateTextureFromSurface (renderer, surface);
	// SDL_FreeSurface (surface);
	// 
	// SDL_Rect sdlRect = CreateRect (rect);
	// SDL_RenderCopy (renderer, texture, NULL, &sdlRect);
	// SDL_DestroyTexture (texture);
}
