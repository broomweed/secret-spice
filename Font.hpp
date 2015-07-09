/* This is a class that collects a bunch of data
 * to describe a font. Formerly, this code lived
 * in the TextBox class. */
class Font {
    public:
        Font() { }

        Font(const std::string& filename,   // the font filename
                int char_height,                    // height of characters
                int* char_widths,                   // width of each character
                const std::string& decoder_string){ // string with characters in order as image
            if (!image.loadFromFile(filename)) {
                throw -3;
            }
            decoder = decoder_string; // it's like a decoder ring ;)
            widths = char_widths;
            charHeight = char_height;
            texLocs.setPrimitiveType(sf::Quads);
            texLocs.resize(decoder.length() * 4);
            int totalWidth = 0;
            for (int i = 0; i < decoder.length(); i++) {
                texLocs[i*4].texCoords = sf::Vector2f(totalWidth, 0);
                texLocs[i*4+1].texCoords = sf::Vector2f(totalWidth + char_widths[i], 0);
                texLocs[i*4+2].texCoords = sf::Vector2f(totalWidth + char_widths[i], char_height);
                texLocs[i*4+3].texCoords = sf::Vector2f(totalWidth, char_height);
                totalWidth += char_widths[i];
            }
        }

        sf::Texture image;      // the letters of the font
        sf::VertexArray texLocs;    // locations of font letter rectangles
        int* widths;                // the widths of each individual character
        int charHeight;             // the height of every character
        std::string decoder;        // the order of the characters within the texture
};
