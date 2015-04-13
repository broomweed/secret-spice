#include <SFML/Graphics.hpp>

/* This is a TileMap. I stole part of this code from the SFML tutorial. It takes
 * a filename for the tiles, a Vector representing the size of each tile, an array
 * of tile ID integers, an integer representing the horizontal dimension in tiles,
 * and an integer representing the vertical dimension in tiles. */
class TileMap : public sf::Drawable, public sf::Transformable {
    public:
        bool load(const std::string& filename, sf::Vector2u size, const int* tiles, unsigned int width_, unsigned int height_) {
            if (!m_tileset.loadFromFile(filename)) {
                return false;
            }

            tile_ints = tiles;

            width = width_;
            height = height_;
            m_vertices.setPrimitiveType(sf::Quads); // it's a bunch of rectangles
            m_vertices.resize(width * height * 4); // 4 things per tile, w*h tiles total
            tile_size = size;

            // map the tile numbers to locations in the texture
            for (unsigned int i = 0; i < width; i++) {
                for (unsigned int j = 0; j < height; j++) {
                    int tile_number = tiles[i + j*width];   // which tile type are we talking about?

                    // find its position in the tile texture
                    int tex_x = tile_number % (m_tileset.getSize().x / tile_size.x);
                    int tex_y = tile_number / (m_tileset.getSize().x / tile_size.x); // integer division so it works

                    // find where to put the new quad
                    sf::Vertex* quad = &m_vertices[(i + j*width) * 4];

                    // define where it will be drawn
                    quad[0].position = sf::Vector2f(i * tile_size.x, j * tile_size.y);
                    quad[1].position = sf::Vector2f((i+1) * tile_size.x, j * tile_size.y);
                    quad[2].position = sf::Vector2f((i+1) * tile_size.x, (j+1) * tile_size.y);
                    quad[3].position = sf::Vector2f(i * tile_size.x, (j+1) * tile_size.y);

                    // and define the corners of the part of the texture that it maps to
                    quad[0].texCoords = sf::Vector2f(tex_x * tile_size.x, tex_y * tile_size.y);
                    quad[1].texCoords = sf::Vector2f((tex_x+1) * tile_size.x, tex_y * tile_size.y);
                    quad[2].texCoords = sf::Vector2f((tex_x+1) * tile_size.x, (tex_y+1) * tile_size.y);
                    quad[3].texCoords = sf::Vector2f(tex_x * tile_size.x, (tex_y+1) * tile_size.y);
                }
            }

            return true;
        }

        int tile_id_at_point(sf::Vector2f point) {
            if (point.x >= width * tile_size.x || point.y >= height * tile_size.y
                    || point.y < 0 || point.x < 0) {
                return 0;
            }
            // find the right vertex (convert to tile coordinates by dividing by tile size, then find it in the array based on that)
            int vertnum = (int)(point.x / tile_size.x) + width*((int)(point.y / tile_size.y));
            if (vertnum > width * height) {
                // trying to go outside the border
                return 0;
            }
            // otherwise just look it up in the array
            return tile_ints[vertnum];
        }

        unsigned int width;
        unsigned int height;
        sf::Vector2u tile_size;

    private:
        sf::VertexArray m_vertices;
        sf::Texture m_tileset;
        const int* tile_ints;
        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            // apply transform
            states.transform *= getTransform();
            // apply texture
            states.texture = &m_tileset;
            // draw vertex array
            target.draw(m_vertices, states);
        }
};
