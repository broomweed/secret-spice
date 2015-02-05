#include <vector>
#include <SFML/Graphics.hpp>
#include "GameObject.hpp"

/* This is a scene that contains a series of GameObjects. This is what handles
 * drawing in order of drawDepth. */
class Scene : public sf::Drawable {
    public:
        Scene(GameObject* objs_, int size) {
            for (int i = 0; i < size; i++) {
                add(&objs_[i]);
            }
        }

        Scene() {
            // pass
        }

        void add(GameObject *obj) {
            objs.push_back(obj);
            obj->sceneIndex = objs.size()-1;
        }

        std::vector<GameObject*> getObjs() {
            return objs;
        }

        void move_sprite(GameObject obj, float hmove, float vmove) {
            int index = obj.sceneIndex;
            objs[index]->move(hmove, vmove);
            for (int i = 0; i < objs.size(); i++) {
                if ((objs[index]->getPosition().y < objs[i]->getPosition().y
                            && objs[index]->drawDepth > objs[i]->drawDepth)
                        || (objs[index]->getPosition().y > objs[i]->getPosition().y
                            && objs[index]->drawDepth < objs[i]->drawDepth)) {
                    unsigned int tmp = objs[index]->drawDepth;
                    objs[index]->drawDepth = objs[i]->drawDepth;
                    objs[i]->drawDepth = tmp;
                }
            }
        }

    protected:
        std::vector<GameObject*> objs;

        virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const {
            /* there is DEFINITELY a more efficient way to do this */
            /* TODO: figure this out. Maybe sort the list of thingies when
               they are added, and then exchange objects only when they swap depths?
               (i.e. be the gatekeeper for swapping depths in a scene) But then the
               drawing depths should probably be contained in this class, not in
               the objects themselves. Which could be done, with like:
                    struct depthObj { uint depth; GameObject* obj; }
               or something. Then objs would be a std::vector<struct depthObj>. */
            /* (see, I'm learning this c++ stuff!!) */
            /* OR, Or an alternative way to do this would be to have an 'update'
               function that sorts the list, but is only called when two objects
               switch depths. This would probably be slower, but it would allow
               the drawing-depths to stay inside of the objects themselves. */
            /* IDK, I'm more partial to the first one atm. I'll figure it out */
            unsigned int maxDepth = 0;
            for (int i = 0; i < objs.size(); i++) {
                maxDepth = std::max(maxDepth, objs[i]->drawDepth);
            }
            for (int i = 0; i <= maxDepth; i++) {
                for (int j = 0; j < objs.size(); j++) {
                    if (objs[j]->drawDepth == i) {
                        target.draw(objs[j]->getSprite(), states);
                    }
                }
            }
        }
};