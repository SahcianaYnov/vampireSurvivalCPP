#include <base.hpp>
#include <cstdlib>

namespace spacomp {

    Vector2 randomizePosition(const Transform& windowSize, Vector2& directionNormalized)
    {
        Vector2 newPosition{};
        Vector2 maxBound = windowSize.get_max_bound();

        // Zone al�atoire (0 = haut, 1 = bas, 2 = gauche, 3 = droite)
        int zone = rand() % 4;

        switch (zone)
        {
        case 0: // Haut
            newPosition = { static_cast<float>(rand() % static_cast<int>(maxBound.x)), 0.f };  // Spawn al�atoire sur le bord haut
            directionNormalized = { 0.f, 1.f }; // Direction vers le bas
            break;

        case 1: // Bas
            newPosition = { static_cast<float>(rand() % static_cast<int>(maxBound.x)), maxBound.y };  // Spawn al�atoire sur le bord bas
            directionNormalized = { 0.f, -1.f }; // Direction vers le haut
            break;

        case 2: // Gauche
            newPosition = { 0.f, static_cast<float>(rand() % static_cast<int>(maxBound.y)) };  // Spawn al�atoire sur le bord gauche
            directionNormalized = { 1.f, 0.f }; // Direction vers la droite
            break;

        case 3: // Droite
            newPosition = { maxBound.x, static_cast<float>(rand() % static_cast<int>(maxBound.y)) };  // Spawn al�atoire sur le bord droit
            directionNormalized = { -1.f, 0.f }; // Direction vers la gauche
            break;
        }

        return newPosition;
    }

}