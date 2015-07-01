#ifndef SPICE_SCENEMANAGER_HPP
#define SPICE_SCENEMANAGER_HPP
class Scene;

/* This class is super simple. It really only exists so that
 * you can know which scene is the active one at any given
 * time, and so that only the active scene gets updated.
 * however, since the scenes have to know which manager is
 * managing them, the file structure has gotten a little
 * complicated. Ah, well, c'est la vie. */
class SceneManager {
    public:
        Scene* currentScene;

        SceneManager() {
            currentScene = NULL;
        }

        SceneManager(Scene* scene);
        void setScene(Scene* scene);
        void update();
};
#endif //SPICE_SCENEMANAGER_HPP
