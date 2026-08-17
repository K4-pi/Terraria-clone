#pragma once

#include "gui.h"
#include "lmath.h"

#include <vector>

class Healthbar
{
    private:
        int m_old_hp;
        int m_max_health;
        vector2f_t m_position;
        std::vector<GUI::GuiObject> m_hearts;

    public:
        Healthbar(vector2f_t position, int max_health);
        void Draw(SDL_Renderer *renderer);
        void SetMaxHealth(int max_health);
        void Update(int hp);
};
