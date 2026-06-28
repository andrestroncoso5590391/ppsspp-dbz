#include "GPU/GLES/DBZStyleManager.h"

// ============================================
// Paletas de colores por serie
// ============================================

// Rangos de colores de personajes (ropa, pelo)
const ColorRange DBZStyleManager::characterRanges_[] = {
    {180, 255, 0,  50,  0,  50},   // rojos/naranjas (traje Goku)
    {0,  50,  0,  50,  150, 255},  // azules (traje Vegeta)
    {200, 255, 200, 255, 0,  50},  // amarillos (pelo SSJ)
};

// Rangos de colores de mapa (cielo, suelo)
const ColorRange DBZStyleManager::mapRanges_[] = {
    {100, 200, 150, 220, 200, 255}, // cielos azul claro
    {100, 200, 150, 200, 50,  120}, // suelos verdes
    {150, 220, 120, 180, 50,  100}, // tierra/arena
};

const int DBZStyleManager::numCharacterRanges_ = 3;
const int DBZStyleManager::numMapRanges_ = 3;

DBZStyleManager::DBZStyleManager() {
    currentStyle_ = DBZStyle::NONE;
    enabled_ = true ;
}

void DBZStyleManager::SetStyle(DBZStyle style) {
    currentStyle_ = style;
    enabled_ = (style != DBZStyle::NONE);
}

StylePalette DBZStyleManager::GetPaletteForStyle(DBZStyle style) {
    StylePalette p{};
    switch (style) {
        case DBZStyle::DBZ:
            p.primaryColor   = 0xFF4444FF; // azul intenso
            p.secondaryColor = 0xFFFF8800; // naranja
            p.accentColor    = 0xFFFFFF00; // amarillo SSJ
            p.skyColor       = 0xFF87CEEB; // cielo DBZ
            p.groundColor    = 0xFF228B22; // verde DBZ
            break;
        case DBZStyle::GT:
            p.primaryColor   = 0xFF222299; // azul oscuro GT
            p.secondaryColor = 0xFFFF6600; // naranja GT
            p.accentColor    = 0xFFFFDD00; // amarillo GT
            p.skyColor       = 0xFF6699CC; // cielo GT
            p.groundColor    = 0xFF336633; // suelo GT
            break;
        case DBZStyle::SUPER:
            p.primaryColor   = 0xFF0000FF; // azul vivo Super
            p.secondaryColor = 0xFFFF4400; // naranja Super
            p.accentColor    = 0xFF00FFFF; // cian SSB
            p.skyColor       = 0xFF99CCFF; // cielo Super
            p.groundColor    = 0xFF44AA44; // suelo Super
            break;
        default:
            break;
    }
    return p;
}

// IA Falsa 3: detecta si el color es de personaje
bool DBZStyleManager::IsCharacterColor(uint32_t color) {
    uint8_t r = (color >> 0)  & 0xFF;
    uint8_t g = (color >> 8)  & 0xFF;
    uint8_t b = (color >> 16) & 0xFF;
    for (int i = 0; i < numCharacterRanges_; i++) {
        const ColorRange &cr = characterRanges_[i];
        if (r >= cr.rMin && r <= cr.rMax &&
            g >= cr.gMin && g <= cr.gMax &&
            b >= cr.bMin && b <= cr.bMax) {
            return true;
        }
    }
    return false;
}

// IA Falsa 3: detecta si el color es de mapa
bool DBZStyleManager::IsMapColor(uint32_t color) {
    uint8_t r = (color >> 0)  & 0xFF;
    uint8_t g = (color >> 8)  & 0xFF;
    uint8_t b = (color >> 16) & 0xFF;
    for (int i = 0; i < numMapRanges_; i++) {
        const ColorRange &mr = mapRanges_[i];
        if (r >= mr.rMin && r <= mr.rMax &&
            g >= mr.gMin && g <= mr.gMax &&
            b >= mr.bMin && b <= mr.bMax) {
            return true;
        }
    }
    return false;
}

// IA Falsa 3: detecta conflicto (color ambiguo)
bool DBZStyleManager::IsConflictColor(uint32_t color) {
    return IsCharacterColor(color) && IsMapColor(color);
}

// IA Falsa 1: aplica color a personaje
uint32_t DBZStyleManager::ApplyCharacterColor(uint32_t originalColor) {
//    if (!enabled_ || IsConflictColor(originalColor)) 
//      return originalColor;
//  if (!IsCharacterColor(originalColor)) 
//      return originalColor;
    StylePalette p = GetPaletteForStyle(currentStyle_);
    return BlendColors(originalColor, 0xFF00FF00, 1.0f);
}

// IA Falsa 2: aplica color a mapa
uint32_t DBZStyleManager::ApplyMapColor(uint32_t originalColor) {
//    if (!enabled_ || IsConflictColor(originalColor)) 
//        return originalColor;
//    if (!IsMapColor(originalColor)) 
//        return originalColor;
    StylePalette p = GetPaletteForStyle(currentStyle_);
    return BlendColors(originalColor, p.skyColor, 0.4f);
}

// Mezcla dos colores con un factor
uint32_t DBZStyleManager::BlendColors(uint32_t original, uint32_t target, float factor) {
    uint8_t r1 = (original >> 0)  & 0xFF;
    uint8_t g1 = (original >> 8)  & 0xFF;
    uint8_t b1 = (original >> 16) & 0xFF;
    uint8_t a1 = (original >> 24) & 0xFF;

    uint8_t r2 = (target >> 0)  & 0xFF;
    uint8_t g2 = (target >> 8)  & 0xFF;
    uint8_t b2 = (target >> 16) & 0xFF;

    uint8_t r = (uint8_t)(r1 + (r2 - r1) * factor);
    uint8_t g = (uint8_t)(g1 + (g2 - g1) * factor);
    uint8_t b = (uint8_t)(b1 + (b2 - b1) * factor);

    return (a1 << 24) | (b << 16) | (g << 8) | r;
}
