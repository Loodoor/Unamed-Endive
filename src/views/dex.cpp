#include <iostream>

#include "dex.hpp"
#include "../constants.hpp"
#ifdef PLATFORM_WIN
    #include <windows.h>
#endif // PLATFORM_WIN
#include "../abstract/defines.hpp"

// public
DexView::DexView() :
    View(DEX_VIEW_ID)
    , selected(0)
    , loaded(false)
{

}

bool DexView::load()
{
    // loading textures
    sf::Texture background;
    if (!background.loadFromFile("assets/gui/fd_indexer.png"))
        return false;
    this->textures.add("background", background);

    // setting sprites
    this->sprites[this->BCKG] = sf::Sprite(this->textures.get("background"));

    // setting pos
    this->sprites[this->BCKG].setPosition(20.0f, 20.0f);

    // texts
    if (!this->font.loadFromFile(FONTPATH))
        return false;
    setupFont(this->text, this->font, sf::Color::Black, 24)
    this->text.setString("Dexeur");
    this->text.setPosition(WIN_W / 2 - this->text.getGlobalBounds().width / 2, 30.0f);

    return true;
}

void DexView::render(sf::RenderWindow& window)
{
    window.draw(this->sprites[this->BCKG]);
    window.draw(this->text);
    this->draw_content(window);
}

int DexView::process_event(sf::Event& event, sf::Time elapsed)
{
    int new_view = -1;

    switch(event.type)
    {
    case sf::Event::KeyPressed:
        switch(event.key.code)
        {
        case sf::Keyboard::Escape:
            new_view = LAST_VIEW_ID;
            break;

        default:
            break;
        }
        break;

    case sf::Event::MouseButtonPressed:
        switch(event.mouseButton.button)
        {
        case sf::Mouse::Button::Left:
            // selecting a creature
            if (m__X >= 30 && m__X <= WIN_W - 30 && m__Y >= this->text.getPosition().y + 84.0f && m__Y <= WIN_H - 30)
            {
                int ry = (m__Y - (this->text.getPosition().y + 84.0f)) / 64;
                if (ry >= 0 && ry < 8)
                {
                    // ok the selected creature is in range
                    this->selected += ry;
                    this->selected %= this->dex_content.size();
                }
            }
            break;

        default:
            break;
        }
        break;

    case sf::Event::MouseWheelScrolled:
        this->selected -= event.mouseWheelScroll.delta;
        if (this->selected < -1)
            this->selected = -1;
        if (this->selected > this->dex_content.size())
            this->selected = 0;
        break;

    default:
        break;
    }

    return new_view;
}

void DexView::update(sf::RenderWindow& window, sf::Time elapsed)
{
}

int DexView::index_of(const std::string& name)
{
    int i = 0;

    for (int i=0; i < this->dex_content.size(); i++)
    {
        if (this->dex->getInfo(std::get<2>(this->dex_content[i])).evolution == name)
            return i;
    }

    return i;
}

void DexView::draw_content(sf::RenderWindow& window)
{
    for (int i=this->selected; i < this->selected + 9; i++)
    {
        std::get<0>(this->dex_content[i % this->dex_content.size()]).setPosition(30.0f, this->text.getPosition().y + 84.0f + (i - this->selected) * 64.0f);
        if (i == this->selected)
        {
            if (this->dex->getInfo(std::get<2>(this->dex_content[i % this->dex_content.size()])).viewed)
            {
                // let's display some info about the creature because we've already seen it in the past
                window.draw(std::get<1>(this->dex_content[i % this->dex_content.size()]));
            }
            // just turn the thing green to see what is selected
            std::get<0>(this->dex_content[i % this->dex_content.size()]).setFillColor(sf::Color::Green);
        }
        window.draw(std::get<0>(this->dex_content[i % this->dex_content.size()]));
        std::get<0>(this->dex_content[i % this->dex_content.size()]).setFillColor(sf::Color::Black);
    }
}

void DexView::add_dex(Dex* dex)
{
    this->dex = dex;
}

void DexView::add_crealoader(CreaturesLoader* creaload)
{
    this->crealoader = creaload;
    this->loaded = true;
    // load the content of the dex by creating all the sf::Text
    this->load_dex_content();
}

void DexView::load_dex_content()
{
    if (!this->loaded)
        goto hell_again;

    {
        // we must ensure it is empty each time we use this method
        this->dex_content.clear();

        struct DexInfo crea;

        // variables for formating
        std::string stext;
        std::string stype;
        std::string vu;
        std::string capture;
        std::string evolution;

        // magic variables
        int id = 0;
        std::string current;

        current = this->dex->getNext();
        while (current != "")
        {
            std::tuple<sf::Text, sf::Sprite, std::string> content;

            crea = this->dex->getInfo(current);

            sf::Text _text;
            setupFont(_text, this->font, sf::Color::Black, 20)

            sf::Sprite _sprite;
            _sprite.setTexture(this->crealoader->get(crea.file));
            float factor = 180.0f / this->crealoader->get(crea.file).getSize().y;
            _sprite.setScale(factor, factor);

            stype = convert_type_to_str(crea.type);
            vu = (crea.viewed) ? "oui" : "non";
            capture = (crea.captured) ? "oui" : "non";
            evolution = (crea.evolution != "") ? crea.evolution : "Aucune";

            stext = current
                + ", " + stype
                + "\nNiveau " + to_string<int>(crea.stade)
                + ", Evolution : " + evolution
                + "\nVu : " + vu
                + ", Captur� : " + capture;

            _text.setString(stext);

            _sprite.setPosition(WIN_W - 30.0f - _sprite.getGlobalBounds().width, this->text.getPosition().y + 104.0f);

            std::get<0>(content) = _text;
            std::get<1>(content) = _sprite;
            std::get<2>(content) = current;

            this->dex_content.push_back(content);

            current = this->dex->getNext();
            id++;
        }
    }

    hell_again:;
}

DexView::~DexView()
{
    this->dex = NULL;
    this->crealoader = NULL;
}
