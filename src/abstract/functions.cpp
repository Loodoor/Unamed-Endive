#include "functions.hpp"
#include "type.hpp"

bool is_file_existing(const std::string& file)
{
    std::ifstream reader(file.c_str());
    return !reader.fail();
}

// from https://github.com/LiquidHelium
sf::String wrapText(sf::String str, int width, const sf::Font& font, int characterSize, bool bold)
{
    int currentOffset = 0;
    bool firstWord = true;
    std::size_t wordBegining = 0;

    for (std::size_t pos(0); pos < str.getSize(); ++pos)
    {
        auto currentChar = str[pos];
        if (currentChar == '\n')
        {
            currentOffset = 0;
            firstWord = true;
            continue;
        }
        else if (currentChar == ' ')
        {
            wordBegining = pos;
            firstWord = false;
        }

        auto glyph = font.getGlyph(currentChar, characterSize, bold);
        currentOffset += glyph.advance;

        if (!firstWord && currentOffset > width)
        {
            pos = wordBegining;
            str[pos] = '\n';
            str.insert(pos, "\n");
            firstWord = true;
            currentOffset = 0;
        }
    }

    return str;
}

std::string convert_type_to_str(int t)
{
    std::string s = "???";
    switch (t)
    {
    case Type::NORMAL:
        s = "Normal";
        break;

    case Type::FIRE:
        s = "Feu";
        break;

    case Type::WATER:
        s = "Eau";
        break;

    case Type::GRASS:
        s = "Plante";
        break;

    case Type::FLYING:
        s = "Vol";
        break;

    case Type::FIGHTING:
        s = "Combat";
        break;

    case Type::POISON:
        s = "Poison";
        break;

    case Type::ELECTRIC:
        s = "Electrique";
        break;

    default:
        s = "???";
        break;
    }
    return s;
}

void fix_utf8_string(std::string& str)
{
    std::string temp;
    utf8::replace_invalid(str.begin(), str.end(), back_inserter(temp));
    str = temp;
}
