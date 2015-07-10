#include <iostream>
#include <cstdio>
#include "Scene.hpp"
#include "SceneManager.hpp"

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
    scene->setVisible();
}

void SceneManager::update() {
    currentScene->update();
}
