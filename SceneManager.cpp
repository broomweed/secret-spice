#include <iostream>
#include <cstdio>
#include "SceneManager.hpp"
#include "Scene.hpp"

SceneManager::SceneManager(Scene* scene) {
    currentScene = NULL;
    setScene(scene);
}

void SceneManager::setScene(Scene* scene) {
    if (currentScene) {
        currentScene->parent = NULL;
    }
    currentScene = scene;
    currentScene->parent = this;
    scene->setActive();
}

void SceneManager::update() {
    currentScene->update();
}
