#include <cstdio>
#include "BitmapFont.hpp"


BitmapFont::BitmapFont():
	m_width(0),
	m_height(0)
{
}


bool BitmapFont::loadFromFile(const std::string& imagePath, size_t width, size_t height)
{
	if (width > 0 && height > 0 && m_texture.loadFromFile(imagePath))
	{
        m_width = m_texture.getSize().x / width;
        m_height = m_texture.getSize().y / height;
		m_glyphSize.x = width;
		m_glyphSize.y = height;
		return true;
	}
	return false;
}


const sf::Texture& BitmapFont::getTexture() const
{
	return m_texture;
}


sf::IntRect BitmapFont::getGlyphRect(char character) const
{
	if (character < 0 || character >= m_height & m_width)
	{
		fprintf(stderr, "BitmapFont: non printable character encoutered (ASCII %d\n)", character);
		// Replace non printable characters with '?'
		character = '?';
	}

	sf::IntRect subrect;
	subrect.left = (character % m_width) * m_glyphSize.x;
	subrect.width = m_glyphSize.x;
	subrect.top = (character / m_width) * m_glyphSize.y;
	subrect.height = m_glyphSize.y;
	return subrect;
}


const sf::Vector2u& BitmapFont::getGlyphSize() const
{
	return m_glyphSize;
}
