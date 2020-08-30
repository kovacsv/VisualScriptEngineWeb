#include "SDL2Caches.hpp"
#include "NE_StringUtils.hpp"

FontTextureCacheKey::FontTextureCacheKey (const std::wstring& text, const NUIE::ColorCacheKey& color, int size) :
	text (text),
	color (color),
	size (size)
{

}

bool FontTextureCacheKey::operator== (const FontTextureCacheKey& rhs) const
{
	return text == rhs.text && color == rhs.color && size == rhs.size;
}

bool FontTextureCacheKey::operator!= (const FontTextureCacheKey& rhs) const
{
	return !operator== (rhs);
}

FontController::FontController (const std::string& fontPath) :
	FontCache::Controller (),
	fontPath (fontPath)
{

}

TTF_Font* FontController::CreateValue (const int& key)
{
	return TTF_OpenFont (fontPath.c_str (), key);
}

void FontController::DisposeValue (TTF_Font*& value)
{
	TTF_CloseFont (value);
}

FontTextureController::FontTextureController (SDL_Renderer* renderer, FontCache& fontCache) :
	FontTextureCache::Controller (),
	renderer (renderer),
	fontCache (fontCache)
{

}

SDL_Texture* FontTextureController::CreateValue (const FontTextureCacheKey& key)
{
	TTF_Font* ttfFont = fontCache.Get (key.size);
	std::string textStr = NE::WStringToString (key.text);
	SDL_Color sdlColor = { key.color.r, key.color.g, key.color.b, 255 };
	SDL_Surface* surface = TTF_RenderUTF8_Blended (ttfFont, textStr.c_str (), sdlColor);
	SDL_Texture* texture = SDL_CreateTextureFromSurface (renderer, surface);
	SDL_FreeSurface (surface);
	return texture;
}

void FontTextureController::DisposeValue (SDL_Texture*& value)
{
	SDL_DestroyTexture (value);
}
