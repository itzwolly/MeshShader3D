#ifndef CAMERABEHAVIOUR_HPP
#define CAMERABEHAVIOUR_HPP

#include "mge/behaviours/AbstractBehaviour.hpp"

class CameraBehaviour : public AbstractBehaviour {
	public:
		//move speed is in units per second, turnspeed in degrees per second
		CameraBehaviour(float pMoveSpeed = 5, float pTurnSpeed = 45);
		virtual ~CameraBehaviour();
		virtual void update(float pStep);

	private:
		float _moveSpeed;
		float _turnSpeed;
};

#endif // CAMERABEHAVIOUR_HPP