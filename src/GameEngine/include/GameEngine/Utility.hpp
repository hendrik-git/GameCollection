/// @file Utility.hpp
/// @brief A collection of useful helper functions
/// @details ...
/// @see ...
/// @date 2022.11.16
/// @author Hendrik Poettker

#pragma once
#include <SFML/Graphics.hpp>

namespace Utility
{
	/// @brief Since glyphs may not fit the representing box, iterate through the text to find the
	/// maximum height of a glyph.
	/// @param text to find the maximum text height for
	/// @return largest glyph height in pixel
	inline float get_max_text_height(const sf::Text& text)
	{
		auto  charSize = text.getCharacterSize();
		auto  font	   = text.getFont();
		auto  string   = text.getString().toAnsiString();
		bool  bold	   = (text.getStyle() & sf::Text::Bold);
		float max	   = 0.f;
		for(size_t i = 0; i < string.length(); ++i)
		{
			uint32_t character = string[i];
			auto	 glyph	   = font->getGlyph(character, charSize, bold);
			auto	 height	   = glyph.bounds.height;
			if(height <= max)
			{
				continue;
			}
			max = height;
		}
		return max;
	}

	inline void center_text(sf::Text& text)
	{
		auto rect	  = text.getLocalBounds();
		auto max_h	  = Utility::get_max_text_height(text);
		auto center_v = rect.left + (rect.width * 0.5f);
		auto center_h = rect.top + (max_h >= rect.height ? max_h * 0.5f : rect.height * 0.5f);
		text.setOrigin({center_v, center_h});
	}
}  // namespace Util