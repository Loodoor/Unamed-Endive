#ifndef DEF_INVENT_VIEW
#define DEF_INVENT_VIEW

#include <SFML/Graphics.hpp>
#include <SFML/System/String.hpp>
#include <string>
#include <map>

#include "view.hpp"
#include "../abstract/container.hpp"
#include "../bag/bag.hpp"
#include "../constants.hpp"
#ifdef PLATFORM_WIN
    #include <windows.h>
#endif // PLATFORM_WIN
#include "../bag/objectstable.hpp"
#include "../abstract/functions.hpp"
#include "../objects/messenger.hpp"

class InventView : public View
{
private:
    // variables
    int current;
    int selected;
    int offset;
    Container<sf::Texture> textures;
    std::map<std::string, sf::Sprite> sprites;
    sf::Font font;
    sf::Text current_pocket_name;
    sf::Text object_name;
    sf::Text object_desc;
    sf::Text errmsg;
    int errdisplay;
    Bag* bag;

    const std::string BCKG = "backgroud";
    const std::string LARROW = "left_arrow";
    const std::string RARROW = "right_arrow";
    const std::string POCKET1 = "pocket1";
    const std::string POCKET2 = "pocket2";
    const std::string POCKET3 = "pocket3";
    const std::string POCKET4 = "pocket4";
    const std::string POCKET5 = "pocket5";
    const std::string VID = "btn_vider";
    const std::string USE = "btn_use";
    const std::string DROP = "btn_drop";

    // methods
    void draw_content(sf::RenderWindow&);
    void change_pocket();

public:
    // methods
    InventView();
    ~InventView();
    bool load() override;
    void render(sf::RenderWindow&) override;
    int process_event(sf::Event&, sf::Time) override;
    void update(sf::RenderWindow&, sf::Time) override;
    void add_bag(Bag*);
};

#endif // DEF_INVENT_VIEW




