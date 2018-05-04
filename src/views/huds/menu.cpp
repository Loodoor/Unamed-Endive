#include <SFML/Graphics.hpp>
#include <iostream>
#include "../../../debug.hpp"

#include "menu.hpp"
#include "../../constants.hpp"
#ifdef PLATFORM_WIN
    #include <windows.h>
#endif // PLATFORM_WIN
#include "../../abstract/defines.hpp"
#include "../settings.hpp"
#include "../../abstract/config.hpp"

// public
MenuHUD::MenuHUD() :
    Hud(MENU_VIEW_ID)
    , current(-1)
{
    if (!this->font.loadFromFile(FONTPATH))
        DebugLog(SH_ERR, "Can not load font at " << FONTPATH);
    DebugLog(SH_OK, "Menu HUD loaded");
}

bool MenuHUD::load()
{
    // loading textures
    sf::Texture cat;
    if (!cat.loadFromFile("assets/gui/menu/fd_case_creature.png"))
        return false;
    this->textures.add(this->BG_CAT_CREA, cat);

    sf::Texture cat2;
    if (!cat2.loadFromFile("assets/gui/menu/fd_case_creature_selected.png"))
        return false;
    this->textures.add(this->BG_CAT_SEL_CREA, cat2);

    sf::Texture cat3;
    if (!cat3.loadFromFile("assets/gui/menu/fd_case_indexer.png"))
        return false;
    this->textures.add(this->BG_CAT_DEX, cat3);

    sf::Texture cat4;
    if (!cat4.loadFromFile("assets/gui/menu/fd_case_indexer_selected.png"))
        return false;
    this->textures.add(this->BG_CAT_SEL_DEX, cat4);

    sf::Texture cat5;
    if (!cat5.loadFromFile("assets/gui/menu/fd_case_inventaire.png"))
        return false;
    this->textures.add(this->BG_CAT_INVENTORY, cat5);

    sf::Texture cat6;
    if (!cat6.loadFromFile("assets/gui/menu/fd_case_inventaire_selected.png"))
        return false;
    this->textures.add(this->BG_CAT_SEL_INVENTORY, cat6);

    sf::Texture cat7;
    if (!cat7.loadFromFile("assets/gui/menu/fd_case_map.png"))
        return false;
    this->textures.add(this->BG_CAT_MAP, cat7);

    sf::Texture cat8;
    if (!cat8.loadFromFile("assets/gui/menu/fd_case_map_selected.png"))
        return false;
    this->textures.add(this->BG_CAT_SEL_MAP, cat8);

    sf::Texture cat9;
    if (!cat9.loadFromFile("assets/gui/menu/fd_case_retour.png"))
        return false;
    this->textures.add(this->BG_CAT_BACK, cat9);

    sf::Texture cat10;
    if (!cat10.loadFromFile("assets/gui/menu/fd_case_retour_selected.png"))
        return false;
    this->textures.add(this->BG_CAT_SEL_BACK, cat10);

    sf::Texture cat11;
    if (!cat11.loadFromFile("assets/gui/menu/fd_case_sauvegarder.png"))
        return false;
    this->textures.add(this->BG_CAT_SAVE, cat11);

    sf::Texture cat12;
    if (!cat12.loadFromFile("assets/gui/menu/fd_case_sauvegarder_selected.png"))
        return false;
    this->textures.add(this->BG_CAT_SEL_SAVE, cat12);

    sf::Texture param;
    if (!param.loadFromFile("assets/gui/menu/fd_parametres.png"))
        return false;
    this->textures.add(this->BG_PARAM, param);

    // creating sprites
    this->sprites[this->BG_CAT_CREA] = sf::Sprite(this->textures.get(this->BG_CAT_CREA));
    this->sprites[this->BG_CAT_SEL_CREA] = sf::Sprite(this->textures.get(this->BG_CAT_SEL_CREA));
    this->sprites[this->BG_CAT_DEX] = sf::Sprite(this->textures.get(this->BG_CAT_DEX));
    this->sprites[this->BG_CAT_SEL_DEX] = sf::Sprite(this->textures.get(this->BG_CAT_SEL_DEX));
    this->sprites[this->BG_CAT_INVENTORY] = sf::Sprite(this->textures.get(this->BG_CAT_INVENTORY));
    this->sprites[this->BG_CAT_SEL_INVENTORY] = sf::Sprite(this->textures.get(this->BG_CAT_SEL_INVENTORY));
    this->sprites[this->BG_CAT_MAP] = sf::Sprite(this->textures.get(this->BG_CAT_MAP));
    this->sprites[this->BG_CAT_SEL_MAP] = sf::Sprite(this->textures.get(this->BG_CAT_SEL_MAP));
    this->sprites[this->BG_CAT_BACK] = sf::Sprite(this->textures.get(this->BG_CAT_BACK));
    this->sprites[this->BG_CAT_SEL_BACK] = sf::Sprite(this->textures.get(this->BG_CAT_SEL_BACK));
    this->sprites[this->BG_CAT_SAVE] = sf::Sprite(this->textures.get(this->BG_CAT_SAVE));
    this->sprites[this->BG_CAT_SEL_SAVE] = sf::Sprite(this->textures.get(this->BG_CAT_SEL_SAVE));
    this->sprites[this->BG_PARAM] = sf::Sprite(this->textures.get(this->BG_PARAM));

    // setting pos
    float y = 11.0f + WIN_H / 2 - 220.0f;
    this->sprites[this->BG_CAT_CREA].setPosition(30.0f, y);
    this->sprites[this->BG_CAT_SEL_CREA].setPosition(30.0f, y);
    this->sprites[this->BG_CAT_INVENTORY].setPosition(325.0f, y);
    this->sprites[this->BG_CAT_SEL_INVENTORY].setPosition(325.0f, y);
    y += 143.0f;
    this->sprites[this->BG_CAT_MAP].setPosition(30.0f, y);
    this->sprites[this->BG_CAT_SEL_MAP].setPosition(30.0f, y);
    this->sprites[this->BG_CAT_SAVE].setPosition(325.0f, y);
    this->sprites[this->BG_CAT_SEL_SAVE].setPosition(325.0f, y);
    y += 143.0f;
    this->sprites[this->BG_CAT_DEX].setPosition(30.0f, y);
    this->sprites[this->BG_CAT_SEL_DEX].setPosition(30.0f, y);
    this->sprites[this->BG_CAT_BACK].setPosition(325.0f, y);
    this->sprites[this->BG_CAT_SEL_BACK].setPosition(325.0f, y);
    //
    this->sprites[this->BG_PARAM].setPosition(WIN_W - this->sprites[this->BG_PARAM].getGlobalBounds().width - 5.0f, 5.0f);

    // creating texts
    sf::Text crea;
    setupFont(crea, this->font, sf::Color::Black, 24)
    crea.setString("Cr�atures");
    this->texts[this->TXT_CREA] = crea;

    sf::Text invent;
    setupFont(invent, this->font, sf::Color::Black, 24)
    invent.setString("Inventaire");
    this->texts[this->TXT_INVENT] = invent;

    sf::Text carte;
    setupFont(carte, this->font, sf::Color::Black, 24)
    carte.setString("Carte");
    this->texts[this->TXT_MAP] = carte;

    sf::Text save;
    setupFont(save, this->font, sf::Color::Black, 24)
    save.setString("Sauvegarder");
    this->texts[this->TXT_SAVE] = save;

    sf::Text dex;
    setupFont(dex, this->font, sf::Color::Black, 24)
    dex.setString("Indexeur");
    this->texts[this->TXT_DEX] = dex;

    sf::Text goback;
    setupFont(goback, this->font, sf::Color::Black, 24)
    goback.setString("Retour");
    this->texts[this->TXT_BACK] = goback;

    // setting pos (texts)
    this->texts[this->TXT_CREA].setPosition(54.0f, 26.0f + WIN_H / 2 - 220.0f);
    this->texts[this->TXT_INVENT].setPosition(349.0f, 26.0f + WIN_H / 2 - 220.0f);
    this->texts[this->TXT_MAP].setPosition(54.0f, 168.0f + WIN_H / 2 - 220.0f);
    this->texts[this->TXT_SAVE].setPosition(349.0f, 168.0f + WIN_H / 2 - 220.0f);
    this->texts[this->TXT_DEX].setPosition(54.0f, 310.0f + WIN_H / 2 - 220.0f);
    this->texts[this->TXT_BACK].setPosition(349.0f, 310.0f + WIN_H / 2 - 220.0f);

    return true;
}

void MenuHUD::render(sf::RenderTexture& window)
{
    if (!this->isTriggered())
        goto dont;

    window.draw(this->sprites[this->BG_CAT_CREA]);
    window.draw(this->sprites[this->BG_CAT_DEX]);
    window.draw(this->sprites[this->BG_CAT_INVENTORY]);
    window.draw(this->sprites[this->BG_CAT_MAP]);
    window.draw(this->sprites[this->BG_CAT_BACK]);
    window.draw(this->sprites[this->BG_CAT_SAVE]);
    window.draw(this->sprites[this->BG_PARAM]);

    // drawing cases
    switch (this->current)
    {
    case 0:
        window.draw(this->sprites[this->BG_CAT_SEL_CREA]);
        break;

    case 1:
        window.draw(this->sprites[this->BG_CAT_SEL_INVENTORY]);
        break;

    case 2:
        window.draw(this->sprites[this->BG_CAT_SEL_MAP]);
        break;

    case 3:
        window.draw(this->sprites[this->BG_CAT_SEL_SAVE]);
        break;

    case 4:
        window.draw(this->sprites[this->BG_CAT_SEL_DEX]);
        break;

    case 5:
        window.draw(this->sprites[this->BG_CAT_SEL_BACK]);
        break;
    }

    window.draw(this->texts[this->TXT_CREA]);
    window.draw(this->texts[this->TXT_INVENT]);
    window.draw(this->texts[this->TXT_MAP]);
    window.draw(this->texts[this->TXT_SAVE]);
    window.draw(this->texts[this->TXT_DEX]);
    window.draw(this->texts[this->TXT_BACK]);

    dont:;
}

void MenuHUD::update(sf::RenderWindow& window, sf::Time elapsed)
{
    if (this->isTriggered())
    {
        sf::Vector2i mouse = sf::Mouse::getPosition(window);
        this->clic(mouse.x, mouse.y);
    }
}

int MenuHUD::process_event(sf::Event& event, sf::Time elapsed)
{
    int new_view = -1;
    std::string k;

    if (!this->isTriggered())
        goto dont3;

    switch(event.type)
    {
    case sf::Event::TextEntered:
        k = SettingsView::convert_textentered_to_value(event.text.unicode);
        if (Config::get("menu") == k)
            new_view = 5;
        break;

    case sf::Event::KeyPressed:
        switch(event.key.code)
        {
        case sf::Keyboard::Return:
            if (this->current != -1)
                new_view = this->current;
            break;

        case sf::Keyboard::Up:
            this->current -= 2;
            if (this->current == -2)
                this->current = 4;
            else if (this->current == -1)
                this->current = 5;
            break;

        case sf::Keyboard::Down:
            this->current += 2;
            if (this->current == 6)
                this->current = 0;
            else if (this->current == 7)
                this->current = 1;
            break;

        case sf::Keyboard::Left:
            this->current -= 1;
            if (this->current == -1)
                this->current = 5;
            break;

        case sf::Keyboard::Right:
            this->current += 1;
            if (this->current == 6)
                this->current = 0;
            break;

        default:
            break;
        }
        break;

    case sf::Event::MouseButtonPressed:
        switch(event.mouseButton.button)
        {
        case sf::Mouse::Button::Left:
            this->clic(m__X, m__Y);
            if (this->current != -1)
                new_view = this->current;
            break;

        default:
            break;
        }
        break;

    default:
        break;
    }

    dont3:
    if (new_view != -1)
    {
        this->current = -1;
    }
    // return the new view id, if we need to change it
    if (new_view == 5)
    {
        // back button
        this->setTrigger(false);
        return -1;
    }
    return this->id_to_vid(new_view);
}

// private
void MenuHUD::clic(int x, int y)
{
    bool left = (x >= 30) && (x <= 320);
    int nb = (y - 11 - (WIN_H / 2 - 220.0f)) / 143;

    switch (nb)
    {
    case 0:
        if (!left) this->current = 1;
        else this->current = 0;
        break;

    case 1:
        if (!left) this->current = 3;
        else this->current = 2;
        break;

    case 2:
        if (!left) this->current = 5;
        else this->current = 4;
        break;
    }

    if (WIN_W - this->sprites[this->BG_PARAM].getGlobalBounds().width - 5 <= x && x <= WIN_W &&
            5 <= y && y <= this->sprites[this->BG_PARAM].getGlobalBounds().height)
        this->current = 6;
}

int MenuHUD::id_to_vid(int value)
{
    int ret;
    switch (value)
    {
    case -1:
        ret = -1;
        break;

    case 0:
        ret = MYCREATURES_VIEW_ID;
        break;

    case 1:
        ret = INVENTORY_VIEW_ID;
        break;

    case 2:
        ret = MAP_VIEW_ID;
        break;

    case 3:
        ret = SAVING_VIEW_ID;
        break;

    case 4:
        ret = DEX_VIEW_ID;
        break;

    case 5:
        ret = DEFAULT_VIEW_ID;
        break;

    case 6:
        ret = SETTINGS_VIEW_ID;
        break;

    default:
        ret = -1;  // should not happen
        break;
    }

    return ret;
}
