#ifndef SPICE_TEXTBOX_HPP
#define SPICE_TEXTBOX_HPP

#include <iostream>
#include <SFML/Graphics.hpp>
#include "Menu.hpp"

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
            offset = sf::Vector2f(1, 1);
        }

        void setText(std::string string) {
            string.append("\n");
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
                    pxWidth += widths[decoder.find_first_of(nextWord[i], 0)];
                }

                if (nextX + pxWidth > size.width - offset.x) {
                    /* if next word doesn't fit on the line, go to the next line */
                    nextX = 0;
                    nextY += charHeight + 1;
                }

                for (i = 0; i < nextWord.length(); i++) {
                    if (nextWord[i] == '\n') {
                        nextX = 0;
                        nextY += charHeight + 1;
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

                    int texID = decoder.find_first_of(nextWord[i], 0);

                    if (texID < 0) {
                        std::cerr << "Character not in decoder string: " << nextWord[i] << std::endl;
                        continue;
                    }

                    letters[totalChars+i*4].texCoords = texLocs[texID*4].texCoords;
                    letters[totalChars+i*4+1].texCoords = texLocs[texID*4+1].texCoords;
                    letters[totalChars+i*4+2].texCoords = texLocs[texID*4+2].texCoords;
                    letters[totalChars+i*4+3].texCoords = texLocs[texID*4+3].texCoords;

                    letters[totalChars+i*4].position =
                            sf::Vector2f(offset.x + size.left + nextX,
                            offset.y + size.top + nextY);

                    letters[totalChars+i*4+1].position =
                            sf::Vector2f(offset.x + size.left + nextX + widths[texID],
                            offset.y + size.top + nextY);

                    letters[totalChars+i*4+2].position =
                            sf::Vector2f(offset.x + size.left + nextX + widths[texID],
                            offset.y + size.top + nextY + charHeight);

                    letters[totalChars+i*4+3].position =
                            sf::Vector2f(offset.x + size.left + nextX,
                            offset.y + size.top + nextY + charHeight);

                    nextX += widths[texID];
                }

                totalChars += nextWord.length() * 4;
                wordStart = nextWhitespace + 1;
            }

            text = string;
            dirtyPos = false;
        }

        void hide() {
            hidden = true;
        }

        void show() {
            if (dirtyPos) {
                cleanPos();
            }
            hidden = false;
        }

        virtual void cleanPos() {
            setText(text);
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

        void setOffset(float x, float y) {
            setOffset(sf::Vector2f(x, y));
        }

        void setOffset(sf::Vector2f offset_) {
            offset = offset_;
        }

    protected:
        std::string text;           // text to display
        bool dirtyPos;              // has position/size been changed since it was last shown
        sf::Texture fontImage;      // the letters of the font
        sf::VertexArray texLocs;    // locations of font letter rectangles
        sf::VertexArray letters;    // positions of letters on screen
        sf::Rect<int> size;         // size/location of the text box
        sf::Rect<int> initialSize;  // initial dimensions of text box
        int* widths;                // the widths of each individual character
        int charHeight;             // the height of every character
        std::string decoder;        // the order of the characters within the texture
        sf::Vector2f offset;        // the offset of the letters from the upper-left corner

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if (!hidden) {
                // draw a background
                sf::RectangleShape bg(sf::Vector2f(size.width, size.height));
                bg.setPosition(sf::Vector2f(size.left, size.top));
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
            lineFinished = false;
        }

        void setText(std::string string) {
            TextBox::setText(string);
            lineTime.restart();
            lineFinished = false;
        }

        void show() {
            TextBox::show();
            lineTime.restart();
            lineFinished = false;
        }

        bool lineFinished;

        void update() {
            if (!hidden && !lineFinished) {
                // seconds * letters/sec = # letters (unit cancellation!)
                int dispLineLength = (int)(lineTime.getElapsedTime().asSeconds() * speed);
                if (dispLineLength <= text.length()) {
                    // (we have to multiply by 4 because there are 4 vertices per letter)
                    dispLetters.resize(dispLineLength * 4);
                    lineFinished = false;
                } else {
                    // this means we've already written out all the letters
                    dispLetters.resize(text.length() * 4);
                    lineFinished = true;
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

/* This is an extension of the TypewriterTextBox that has a Dialogue object attached,
   and thus supports sequences of multiple lines. */
class DialogueTextBox : public TypewriterTextBox {
    public:
        DialogueTextBox (sf::Rect<int> dimensions,
                float lettersPerSecond,
                Dialogue lines_,
                Animation continueArrow_)
                : TypewriterTextBox(dimensions, lettersPerSecond) {
            lines = lines_;
            continueArrow = continueArrow_;
        }

        DialogueTextBox (sf::Rect<int> dimensions,
                float lettersPerSecond,
                Animation continueArrow_)
                : TypewriterTextBox(dimensions, lettersPerSecond) {
            lines = Dialogue();
            continueArrow = continueArrow_;
        }

        void nextLine() {
            if (lines.hasMoreLines()) {
                setText(lines.getNextLine());
            } else {
                hide();
            }
        }

        void show() {
            TypewriterTextBox::show();
            lines.reset();
            nextLine();
        }

        void setSize(int w, int h) {
            TypewriterTextBox::setSize(w, h);
        }

        void setPosition(int x, int y) {
            TypewriterTextBox::setPosition(x, y);
        }

        void cleanPos() {
            TypewriterTextBox::cleanPos();
            continueArrow.setPosition(size.left + size.width + offset.x - continueArrow.getLocalBounds().width - 1,
                    size.top + size.height - continueArrow.getLocalBounds().height - 1);
        }


        void setDialogue(Dialogue lines_) {
            lines = lines_;
        }

        void update() {
            if (!hidden) {
                TypewriterTextBox::update();
                continueArrow.update();
            }
        }

    protected:
        Dialogue lines;             // the lines this textbox contains
        Animation continueArrow;    // the little arrow that will show when there are more lines
        sf::Texture contTexture;    // the texture of the continue arrow

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if (!hidden) {
                TypewriterTextBox::draw(target, states);
                if (lines.hasMoreLines() && lineFinished) {
                    target.draw(continueArrow);
                }
            }
        }
};

/* a generic version of the MenuTextBox class with no templating,
 * so that we can handle nitty-gritty stuff like selecting up/down without
 * having to write separate code for each possible type */
class MenuTextBoxBase : public TextBox {
    public:
        MenuTextBoxBase(sf::Rect<int> dimensions,
                Animation selectorArrow_)
                : TextBox(dimensions) {
            selectorArrow = selectorArrow_;
            setOffset(selectorArrow.getLocalBounds().width + 2, 1);
            setSelection(0);
            menuLength = 1;
        }

        void setSelection(int i) {
            selectionIndex = i;
            cleanPos();
        }

        void selectNext() {
            if (selectionIndex == menuLength - 1) {
                setSelection(0);
            } else {
                setSelection(selectionIndex + 1);
            }
        }

        void selectPrev() {
            if (selectionIndex == 0) {
                setSelection(menuLength - 1);
            } else {
                setSelection(selectionIndex - 1);
            }
        }

        void cleanPos() {
            TextBox::cleanPos();
            selectorArrow.setPosition(size.left + 1, size.top + getSelection() * (charHeight + 1) + 1);
        }

        int getSelection() {
            return selectionIndex;
        }

    protected:
        Animation selectorArrow;
        int selectionIndex;
        int menuLength;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if (!hidden) {
                TextBox::draw(target, states);
                target.draw(selectorArrow);
            }
        }
};

template<typename T>
class MenuTextBox : public MenuTextBoxBase {
    public:
        MenuTextBox<T>(sf::Rect<int> dimensions,
                Animation selectorArrow_)
                : MenuTextBoxBase(dimensions, selectorArrow_) { }

        void setMenu(Menu<T> menu_) {
            menu = menu_;
            std::string textToSet = "";
            for (int i = 0; i < menu.names.size(); i++) {
                textToSet += menu.getItemName(i) + "\n";
            }
            setText(textToSet);
            menuLength = menu.items.size(); // now the base class knows about the length
        }

        T getSelectedItem() {
            return menu.items[selectionIndex];
        }

    protected:
        Menu<T> menu;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            MenuTextBoxBase::draw(target, states);
        }
};
#endif // SPICE_TEXTBOX_HPP
