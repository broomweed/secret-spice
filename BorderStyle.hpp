/* This represents a style of border (flavor of window) that can
 * be applied to a text box. It provides a set of textures. The
 * text box provides its own set of sprites that use these textures,
 * because they have to be drawn in different places for different
 * text boxes. */
class BorderStyle {
    public:
        BorderStyle() {
            exists = false;
        }

        bool exists;

        bool loadFromFile(std::string folderName) {
            int error = 0;
            std::vector<std::string> filenames;
            filenames.push_back("ul"); // Upper left corner
            filenames.push_back("ur"); // Upper right corner
            filenames.push_back("dl"); // Bottom left corner
            filenames.push_back("dr"); // Bottom right corner
            filenames.push_back("u");  // Top border
            filenames.push_back("d");  // Bottom border
            filenames.push_back("l");  // Left border
            filenames.push_back("r");  // Right border
            for (int i = 0; i < filenames.size(); i++) {
                sf::Texture tex;
                if (!tex.loadFromFile(folderName + "/" + filenames[i] + ".png")) {
                    std::cerr << "Failed to load image at " << folderName << "/" << filenames[i] << ".png." << std::endl;
                    std::cerr << "(maybe you put a slash after the folder name?)" << std::endl;
                    error = 1;
                }
                textures.push_back(tex);
            }
            if (error) return false; /* I'm doing error handling this way
                                        so that it will notify about all missing
                                        images rather than only one at a time
                                        (which it would if the return was in the
                                        loop.) */
            exists = true;

            return true;
        }

        std::vector<sf::Texture> textures;
};
