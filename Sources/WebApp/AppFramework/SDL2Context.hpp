#ifndef SDL2_CONTEXT_HPP
#define SDL2_CONTEXT_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <SDL_image.h>

#include "NE_Cache.hpp"
#include "NUIE_DrawingContext.hpp"

#include "SDL2Caches.hpp"

class SDL2Context : public NUIE::DrawingContext
{
public:
	SDL2Context (SDL_Renderer* renderer, const std::string& fontPath);

	virtual void		Resize (int newWidth, int newHeight) override;

	virtual int			GetWidth () const override;
	virtual int			GetHeight () const override;

	virtual void		BeginDraw () override;
	virtual void		EndDraw () override;

	virtual bool		NeedToDraw (ItemPreviewMode) override;

	virtual void		DrawLine (const NUIE::Point& /*beg*/, const NUIE::Point& /*end*/, const NUIE::Pen& /*pen*/) override;
	virtual void		DrawBezier (const NUIE::Point& /*p1*/, const NUIE::Point& /*p2*/, const NUIE::Point& /*p3*/, const NUIE::Point& /*p4*/, const NUIE::Pen& /*pen*/) override;

	virtual void		DrawRect (const NUIE::Rect& /*rect*/, const NUIE::Pen& /*pen*/) override;
	virtual void		FillRect (const NUIE::Rect& rect, const NUIE::Color& color) override;

	virtual void		DrawEllipse (const NUIE::Rect& /*rect*/, const NUIE::Pen& /*pen*/) override;
	virtual void		FillEllipse (const NUIE::Rect& /*rect*/, const NUIE::Color& /*color*/) override;

	virtual void		DrawFormattedText (const NUIE::Rect& /*rect*/, const NUIE::Font& /*font*/, const std::wstring& /*text*/, NUIE::HorizontalAnchor /*hAnchor*/, NUIE::VerticalAnchor /*vAnchor*/, const NUIE::Color& /*textColor*/) override;
	virtual NUIE::Size	MeasureText (const NUIE::Font& /*font*/, const std::wstring& /*text*/) override;
				 
	virtual bool		CanDrawIcon () override;
	virtual void		DrawIcon (const NUIE::Rect& /*rect*/, const NUIE::IconId& /*iconId*/) override;

private:
	int						width;
	int						height;
	SDL_Renderer*			renderer;
	std::string				fontPath;

	FontController			fontController;
	FontCache				fontCache;

	FontTextureController	fontTextureController;
	FontTextureCache		fontTextureCache;
};

#endif
