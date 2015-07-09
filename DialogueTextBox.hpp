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

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            if (!hidden) {
                TypewriterTextBox::draw(target, states);
                if (lines.hasMoreLines() && lineFinished) {
                    target.draw(continueArrow);
                }
            }
        }
};
