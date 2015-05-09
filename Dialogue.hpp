#ifndef SPICE_DIALOGUE_HPP
#define SPICE_DIALOGUE_HPP
#include <vector>

using std::string;

/* This class represents a series of lines that a character can have.
   Lines are represented as std::strings. */
class Dialogue {
    public:
        Dialogue (std::vector<string> lines_) {
            lines = lines_;
            index = 0;
        }

        Dialogue () {
            index = 0;
        }

        Dialogue (string line) {
            lines.push_back(line);
            index = 0;
        }

        void addLine(string line) {
            lines.push_back(line);
        }

        string getLine(int i) const {
            if (i < lines.size()) {
                return lines[i];
            } else {
                return string("");
            }
        }

        string getNextLine() {
            return getLine(index++);
        }

        int numLines() const {
            return lines.size();
        }

        void reset() {
            index = 0;
        }

        bool hasMoreLines() const {
            return (index < lines.size());
        }

    protected:
        std::vector<string> lines;
        int index;
};
#endif // SPICE_DIALOGUE_HPP
