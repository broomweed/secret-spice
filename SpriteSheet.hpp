#ifndef SPICE_SPRITESHEET_HPP
#define SPICE_SPRITESHEET_HPP
#include <SFML/Graphics.hpp>

using std::string;
using std::vector;

/* This is a really handy class. Just give it a texture (or even a filename) and
   2 numbers representing the number of sprites horizontally and vertically
   (these are slightly confusingly named 'width' and 'height,' I may change this
   later), and it will automatically generate a vector of sprites that all draw
   their texture coordinates from the same texture and are all positioned correctly.
   The one thing about this is that sprites in a sprite sheet have to be carefully
   positioned so that it doesn't mess up the offsets (e.g. I had to adjust the dan.png
   spritesheet and shift some things around or else the sprites would wiggle back
   and forth annoyingly.) */
class SpriteSheet {
    public:
        SpriteSheet(sf::Texture tex_,
                int width_,
                int height_) {
            setTexture(tex_, width_, height_);
        }

        SpriteSheet(string filename,
                int width_,
                int height_) {
            setTexture(filename, width_, height_);
        }

        void setTexture(string filename, int width_, int height_) {
            sf::Texture tex_;
            if (!tex_.loadFromFile(filename)) {
                std::cerr << "Couldn't load texture from file: " << filename << "!\n";
            }
            tex_.setSmooth(false);
            setTexture(tex_, width_, height_);
        }

        void setTexture(sf::Texture tex_, int width_, int height_) {
            width = width_;
            height = height_;
            sprites.reserve(width*height);
            tex = tex_;
            if (tex.getSize().x % width != 0) {
                std::cerr << "Sprite dimensions didn't quite properly work out x-wise.\n";
            }
            int spriteWidth = tex.getSize().x / width;
            if (tex.getSize().y % height != 0) {
                std::cerr << "Sprite dimensions didn't quite properly work out y-wise.\n";
            }
            int spriteHeight = tex.getSize().y / height;

            for (int j = 0; j < height; j++) {
                for (int i = 0; i < width; i++) {
                    sprites[j*width+i] = sf::Sprite(tex);
                    sprites[j*width+i].setTextureRect(sf::IntRect(i * spriteWidth, j * spriteHeight, spriteWidth, spriteHeight));
                }
            }
        }

        sf::Sprite getSprite(int x, int y) {
            if (x >= width || y >= height || x < 0 || y < 0) {
                std::cerr << "access out of bounds @ " << x << ", " << y << std::endl;
                return sf::Sprite();
            }
            return sprites[y*width+x];
        }

        sf::Sprite getSprite(int index) {
            return sprites[index];
        }

        int width;
        int height;

    protected:
        sf::Texture tex;
        vector<sf::Sprite> sprites;
};
#endif // SPICE_SPRITESHEET_HPP
