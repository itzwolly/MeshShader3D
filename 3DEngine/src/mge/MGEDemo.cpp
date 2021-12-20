#include <iostream>
#include <string>

#include "glm.hpp"

#include "mge/core/Renderer.hpp"

#include "mge/core/Mesh.hpp"
#include "mge/core/World.hpp"
#include "mge/core/Texture.hpp"
#include "mge/core/Light.hpp"
#include "mge/core/Camera.hpp"
#include "mge/core/GameObject.hpp"

#include "mge/materials/AbstractMaterial.hpp"
#include "mge/materials/MeshShaderMaterial.hpp"

#include "mge/behaviours/CameraBehaviour.hpp"

#include "mge/util/DebugHud.hpp"

#include "mge/config.hpp"
#include "mge/MGEDemo.hpp"

//construct the game class into _window, _renderer and hud (other parts are initialized by build)
MGEDemo::MGEDemo(): AbstractGame () , _hud(0) { }

void MGEDemo::initialize() {
    //setup the core part
    AbstractGame::initialize();

    //setup the custom part so we can display some text
	std::cout << "Initializing HUD" << std::endl;
	_hud = new DebugHud(_window);
	std::cout << "HUD initialized." << std::endl << std::endl;
}

Camera* camera;

//build the game _world
void MGEDemo::_initializeScene() {
    //MATERIALS
	AbstractMaterial* meshShaderMaterial = new MeshShaderMaterial();

    //SCENE SETUP

    //add camera first (it will be updated last)
    camera = new Camera ("Camera", glm::vec3(30, -47.5f, 1));
    // camera = new Camera("Camera", glm::vec3(-120, 20, 42));
	
    camera->setBehaviour(new CameraBehaviour(40));
    camera->rotate(glm::radians(90.0f), glm::vec3(1, 0, 0));
    _world->add(camera);
    _world->setMainCamera(camera);

	GameObject* meshShaderContainer = new GameObject("Mesh Shader Container", glm::vec3(-339617, -5790966, -104 + -3));
    // GameObject* meshShaderContainer = new GameObject("Mesh Shader Container", glm::vec3(0, 0, -3));
	meshShaderContainer->setMaterial(meshShaderMaterial);
	_world->add(meshShaderContainer);
}

void MGEDemo::_render() {
    AbstractGame::_render();
    _updateHud();
}

void MGEDemo::_updateHud() {
    std::string debugInfo = "";
    debugInfo += std::string ("FPS:") + std::to_string((int)_fps) + "\n\n";
    // debugInfo += _world->GetChildrenNames(_world, 0) + "\n";
    // debugInfo += std::string("Camera Transform: \n - ") + std::to_string(camera->getWorldPosition().x) + ", " + std::to_string(camera->getWorldPosition().y) + ", " + std::to_string(camera->getWorldPosition().z) + "\n";
	
    _hud->setDebugInfo(debugInfo);
    _hud->draw();
}

MGEDemo::~MGEDemo() {
	//dtor
}
