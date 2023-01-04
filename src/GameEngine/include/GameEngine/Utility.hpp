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
			auto&	 glyph	   = font->getGlyph(character, charSize, bold);
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

	/// @brief
	/// @param tex
	/// @return
	inline auto get_circle_from(const sf::Texture& tex) -> sf::CircleShape
	{
		const auto		d_width	 = static_cast<float>(tex.getSize().x);
		const auto		d_height = static_cast<float>(tex.getSize().y);
		const auto		r_mean	 = (d_width + d_height) / 4.F;
		sf::CircleShape shape{r_mean};
		shape.setOrigin({r_mean, r_mean});
		return shape;
	}

	/// @brief
	/// @note The hitbox may not match the entity if rotations are involved
	/// @param tex
	/// @return
	inline auto get_rectangle_from(const sf::Texture& tex) -> sf::RectangleShape
	{
		const auto		   width  = static_cast<float>(tex.getSize().x);
		const auto		   height = static_cast<float>(tex.getSize().y);
		sf::RectangleShape shape{{width, height}};
		shape.setOrigin({width / 2.F, height / 2.F});
		return shape;
	}
}  // namespace Utility