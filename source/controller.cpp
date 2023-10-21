#include "controller.hpp"

namespace controller {
	u64 getPadDown(PadState pad) {
		padUpdate(&pad);
		return padGetButtonsDown(&pad);
	}
}