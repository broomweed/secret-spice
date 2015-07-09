#include "TextBox.hpp"

/* a generic version of the MenuTextBox class with no templating,
 * so that we can handle nitty-gritty stuff like selecting up/down without
 * having to write separate code for each possible type */
class MenuTextBoxBase : public TextBox {
    public:
        MenuTextBoxBase(sf::Rect<int> dimensions,
                Animation selectorArrow_)
                : TextBox(dimensions) {
            selectorArrow = selectorArrow_;
            setOffset(selectorArrow.getLocalBounds().width + 2, offset.y);
            setSelection(0);
            menuLength = 1;
            parent = NULL;
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
            selectorArrow.setPosition(size.left + 1,
                    size.top + offset.y + getSelection() * (font.charHeight + 1));
        }

        int getSelection() {
            return selectionIndex;
        }

        MenuTextBoxBase *getParent() {
            return parent;
        }

        void setParent(MenuTextBoxBase *parent_) {
            parent = parent_;
        }

        void setLength(int items) {
            menuLength = items;
            setLines(items);
        }

        virtual void show() {
            TextBox::show();
            setSelection(0);
        }

    protected:
        Animation selectorArrow;
        int selectionIndex;
        int menuLength;
        MenuTextBoxBase *parent;    // the menu that spawned it

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
                append(menu.getItemName(i) + "\n");
            }
            setLength(menu.items.size()); // now the base class knows about the length
            setSelection(0);
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
