#include "mge/behaviours/CameraBehaviour.hpp"
#include "mge/core/GameObject.hpp"
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>


CameraBehaviour::CameraBehaviour(float pMoveSpeed, float pTurnSpeed) : AbstractBehaviour(), _moveSpeed(pMoveSpeed), _turnSpeed(pTurnSpeed) { }

CameraBehaviour::~CameraBehaviour() { }

void CameraBehaviour::update(float pStep) {
	float vertical = 0.0f;
	float horizontal = 0.0f;
	bool shouldRotate = false;

	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Right)) {
		shouldRotate = true;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		vertical = -_moveSpeed;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		vertical = _moveSpeed;
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		horizontal = -_moveSpeed;
	} else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		horizontal = _moveSpeed;
	}

	if (shouldRotate) {
		if (vertical != 0 || horizontal != 0) {
			_owner->rotate(glm::radians(_turnSpeed * pStep), glm::vec3(-vertical, -horizontal, 0.0f));
		}
	} else {
		_owner->translate(glm::vec3(horizontal * pStep, 0.0f, vertical * pStep));
	}
}