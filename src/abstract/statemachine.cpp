#include <iostream>
#include "../../debug.hpp"

#include "statemachine.hpp"

// public
StateMachine::StateMachine() :
    loaded(0)
    , current_view(-1)
{
    this->history.reserve(MAX_HISTORY);
}

bool StateMachine::load(sf::String playername, bool new_game, sf::RenderWindow& window, bool& has_requested_quit)
{
    int id = -1;
    bool b = false;
    switch(this->loaded)
    {
    case 0:
        id = this->defaultv.getId();
        b = this->defaultv.load(playername, new_game, window, has_requested_quit);
        DebugLog(SH_SPE, "default view loaded : " << b);
        break;

    case 1:
        id = this->creav.getId();
        b = this->creav.load();
        DebugLog(SH_SPE, "crea view loaded : " << b);
        break;

    case 2:
        id = this->dexv.getId();
        b = this->dexv.load();
        DebugLog(SH_SPE, "dex view loaded : " << b);
        break;

    case 3:
        id = this->savev.getId();
        b = this->savev.load();
        DebugLog(SH_SPE, "save view loaded : " << b);
        break;

    case 4:
        id = this->inventoryv.getId();
        b = this->inventoryv.load();
        DebugLog(SH_SPE, "inventory view loaded : " << b);
        break;

    case 5:
        id = this->mapv.getId();
        b = this->mapv.load();
        DebugLog(SH_SPE, "map view loaded : " << b);
        break;

    case 6:
        id = this->fightv.getId();
        b = this->fightv.load();
        DebugLog(SH_SPE, "fight view loaded : " << b);
        break;

    case 7:
        id = this->shopv.getId();
        b = this->shopv.load();
        DebugLog(SH_SPE, "shop view loaded : " << b);
        break;

    case 8:
        id = this->settingsv.getId();
        b = this->settingsv.load();
        DebugLog(SH_SPE, "settings views loaded : " << b);
        break;
    }
    DebugLog(SH_INFO, "Loading view " << id);
    this->loaded++;

    return this->loaded == 9;
}

int StateMachine::getId()
{
    return this->current_view;
}

bool StateMachine::hasActiveHud(int vid)
{
    bool ret_val = false;

    switch(vid)
    {
    case DEFAULT_VIEW_ID:
        ret_val = this->defaultv.hasActiveHud();
        break;

    case MYCREATURES_VIEW_ID:
        ret_val = this->creav.hasActiveHud();
        break;

    case DEX_VIEW_ID:
        ret_val = this->dexv.hasActiveHud();
        break;

    case SAVING_VIEW_ID:
        ret_val = this->savev.hasActiveHud();
        break;

    case INVENTORY_VIEW_ID:
        ret_val = this->inventoryv.hasActiveHud();
        break;

    case MAP_VIEW_ID:
        ret_val = this->mapv.hasActiveHud();
        break;

    case FIGHT_VIEW_ID:
        ret_val = this->fightv.hasActiveHud();
        break;

    case SHOP_VIEW_ID:
        ret_val = this->fightv.hasActiveHud();
        break;

    case SETTINGS_VIEW_ID:
        ret_val = this->settingsv.hasActiveHud();
        break;

    default:
        ret_val = -1;
        break;
    }

    return ret_val;
}

int StateMachine::change_view(int new_view)
{
    if (new_view == -1)
        return 0;

    int ret_val = -1;

    switch(new_view)
    {
    case LAST_VIEW_ID:
        ret_val = (!this->go_back_to_last_view()) ? 1 : -1;
        break;

    // all the views id here
    // we can add specific methods to each view just before displaying them
    case DEFAULT_VIEW_ID:
    case MYCREATURES_VIEW_ID:
    case DEX_VIEW_ID:
        this->dexv.load_dex_content();
    case SAVING_VIEW_ID:
    case INVENTORY_VIEW_ID:
    case SHOP_VIEW_ID:
    case MAP_VIEW_ID:
    case SETTINGS_VIEW_ID:
        this->history.push_back(this->current_view);
        this->current_view = new_view;
        DebugLog(SH_OK, "Changing view from id " << this->history[this->history.size() - 1] << " to id " << new_view);
        ret_val = 1;
        break;

    case FIGHT_VIEW_ID:
        // note that here, we don't log that we were in a fight to avoid going back with the use of LAST_VIEW_ID
        this->current_view = new_view;
        DebugLog(SH_SPE, "Changing view (fight)  " << new_view);
        this->fightv.start();
        ret_val = 1;
        break;

    default:
        ret_val = -1;
        break;
    }

    return ret_val;
}

int StateMachine::go_back_to_last_view()
{
    if (this->history.size() != 0)
    {
        int latest = pop<int>(&this->history, -1);
        if (latest != -1)
        {
            this->current_view = latest;
            DebugLog(SH_INFO, "Going back to view id " << latest);
            return 0;
        }
        else
        {
            return -1;  // can not pop
        }
    }
    return -1; // history is empty
}

int StateMachine::process_event_current(sf::Event& event, sf::Time elapsed, sf::RenderWindow& win)
{
    int ret_val = -1;

    switch(this->current_view)
    {
    case DEFAULT_VIEW_ID:
        ret_val = this->defaultv.process_event(event, elapsed, win);
        break;

    case MYCREATURES_VIEW_ID:
        ret_val = this->creav.process_event(event, elapsed);
        break;

    case DEX_VIEW_ID:
        ret_val = this->dexv.process_event(event, elapsed);
        break;

    case SAVING_VIEW_ID:
        ret_val = this->savev.process_event(event, elapsed);
        break;

    case INVENTORY_VIEW_ID:
        ret_val = this->inventoryv.process_event(event, elapsed);
        break;

    case MAP_VIEW_ID:
        ret_val = this->mapv.process_event(event, elapsed);
        break;

    case FIGHT_VIEW_ID:
        ret_val = this->fightv.process_event(event, elapsed);
        break;

    case SHOP_VIEW_ID:
        ret_val = this->shopv.process_event(event, elapsed);
        break;

    case SETTINGS_VIEW_ID:
        ret_val = this->settingsv.process_event(event, elapsed);
        break;

    default:
        ret_val = -1;
        break;
    }

    return ret_val;
}

void StateMachine::render_current(sf::RenderWindow& window)
{
    switch(this->current_view)
    {
    case DEFAULT_VIEW_ID:
        this->defaultv.render(window);
        break;

    case MYCREATURES_VIEW_ID:
        this->creav.render(window);
        break;

    case DEX_VIEW_ID:
        this->dexv.render(window);
        break;

    case SAVING_VIEW_ID:
        this->savev.render(window);
        break;

    case INVENTORY_VIEW_ID:
        this->inventoryv.render(window);
        break;

    case MAP_VIEW_ID:
        this->mapv.render(window);
        break;

    case FIGHT_VIEW_ID:
        this->fightv.render(window);
        break;

    case SHOP_VIEW_ID:
        this->shopv.render(window);
        break;

    case SETTINGS_VIEW_ID:
        this->settingsv.render(window);
        break;

    default:
        break;
    }
}

void StateMachine::update_current(sf::RenderWindow& window, sf::Time elapsed)
{
    // set sf::view as default (kind of resetting)
    window.setView(window.getDefaultView());

    switch(this->current_view)
    {
    case DEFAULT_VIEW_ID:
        this->defaultv.update(window, elapsed);
        break;

    case MYCREATURES_VIEW_ID:
        this->creav.update(window, elapsed);
        break;

    case DEX_VIEW_ID:
        this->dexv.update(window, elapsed);
        break;

    case SAVING_VIEW_ID:
        this->savev.update(window, elapsed);
        break;

    case INVENTORY_VIEW_ID:
        this->inventoryv.update(window, elapsed);
        break;

    case MAP_VIEW_ID:
        this->mapv.update(window, elapsed);
        break;

    case FIGHT_VIEW_ID:
        this->fightv.update(window, elapsed);
        break;

    case SHOP_VIEW_ID:
        this->shopv.update(window, elapsed);
        break;

    case SETTINGS_VIEW_ID:
        this->settingsv.update(window, elapsed);

    default:
        break;
    }

    if (OMessenger::get().target_view != UNREACHABLE_VIEW_ID)
    {
        this->change_view(OMessenger::get().target_view);
    }
}

DefaultView* StateMachine::getDefault()
{
    return &this->defaultv;
}

CreaView* StateMachine::getCrea()
{
    return &this->creav;
}

DexView* StateMachine::getDex()
{
    return &this->dexv;
}

SaveView* StateMachine::getSave()
{
    return &this->savev;
}

InventView* StateMachine::getInventory()
{
    return &this->inventoryv;
}

MapView* StateMachine::getMap()
{
    return &this->mapv;
}

FightView* StateMachine::getFight()
{
    return &this->fightv;
}

ShopView* StateMachine::getShop()
{
    return &this->shopv;
}

SettingsView* StateMachine::getSettings()
{
    return &this->settingsv;
}
