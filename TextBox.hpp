#ifndef SPICE_TEXTBOX_HPP
#define SPICE_TEXTBOX_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu.hpp"
#include "BorderStyle.hpp"
#include "Font.hpp"

/* This is a text box. This textbox is nothing special. It
 * can change its font on command, though. It just displays text
 * immediately and can be shown or hidden. And it does automatic
 * line breaks. This class is useful for deriving text boxes that
 * do more useful things. Currently, it derives the DialogueTextBox
 * class (for talking to people) and the MenuTextBox class (for
 * selecting things in menus.) */
class TextBox : public sf::Drawable {
    public:
        bool hidden;

        TextBox(sf::Rect<int> dimensions) {
            hidden = true;
            size = dimensions;
            initialSize = size;
            offset = sf::Vector2f(5, 5);
            for (int i = 0; i < 8; i++) {
                borderSecs.push_back(sf::Sprite());
            }
            setBorderStyle(gborder);
            font = gfont;
        }

        void setText(std::string string) {
            letters.resize(string.length() * 4);
            letters.setPrimitiveType(sf::Quads);
            int nextX = 0;
            int nextY = 0;
            int totalChars = 0;

            std::string nextWord;
            int wordStart = 0;
            int nextWhitespace = 0;
            while (nextWhitespace >= 0) {
                nextWhitespace = std::min(string.find_first_of('\n', wordStart), string.find_first_of(' ', wordStart));
                nextWord = string.substr(wordStart, nextWhitespace - wordStart + 1);

                // find width of the next word
                int i;
                int pxWidth = 0;
                for (i = 0; i < nextWord.length(); i++) {
                    /* add up the widths of the individual letters in the word to find its width */
                    if (nextWord[i] == '\n') continue; // ignore newlines; they have no width
                    pxWidth += font.widths[font.decoder.find_first_of(nextWord[i], 0)];
                }

                if (nextX + pxWidth > size.width - 2 * offset.x) {
                    /* if next word doesn't fit on the line, go to the next line */
                    nextX = 0;
                    nextY += font.charHeight + 1;
                }

                for (i = 0; i < nextWord.length(); i++) {
                    if (nextWord[i] == '\n') {
                        nextX = 0;
                        nextY += font.charHeight + 1;
                        /* hide the character representing the newline by setting it to (0,0) --
                         * otherwise it would output random junk into the textbox */
                        letters[totalChars+i*4].texCoords = sf::Vector2f(0, 0);
                        letters[totalChars+i*4+1].texCoords = sf::Vector2f(0, 0);
                        letters[totalChars+i*4+2].texCoords = sf::Vector2f(0, 0);
                        letters[totalChars+i*4+3].texCoords = sf::Vector2f(0, 0);
                        letters[totalChars+i*4].position = sf::Vector2f(0, 0);
                        letters[totalChars+i*4+1].position = sf::Vector2f(0, 0);
                        letters[totalChars+i*4+2].position = sf::Vector2f(0, 0);
                        letters[totalChars+i*4+3].position = sf::Vector2f(0, 0);
                        break;
                    }

                    int texID = font.decoder.find_first_of(nextWord[i], 0);

                    if (texID < 0) {
                        std::cerr << "Character not in decoder string: " << nextWord[i] << std::endl;
                        continue;
                    }

                    letters[totalChars+i*4].texCoords = font.texLocs[texID*4].texCoords;
                    letters[totalChars+i*4+1].texCoords = font.texLocs[texID*4+1].texCoords;
                    letters[totalChars+i*4+2].texCoords = font.texLocs[texID*4+2].texCoords;
                    letters[totalChars+i*4+3].texCoords = font.texLocs[texID*4+3].texCoords;

                    letters[totalChars+i*4].position =
                            sf::Vector2f(offset.x + size.left + nextX,
                            offset.y + size.top + nextY);

                    letters[totalChars+i*4+1].position =
                            sf::Vector2f(offset.x + size.left + nextX + font.widths[texID],
                            offset.y + size.top + nextY);

                    letters[totalChars+i*4+2].position =
                            sf::Vector2f(offset.x + size.left + nextX + font.widths[texID],
                            offset.y + size.top + nextY + font.charHeight);

                    letters[totalChars+i*4+3].position =
                            sf::Vector2f(offset.x + size.left + nextX,
                            offset.y + size.top + nextY + font.charHeight);

                    nextX += font.widths[texID];
                }

                totalChars += nextWord.length() * 4;
                wordStart = nextWhitespace + 1;
            }

            text = string;
            dirtyPos = false;
        }

        virtual void append(std::string string) {
            text += string;
            setText(text);
        }

        void hide() {
            hidden = true;
        }

        virtual void show() {
            if (dirtyPos) {
                cleanPos();
            }
            hidden = false;
        }

        virtual void cleanPos() {
            setText(text);
            updateBorderPosition();
        }

        void setPosition(sf::Vector2u pos) {
            setPosition(pos.x, pos.y);
        }

        void setPosition(int x, int y) {
            size.left = x;
            size.top = y;
            dirtyPos = true;
        }

        void updatePosition(float cameraX, float cameraY, int scrwidth, int scrheight) {
            setPosition((int)cameraX - scrwidth/2 + initialSize.left, (int)cameraY - scrheight/2 + initialSize.top);
        }

        void setSize(sf::Vector2u size_) {
            setSize(size_.x, size_.y);
        }

        void setSize(int w, int h) {
            size.width = w;
            size.height = h;
            dirtyPos = true;
        }

        void setLines(int numLines) {
            size.height = numLines * (font.charHeight + 1) + 2 * offset.y - font.descenderHeight - 1;
        }

        sf::Rect<int> getSizeRect() {
            return size;
        }

        std::string getText() {
            return text;
        }

        void setOffset(float x, float y) {
            setOffset(sf::Vector2f(x, y));
        }

        void setOffset(sf::Vector2f offset_) {
            offset = offset_;
        }

        void setBorderStyle(BorderStyle bs) {
            border = bs;
            for (int i = 0; i < bs.textures.size(); i++) {
                borderSecs[i].setTexture(border.textures[i], true);
            }
            updateBorderPosition();
        }

        static void setGlobalBorderStyle(BorderStyle gborder_) {
            gborder = gborder_;
        }

        void setFont(Font font_) {
            font = font_;
        }

        static void setGlobalFont(Font gfont_) {
            gfont = gfont_;
        }

    protected:
        std::string text;           // text to display
        bool dirtyPos;              // has position/size been changed since it was last shown
        sf::VertexArray letters;    // positions of letters on screen
        sf::Rect<int> size;         // size/location of the text box
        sf::Rect<int> initialSize;  // initial dimensions of text box
        sf::Vector2f offset;        // the offset of the letters from the upper-left corner
        Font font;                  // the font with which it shall be drawn
        static Font gfont;          // the default ("global") font to be used
        BorderStyle border;         // the style of border that will be drawn around the text box
        static BorderStyle gborder; // the global border style for all (newly-created) windows
        std::vector<sf::Sprite> borderSecs; // the individual sprites representing components of the border

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if (!hidden) {
                // draw a background
                sf::RectangleShape bg(sf::Vector2f(size.width, size.height));
                bg.setPosition(sf::Vector2f(size.left, size.top));
                bg.setFillColor(sf::Color(sf::Color::Black));
                target.draw(bg, states);
                // draw border
                drawBorder(target, states);
                // apply texture
                states.texture = &font.image;
                // draw vertex array
                target.draw(letters, states);
            }
        }

        virtual void drawBorder(sf::RenderTarget& target, sf::RenderStates states) const {
            if (border.exists) {
                // draw border
                for (int i = 0; i < borderSecs.size(); i++) {
                    target.draw(borderSecs[i], states);
                }
            }
        }

        void updateBorderPosition() {
            borderSecs[0].setPosition(size.left - borderSecs[0].getLocalBounds().width,
                    size.top - borderSecs[0].getLocalBounds().height);   // Upper left
            borderSecs[1].setPosition(size.left + size.width,
                    size.top - borderSecs[0].getLocalBounds().height);   // Upper right
            borderSecs[2].setPosition(size.left - borderSecs[2].getLocalBounds().width,
                    size.top + size.height);                             // Bottom left
            borderSecs[3].setPosition(size.left + size.width,
                    size.top + size.height);                             // Bottom right
            borderSecs[4].setPosition(size.left, size.top - borderSecs[4].getLocalBounds().height);
            borderSecs[4].setScale((float)size.width / borderSecs[4].getLocalBounds().width, 1.0f);    // Top
            borderSecs[5].setPosition(size.left, size.top + size.height);
            borderSecs[5].setScale((float)size.width / borderSecs[5].getLocalBounds().width, 1.0f);    // Bottom
            borderSecs[6].setPosition(size.left - borderSecs[6].getLocalBounds().width, size.top);
            borderSecs[6].setScale(1.0f, (float)size.height / borderSecs[6].getLocalBounds().height);  // Left
            borderSecs[7].setPosition(size.left + size.width, size.top);
            borderSecs[7].setScale(1.0f, (float)size.height / borderSecs[7].getLocalBounds().height);  // Right
        }
};

Font TextBox::gfont; // ??? ??  ? c++?
BorderStyle TextBox::gborder;

#endif // SPICE_TEXTBOX_HPP
