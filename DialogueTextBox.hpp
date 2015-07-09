#include "TextBox.hpp"

/* The TypewriterTextBox is nice because it displays text one letter at a time.
 * It takes a constructor parameter 'speed' which is the number of letters per
 * second to be displayed. It is otherwise basically the same as the regular
 * TextBox. The letters reset whenever it is re-shown. */
class TypewriterTextBox : public TextBox {
    public:
        TypewriterTextBox(sf::Rect<int> dimensions, float lettersPerSecond) : TextBox(dimensions) {
            speed = lettersPerSecond;
            lineFinished = false;
            totalTime = sf::Time::Zero;
            scrollAmount = 0;
        }

        void setText(std::string string) {
            TextBox::setText(string);
            lineTime.restart();
            lineFinished = false;
        }

        virtual void show() {
            TextBox::show();
            lineTime.restart();
            lineFinished = false;
        }

        virtual void hide() {
            TextBox::hide();
            totalTime = sf::Time::Zero;
        }

        bool lineFinished;

        virtual void append(std::string string) {
            TextBox::append(string);
            lineTime.restart();
            lineFinished = false;
            update();
        }

        void update() {
            if (!hidden && !lineFinished) {
                // seconds * letters/sec = # letters (unit cancellation!)
                int dispLineLength = (int)((totalTime + lineTime.getElapsedTime()).asSeconds() * speed);
                if (dispLineLength <= text.length()) {
                    // (we have to multiply by 4 because there are 4 vertices per letter)
                    dispLetters.resize(dispLineLength * 4);
                    lineFinished = false;
                } else {
                    // this means we've already written out all the letters
                    dispLetters.resize(text.length() * 4);
                    lineFinished = true;
                    totalTime += lineTime.getElapsedTime();
                }
                dispLetters.setPrimitiveType(sf::Quads);
                // copy the ones that need to be displayed into the vertex array
                for (int i = 0; i < dispLetters.getVertexCount(); i++) {
                    if (i < letters.getVertexCount() - 4) { // is it not the last letter?
                        while (letters[i+4].position.y > size.top + size.height + 1) {
                            // if the next one would be out of the box, scroll up
                            scrollUp();
                        }
                    }
                    dispLetters[i] = letters[i];
                }
            }
        }

        /* move all text up a line */
        void scrollUp() {
            for (int i = 0; i < letters.getVertexCount(); i++) {
                if (letters[i].position.y > size.top + font.charHeight + offset.y) {
                    letters[i].position.y -= font.charHeight + 1;
                } else {
                    letters[i].position.x = 0;
                    letters[i].position.y = 0;
                }
            }
        }

    protected:
        sf::Clock lineTime;
        sf::Time totalTime;
        // only the letters that are displayed
        sf::VertexArray dispLetters;
        float speed;
        int scrollAmount;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if (!hidden) {
                // draw background
                sf::RectangleShape bg(sf::Vector2f(size.width + 2, size.height + 2));
                bg.setPosition(sf::Vector2f(size.left - 1, size.top - 1));
                bg.setFillColor(sf::Color(sf::Color::Black));
                target.draw(bg, states);
                // draw border
                drawBorder(target, states);
                // apply the texture
                states.texture = &font.image;
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
            alreadyHasText = false;
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
                if (alreadyHasText) {
                    append("\n" + lines.getNextLine());
                } else {
                    setText(lines.getNextLine());
                    alreadyHasText = true;
                }
            } else {
                hide();
            }
        }

        void show() {
            alreadyHasText = false;
            TypewriterTextBox::show();
            lines.reset();
            nextLine();
        }

        void hide() {
            TypewriterTextBox::hide();
            setText("");
        }

        void setSize(int w, int h) {
            TypewriterTextBox::setSize(w, h);
        }

        void setPosition(int x, int y) {
            TypewriterTextBox::setPosition(x, y);
        }

        void cleanPos() {
            TypewriterTextBox::cleanPos();
            continueArrow.setPosition(size.left + size.width - offset.x - continueArrow.getLocalBounds().width,
                    size.top + size.height - continueArrow.getLocalBounds().height - offset.y);
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
        bool alreadyHasText;        // whether we should put a newline in front of the line to add

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if (!hidden) {
                TypewriterTextBox::draw(target, states);
                if (lines.hasMoreLines() && lineFinished) {
                    target.draw(continueArrow);
                }
            }
        }
};
