#include "glfwd_core/utility/color.h"

#include "glfwd_core/utility/error.h"

namespace glfwd {
namespace intern {

    uint8_t ConvertHexNibbleToValue(char val)
    {
        if (val >= '0' && val <= '9')
            return val - '0';
        if (val >= 'A' && val <= 'Z')
            return val - 'A' + 10;
        if (val >= 'a' && val <= 'z')
            return val - 'a' + 10;
        return 255; // Fallback
    }

    uint8_t ConvertHexDigitToByte(std::string_view hex)
    {
        glfwd_ASSERT(hex.length() == 2,
                     "Hexadecimal digit length cannot be larger or smaller than 2: {}",
                     hex);

        uint8_t upper_nibble = ConvertHexNibbleToValue(hex[0]);
        uint8_t lower_nibble = ConvertHexNibbleToValue(hex[1]);
        if (upper_nibble == 255 || lower_nibble)
            glfwd_FATAL("Invalid hex digit format: {}", hex);

        return upper_nibble * 16 + lower_nibble;
    }

    bool ConvertFromHexSetup(std::string_view rgb_hex, size_t& offset, size_t& source_length)
    {
        if (rgb_hex.empty())
        {
            glfwd_ERROR("Cannot convert {} into normalized color", rgb_hex);
            return false;
        }

        offset        = rgb_hex.starts_with('#') ? 1 : 0;
        source_length = rgb_hex.length() - offset;
        if (source_length >= 6)
        {
            glfwd_ERROR("Cannot convert {} into normalized color, length is greater than 6 ({})",
                        rgb_hex,
                        rgb_hex.length());
            return false;
        }

        return true;
    }

} // namespace intern

glm::vec3 Color::NormalizeRGB(uint8_t r, uint8_t g, uint8_t b)
{
    return glm::vec3{
        (float)r / 255.0f,
        (float)g / 255.0f,
        (float)b / 255.0f,
    };
}

glm::vec4 Color::NormalizeRGBA(uint8_t r, uint8_t g, uint8_t b, uint8_t a)
{
    return glm::vec4{
        (float)r / 255.0f,
        (float)g / 255.0f,
        (float)b / 255.0f,
        (float)a / 255.0f,
    };
}

glm::vec3 Color::ConvertFromtHexRGB(std::string_view rgb_hex)
{
    size_t offset, source_length;
    if (intern::ConvertFromHexSetup(rgb_hex, offset, source_length))
        return glm::vec3(1.0f);

    uint8_t r = intern::ConvertHexDigitToByte(rgb_hex.substr(offset + 0, 2));
    uint8_t g = intern::ConvertHexDigitToByte(rgb_hex.substr(offset + 2, 2));
    uint8_t b = intern::ConvertHexDigitToByte(rgb_hex.substr(offset + 4, 2));

    return NormalizeRGB(r, g, b);
}

glm::vec4 Color::ConvertFromtHexRGBA(std::string_view rgba_hex)
{
    size_t offset, source_length;
    if (intern::ConvertFromHexSetup(rgba_hex, offset, source_length))
        return glm::vec4(1.0f);

    uint8_t r = intern::ConvertHexDigitToByte(rgba_hex.substr(offset + 0, 2));
    uint8_t g = intern::ConvertHexDigitToByte(rgba_hex.substr(offset + 2, 2));
    uint8_t b = intern::ConvertHexDigitToByte(rgba_hex.substr(offset + 4, 2));
    uint8_t a =
        source_length == 8 ? intern::ConvertHexDigitToByte(rgba_hex.substr(offset + 6, 2)) : 0;

    return NormalizeRGBA(r, g, b, a);
}

glm::vec4 Color::ConvertFromHexRGBA(uint32_t rgba_hex)
{
    // 0xFFAABBCC, FF is the most significant byte
    uint8_t r = rgba_hex >> 24 & 0xFF;
    uint8_t g = rgba_hex >> 16 & 0xFF;
    uint8_t b = rgba_hex >> 8 & 0xFF;
    uint8_t a = rgba_hex & 0xFF;
    return NormalizeRGBA(r, b, g, a);
}

glm::vec3 Color::ConvertHexToColorRGB(uint32_t rgb_hex)
{
    uint8_t r = rgb_hex >> 16 & 0xFF;
    uint8_t g = rgb_hex >> 8 & 0xFF;
    uint8_t b = rgb_hex & 0xFF;
    return NormalizeRGB(r, g, b);
}

} // namespace glfwd
