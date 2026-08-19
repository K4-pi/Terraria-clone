#include "../include/healthbar.h"
#include "../include/lmath.h"
#include "../include/id.h"

Healthbar::Healthbar(vector2f_t position, int max_health)
    : m_old_hp { 0 }
    , m_max_health { max_health }
    , m_position { position }
    , m_hearts {}
{
    vector2f_t current_position = m_position;

    for (int i=0; i < max_health / 10; i++)
    {
        m_hearts.push_back(GUI::GuiObject(
            current_position,
            {64.0f, 64.0f},
            HEART_LVL_1_ID
        ));

        current_position.x += 32.0f + 8.0f;
    }
}

void Healthbar::SetMaxHealth(int max_health)
{
    m_max_health = max_health;
}

void Healthbar::Draw(SDL_Renderer *renderer)
{
    for (GUI::GuiObject &obj : m_hearts)
    {
        obj.Draw(renderer);
    }
}

void Healthbar::Update(int hp)
{
    if (m_hearts.size() == 0 || hp == m_old_hp) return;

    m_old_hp = hp;
    int hp_val = hp;

    for (GUI::GuiObject &obj : m_hearts)
    {
        if (hp_val >= 10) obj.m_id = HEART_LVL_4_ID;
        else if (hp_val >= 5) obj.m_id = HEART_LVL_3_ID;
        else if (hp_val >= 1) obj.m_id = HEART_LVL_2_ID;
        else obj.m_id = HEART_LVL_1_ID;

        hp_val -= 10;
    }
}
