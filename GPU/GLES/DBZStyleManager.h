#pragma once

#include <cstdint>

// ============================================
// DBZ Style Manager - Sistema de estilos DBZ
// IA Falsa 1: Colores de personajes
// IA Falsa 2: Colores de mapa
// IA Falsa 3: Supervisora anti-conflictos
// ============================================

// Estilos disponibles
enum class DBZStyle {
    NONE = 0,
    DBZ,      // Dragon Ball Z
    GT,       // Dragon Ball GT
    SUPER,    // Dragon Ball Super
    BT3,      // Budokai Tenkaichi 3 (original)
};

// Paleta de colores por estilo
struct StylePalette {
    // Colores base RGBA (0-255)
    uint32_t primaryColor;    // color principal personaje
    uint32_t secondaryColor;  // color secundario
    uint32_t accentColor;     // color acento
    uint32_t skyColor;        // color cielo/fondo
    uint32_t groundColor;     // color suelo
};

// IA Falsa 3: Rangos de color para evitar conflictos
struct ColorRange {
    uint8_t rMin, rMax;
    uint8_t gMin, gMax;
    uint8_t bMin, bMax;
};

class DBZStyleManager {
public:
    DBZStyleManager();
    ~DBZStyleManager() = default;

    // Cambia el estilo activo (botones de la UI)
    void SetStyle(DBZStyle style);
    DBZStyle GetCurrentStyle() const { return currentStyle_; }

    // IA Falsa 1: modifica color de personaje
    uint32_t ApplyCharacterColor(uint32_t originalColor);

    // IA Falsa 2: modifica color de mapa
    uint32_t ApplyMapColor(uint32_t originalColor);

    // IA Falsa 3: supervisora - detecta si es personaje o mapa
    bool IsCharacterColor(uint32_t color);
    bool IsMapColor(uint32_t color);
    bool IsConflictColor(uint32_t color);

    // Activa/desactiva el sistema
    bool IsEnabled() const { return enabled_; }
    void SetEnabled(bool enabled) { enabled_ = enabled; }

private:
    DBZStyle currentStyle_ = DBZStyle::NONE;
    bool enabled_ = false;

    StylePalette GetPaletteForStyle(DBZStyle style);

    // Rangos conocidos de colores de personajes DBZ
    // para que la IA supervisora los identifique
    static const ColorRange characterRanges_[];
    static const ColorRange mapRanges_[];
    static const int numCharacterRanges_;
    static const int numMapRanges_;

    uint32_t BlendColors(uint32_t original, uint32_t target, float factor);
};
extern DBZStyleManager g_dbzStyleManager;
