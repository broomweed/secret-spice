using std::string;

template<typename T>
class Menu {
    public:
        Menu() { }

        std::vector<string> names;
        std::vector<T> items;

        void addItem(string name, T item) {
            names.push_back(name);
            items.push_back(item);
        }

        string getItemName(int index) {
            return names[index];
        }

        T getItemAction(int index) {
            return items[index];
        }
};
