#ifndef SDL2_CACHES_HPP
#define SDL2_CACHES_HPP

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

#include "NE_Cache.hpp"
#include "NUIE_DrawingCacheKeys.hpp"

class FontTextureCacheKey
{
public:
	FontTextureCacheKey (const std::wstring& text, const NUIE::ColorCacheKey& color, int size);

	bool	operator== (const FontTextureCacheKey& rhs) const;
	bool	operator!= (const FontTextureCacheKey& rhs) const;

	std::wstring			text;
	NUIE::ColorCacheKey		color;
	int						size;
};

namespace std
{
	template <>
	struct hash<FontTextureCacheKey>
	{
		size_t operator() (const FontTextureCacheKey& key) const noexcept
		{
			return std::hash<std::wstring> {} (key.text) + 24593 * std::hash<NUIE::ColorCacheKey> {} (key.color) + 49157 * key.size;
		}
	};
}

using FontCache = NE::Cache<int, TTF_Font*>;
using FontTextureCache = NE::Cache<FontTextureCacheKey, SDL_Texture*>;

class FontController : public FontCache::Controller
{
public:
	FontController (const std::string& fontPath);

	virtual TTF_Font*	CreateValue (const int& key) override;
	virtual void		DisposeValue (TTF_Font*& value) override;

private:
	std::string			fontPath;
};

class FontTextureController : public FontTextureCache::Controller
{
public:
	FontTextureController (SDL_Renderer* renderer, FontCache& fontCache);

	virtual SDL_Texture*	CreateValue (const FontTextureCacheKey& key) override;
	virtual void			DisposeValue (SDL_Texture*& value) override;

private:
	SDL_Renderer*		renderer;
	FontCache&			fontCache;
};

#endif
