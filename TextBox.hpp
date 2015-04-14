#include <iostream>
#include <SFML/Graphics.hpp>

/* This is a text box. This textbox is nothing special. It
 * can change its font on command, though. It just displays text
 * immediately and can be shown or hidden. And it does automatic
 * line breaks. This class is useful for deriving text boxes that
 * do more useful things. It already has derived the TypewriterTextBox
 * class, and I hope to also derive a 'menu text box' class as well. */
class TextBox : public sf::Drawable {
    public:
        bool hidden;

        TextBox(sf::Rect<int> dimensions) {
            hidden = true;
            size = dimensions;
        }

        void setText(std::string string) {
            string.append(" ");
            letters.resize(string.length() * 4);
            letters.setPrimitiveType(sf::Quads);
            int nextX = 0;
            int nextY = 0;
            int totalChars = 0;

            std::string nextWord;
            int wordStart = 0;
            int nextSpace = 0;
            while (nextSpace >= 0) {
                nextSpace = string.find_first_of(' ', wordStart);
                nextWord = string.substr(wordStart, nextSpace - wordStart + 1);

                // find width of the next word
                int i;
                int pxWidth = 0;
                for (i = 0; i < nextWord.length(); i++) {
                    pxWidth += widths[decoder.find_first_of(nextWord[i], 0)];
                }

                if (nextX + pxWidth > size.width) {
                    nextX = 0;
                    nextY += charHeight + 1;
                }

                for (i = 0; i < nextWord.length(); i++) {
                    int texID = decoder.find_first_of(nextWord[i], 0);

                    if (texID < 0) {
                        std::cerr << "Character not in decoder string: " << nextWord[i] << std::endl;
                        continue;
                    }

                    letters[totalChars+i*4].texCoords = texLocs[texID*4].texCoords;
                    letters[totalChars+i*4+1].texCoords = texLocs[texID*4+1].texCoords;
                    letters[totalChars+i*4+2].texCoords = texLocs[texID*4+2].texCoords;
                    letters[totalChars+i*4+3].texCoords = texLocs[texID*4+3].texCoords;

                    letters[totalChars+i*4].position = sf::Vector2f(size.left + nextX, size.top + nextY);
                    letters[totalChars+i*4+1].position = sf::Vector2f(size.left + nextX + widths[texID], size.top + nextY);
                    letters[totalChars+i*4+2].position = sf::Vector2f(size.left + nextX + widths[texID], size.top + nextY + charHeight);
                    letters[totalChars+i*4+3].position = sf::Vector2f(size.left + nextX, size.top + nextY + charHeight);

                    nextX += widths[texID];
                }

                totalChars += nextWord.length() * 4;
                wordStart = nextSpace + 1;
            }

            text = string;
            dirtyPos = false;
        }

        void hide() {
            hidden = true;
        }

        void show() {
            if (dirtyPos) {
                setText(text);
            }
            hidden = false;
        }

        void setPosition(sf::Vector2u pos) {
            setPosition(pos.x, pos.y);
        }

        void setPosition(int x, int y) {
            size.left = x;
            size.top = y;
            dirtyPos = true;
        }

        void setSize(sf::Vector2u size_) {
            setSize(size_.x, size_.y);
        }

        void setSize(int w, int h) {
            size.width = w;
            size.height = h;
            dirtyPos = true;
        }

        sf::Rect<int> getSizeRect() {
            return size;
        }

        std::string getText() {
            return text;
        }

        bool setFont(const std::string& filename,   // the font filename
                int char_height,                    // height of characters
                int* char_widths,                   // width of each character
                const std::string& decoder_string){ // string with characters in order as image
            if (!fontImage.loadFromFile(filename)) {
                return false;
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

            return true;
        }

    protected:
        std::string text;
        bool dirtyPos;
        sf::Texture fontImage;
        sf::VertexArray texLocs;
        sf::VertexArray letters;
        sf::Rect<int> size;
        int* widths;
        int charHeight;
        std::string decoder;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if (!hidden) {
                // draw a background
                sf::RectangleShape bg(sf::Vector2f(size.width + 2, size.height + 2));
                bg.setPosition(sf::Vector2f(size.left - 1, size.top - 1));
                bg.setFillColor(sf::Color(sf::Color::Black));
                target.draw(bg, states);
                // apply texture
                states.texture = &fontImage;
                // draw vertex array
                target.draw(letters, states);
            }
        }
};

/* The TypewriterTextBox is nice because it displays text one letter at a time.
 * It takes a constructor parameter 'speed' which is the number of letters per
 * second to be displayed. It is otherwise basically the same as the regular
 * TextBox. The letters reset whenever it is re-shown. */
class TypewriterTextBox : public TextBox {
    public:
        TypewriterTextBox(sf::Rect<int> dimensions, float lettersPerSecond) : TextBox(dimensions) {
            speed = lettersPerSecond;
            finished = false;
        }

        void setText(std::string string) {
            TextBox::setText(string);
            lineTime.restart();
        }

        void show() {
            TextBox::show();
            lineTime.restart();
            finished = false;
        }

        bool finished;

        void update() {
            if (!hidden && !finished) {
                // seconds * letters/sec = # letters (unit cancellation!)
                int dispLineLength = (int)(lineTime.getElapsedTime().asSeconds() * speed);
                if (dispLineLength <= text.length()) {
                    // (we have to multiply by 4 because there are 4 vertices per letter)
                    dispLetters.resize(dispLineLength * 4);
                    finished = false;
                } else {
                    // this means we've already written out all the letters
                    dispLetters.resize(text.length() * 4);
                    finished = true;
                }
                dispLetters.setPrimitiveType(sf::Quads);
                // copy the ones that need to be displayed into the vertex array
                for (int i = 0; i < dispLetters.getVertexCount(); i++) {
                    dispLetters[i] = letters[i];
                }
            }
        }

    protected:
        sf::Clock lineTime;
        // only the letters that are displayed
        sf::VertexArray dispLetters;
        float speed;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if (!hidden) {
                // draw background
                sf::RectangleShape bg(sf::Vector2f(size.width + 2, size.height + 2));
                bg.setPosition(sf::Vector2f(size.left - 1, size.top - 1));
                bg.setFillColor(sf::Color(sf::Color::Black));
                target.draw(bg, states);
                // apply the texture
                states.texture = &fontImage;
                target.draw(dispLetters, states);
            }
        }
};
