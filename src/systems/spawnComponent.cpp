#include <cstdlib>
#include <systems/movementComponent.hpp>

using namespace movcomp;

namespace spacomp {

	movcomp::Vec2 randomizePosition(const movcomp::Transform& windowSize)
	{
		movcomp::Vec2 newPosition;

		movcomp::Vec2 maxBound = windowSize.get_max_bound();
		movcomp::Vec2 minBound = windowSize.get_min_bound();


		// HAUT GAUCHE

		// HAUT DROIT

		// BAS GAUCHE

		// BAS DROIT


		int zone = rand() % 4; // 0 1 2 3

		switch (zone)
		{
		case 0:
			newPosition = {};

			break;
		case 1:
			newPosition = {};

			break;
		case 2:
			newPosition = {};

			break;
		case 3:
			newPosition = {};

			break;
		}



		


		// Quelque chose qui ne dépasse pas les valeurs des bounds



		return newPosition;
	}

}