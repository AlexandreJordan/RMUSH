#ifndef TOOLS_HPP
#define TOOLS_HPP

#include <math.h>
#include <vector>

using namespace std;

class Tools
{
public:

    static void swapInt(int& a, int& b) {
        int tmp = a;
        a = b;
        b = tmp;
    }

    static float getDistance(const int& x1, const int& y1, const int& x2, const int& y2) {
        int deltaX = x1 - x2;
        int deltaY = y1 - y2;

        return sqrtf( ((deltaX * deltaX) * 1.0f) + ((deltaY * deltaY) * 1.0f) );
    }

    static float getDamage( const int& pattStrength, const int& pattDexterity, const int& pattWeaponMinDamage, const int& pattWeaponMaxDamage,
                            const int& pdefDexterity, const int& pdefDefense) {
        float randFloat = TCODRandom::getInstance()->getFloat(0.0f, 1.0f);

        float damageMultiplier = 0.0f;

        if (randFloat > 0.0f && randFloat < 0.5f)
            damageMultiplier = 0.0f;
        else
            if (randFloat >= 0.5f && randFloat < 1.0f)
                damageMultiplier = 1.0f;
            else
                if (randFloat >= 1.0f && randFloat < 2.0f)
                    damageMultiplier = 2.0f;

        int baseDamage = TCODRandom::getInstance()->getInt(pattWeaponMinDamage, pattWeaponMaxDamage) + pattStrength - pdefDefense;

        return baseDamage * damageMultiplier;
    }

    static std::vector<std::string> split(const std::string& text, const char& sep) {
        string newOcc("");
        vector<string> result;

        for (char c: text) {
            if (c == sep) {
                result.push_back(newOcc);
                newOcc = "";
            }
            else {
                newOcc.push_back(c);
            }
        }

        return result;
    }
};

#endif