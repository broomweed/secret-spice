#include <iostream>
#include "Scene.hpp"

void Scene::initFadeRect() {
    fadeRect = sf::RectangleShape(sf::Vector2f(SCRWIDTH/2, SCRHEIGHT/2));
    fadeRect.setPosition(sf::Vector2f(0,0));
    fadeRect.setSize(sf::Vector2f(tilemap.width * tilemap.tile_size.x, tilemap.height * tilemap.tile_size.y));
    fadeRect.setFillColor(sf::Color(0,0,0,0)); // black
}

void Scene::setName(std::string name_) {
    name = name_;
}

void Scene::add(Thing *obj) {
    objs.push_back(obj);
    obj->parent = this;
    obj->sceneIndex = objs.size()-1;
    obj->drawDepth = num;
    for (int i = 0; i < objs.size(); i++) {
        // Swap objects if they passed behind/in front of each other
        if ((obj->getPosition().y < objs[i]->getPosition().y
                    && obj->drawDepth > objs[i]->drawDepth)
                || (obj->getPosition().y > objs[i]->getPosition().y
                    && obj->drawDepth < objs[i]->drawDepth)) {
            unsigned int tmp = obj->drawDepth;
            obj->drawDepth = objs[i]->drawDepth;
            objs[i]->drawDepth = tmp;
        }
    }
    num++;
}

/* This adds a copy of a GameObject to the scene.
   Useful for things that appear a lot over and over
   and are exactly the same except for position. */
void Scene::add_static(GameObject *obj, sf::Vector2f position) {
    add(new GameObject(obj->getAnimation(), position, obj->boxLoc));
    objs[objs.size()-1]->is_copy = true;
    objs[objs.size()-1]->drawDepth = num;
    num++;
}

void Scene::remove(Thing *obj) {
    objs.erase(objs.begin()+obj->sceneIndex);
    // rejigger scene indices
    for (int i = 0; i < objs.size(); i++) {
        objs[i]->sceneIndex = i;
    }
}

std::vector<Thing*> Scene::getObjs() {
    return objs;
}

void Scene::setMC(Thing *new_mc) {
    mainCharacter = objs[new_mc->sceneIndex];
}

Thing *Scene::getMC() {
    return mainCharacter;
}

void Scene::update() {
    if (visible) {
        if (active) {
            lastLoop = loopTime.getElapsedTime();
            loopTime.restart();
            if (fadeRect.getFillColor().a > 0) {
                // Fade in
                fadeRect.setFillColor(sf::Color(fadeRect.getFillColor().r, fadeRect.getFillColor().g, fadeRect.getFillColor().b,
                            std::max(0.0f, fadeRect.getFillColor().a - lastLoop.asSeconds() * 255))); // take 1 second to fade in
            } else {
                // Interphase
                for (int i = 0; i < objs.size(); i++) {
                    if (!objs[i]->active && !getMC()->hitTest(*objs[i])) {
                        // Re-enable things you aren't touching
                        objs[i]->active = true;
                    }
                    // Move the sprites around
                    move_sprite((*objs[i]), objs[i]->getSpeed().x * lastLoop.asSeconds() * 30,
                            objs[i]->getSpeed().y * lastLoop.asSeconds() * 30);
                    objs[i]->update();
                }
            }
        }
    } else {
        if (active) {
            active = false;
        }
    }
    if (transferring) {
        // Fade out
        lastLoop = loopTime.getElapsedTime();
        loopTime.restart();
        fadeRect.setFillColor(sf::Color(fadeRect.getFillColor().r, fadeRect.getFillColor().g, fadeRect.getFillColor().b,
                    std::min(255.0f, fadeRect.getFillColor().a + lastLoop.asSeconds() * 255))); // take 1 second to fade out
    }
    if (transferring && fadeRect.getFillColor().a >= 255) {
        // Hand off to the next scene
        Thing *mc = getMC();
        visible = false;
        remove(mc);
        mc->face(targetDir);
        mc->setPosition(destCoords.x, destCoords.y);
        mc->stopMoving();
        transferring->setVisible();
        transferring->add(mc);
        transferring->setMC(mc);
        for (int i = 0; i < transferring->getNumObjs(); i++) {
            // disable object if you're standing on top of it
            // immediately upon entering a scene
            if (mc->hitTest(*transferring->getObj(i))) {
                if (i == mc->sceneIndex) continue; // except itself
                transferring->getObj(i)->active = false;
            }
        }
        transferring->fadeRect.setFillColor(sf::Color(
                    fadeRect.getFillColor().r, fadeRect.getFillColor().g, fadeRect.getFillColor().b, 255));
        parent->setScene(transferring);
        transferring = NULL;
    }
}

void Scene::move_sprite(Thing& obj, float hmove, float vmove) {
    int index = obj.sceneIndex;
    objs[index]->turn(hmove, vmove);
    if (hmove != 0.0f || vmove != 0.0f) {
        objs[index]->checked = NULL;
        /* Check if object is touching other objects; if it is, don't move it */
        for (int i = 0; i < objs.size(); i++) {
            if (i == index) continue; // obviously it's always touching itself
            if (objs[i]->hitTest(sf::Rect<float>(objs[index]->absLoc.left + hmove,
                            objs[index]->absLoc.top, objs[index]->absLoc.width, objs[index]->absLoc.height))) {
                if (objs[i]->active && objs[i]->tangible) {
                    /*std::cout << "Setting xspeed to 0." << std::endl;
                    objs[index]->setSpeed(sf::Vector2f(objs[index]->getSpeed().x, 0.0f));*/
                    hmove = 0.0f;
                    objs[index]->checked = objs[i];
                    objs[i]->onTouch();
                }
            }
            if (objs[i]->hitTest(sf::Rect<float>(objs[index]->absLoc.left,
                            objs[index]->absLoc.top + vmove,
                            objs[index]->absLoc.width, objs[index]->absLoc.height))) {
                if (objs[i]->active && objs[i]->tangible) {
                    /*std::cout << "Setting yspeed to 0." << std::endl;
                    objs[index]->setSpeed(sf::Vector2f(0.0f, objs[index]->getSpeed().y));*/
                    vmove = 0.0f;
                    objs[index]->checked = objs[i];
                    objs[i]->onTouch();
                }
            }
        }
        objs[index]->move(hmove, vmove);
        for (int i = 0; i < objs.size(); i++) {
            // Swap objects if they passed behind/in front of each other
            if ((objs[index]->getPosition().y < objs[i]->getPosition().y
                        && objs[index]->drawDepth > objs[i]->drawDepth)
                    || (objs[index]->getPosition().y > objs[i]->getPosition().y
                        && objs[index]->drawDepth < objs[i]->drawDepth)) {
                unsigned int tmp = objs[index]->drawDepth;
                objs[index]->drawDepth = objs[i]->drawDepth;
                objs[i]->drawDepth = tmp;
            }
        }
    } else {
        objs[obj.sceneIndex]->stopMoving();
    }
}

void Scene::setVisible() {
    active = true;
    visible = true;
    loopTime.restart();
}

void Scene::setActive(bool active_) {
    if (!active_) {
        update(); // make sure stuff ends up where it is supposed to be, even if deactivated
    } else {
        loopTime.restart();
    }
    active = active_;
}

bool Scene::isActive() {
    return active;
}

void Scene::transfer(Scene *scene, sf::Vector2f destCoords_, int targetDir_) {
    visible = false;
    transferring = scene;
    destCoords = destCoords_;
    targetDir = targetDir_;
}

int Scene::getNumObjs() {
    return objs.size();
}

Thing* Scene::getObj(int index) {
    return objs[index];
}

void Scene::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if (visible || transferring) {
        // draw the tilemap in the background
        target.draw(tilemap);
        /* there is DEFINITELY a more efficient way to do this */
        /* TODO: figure this out. Maybe sort the list of thingies when
           they are added, and then exchange objects only when they swap depths?
           (i.e. be the gatekeeper for swapping depths in a scene) But then the
           drawing depths should probably be contained in this class, not in
           the objects themselves. Which could be done, with like:
           struct depthObj { uint depth; Thing* obj; }
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
        target.draw(fadeRect, states);
    }
}
