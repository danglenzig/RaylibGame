#pragma once
#include "raylib.h"
#include "raymath.h"
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
class MiscTools
{
public:
	MiscTools();
	~MiscTools();
	static Vector2 AdjustVector2ByAngle(const Vector2 v, const float& angle) {
		float cosA = cosf(angle);
		float sinA = sinf(angle);

		float newX = v.x * cosA - v.y * sinA;
		float newY = v.x * sinA + v.y * cosA;
		return Vector2Normalize({ newX, newY });
	}

	static std::string GetUUID() {
        std::stringstream ss;
        ss << std::hex << std::setfill('0');

        for (int i = 0; i < 16; i++) {
            // Get a random byte (0-255)
            unsigned char byte = (unsigned char)GetRandomValue(0, 255);

            // Apply UUID v4 variants
            if (i == 6) {
                // High nibble must be 4
                byte = 0x40 | (byte & 0x0F);
            }
            else if (i == 8) {
                // High nibble must be 8, 9, A, or B
                byte = 0x80 | (byte & 0x3F);
            }

            ss << std::setw(2) << (int)byte;

            // Insert hyphens at specific positions
            if (i == 3 || i == 5 || i == 7 || i == 9) {
                ss << "-";
            }
        }

        return ss.str();
    }

private:

};

MiscTools::MiscTools()
{
}

MiscTools::~MiscTools()
{
}