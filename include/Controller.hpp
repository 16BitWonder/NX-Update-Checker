#ifndef CONTROLLER_HPP
#define CONTROLLER_HPP
#include "common.hpp"

class Controller {
	private:
	PadState pad;
	u64 kHeld = 0;
	
	public:
	void Init();
	u64 getNewPressed();
};
#endif