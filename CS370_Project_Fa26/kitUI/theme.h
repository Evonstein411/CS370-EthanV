//
// Created by ethan on 11/24/2026.
//

#ifndef CS370_FA26_THEME_H
#define CS370_FA26_THEME_H

#include "../common/vmath.h"


//colors
const auto HUD_PANEL = vmath::vec4(0.28f, 0.22f, 0.16f, 0.92f);  // dark sand / panel bg
const auto HUD_FIELD = vmath::vec4(0.38f, 0.30f, 0.22f, 0.95f);  // text fields, dropdowns
const auto HUD_CANVAS = vmath::vec4(0.22f, 0.17f, 0.12f, 0.95f);  // top bar, slightly darker
const auto HUD_BUTTON = vmath::vec4(0.55f, 0.38f, 0.22f, 0.95f);  // terracotta buttons
const auto HUD_DROPFIELD = vmath::vec4(0.45f, 0.34f, 0.22f, 0.95f);  // dropdown label + closed box
const auto HUD_SECTION = vmath::vec4(0.36f, 0.27f, 0.18f, 0.95f);  // section bar (normal)
const auto HUD_LABEL = vmath::vec4(0.50f, 0.34f, 0.20f, 0.95f);  // section bar (normal)
const auto HUD_ACCENT      = vmath::vec4(0.85f, 0.55f, 0.25f, 1.00f);  // selected, focus ring
const auto HUD_HIGHLIGHTED = vmath::vec4(0.75f, 0.55f, 0.30f, 0.95f);  // hover / active
const auto HUD_DISABLED    = vmath::vec4(0.22f, 0.20f, 0.18f, 0.65f);  // muted
const auto ALT_TEXT_COLOR = vmath::vec4(0.78f, 0.66f, 0.54f, 1.00f);
const auto DISABLED_TEXT_COLOR = vmath::vec4(0.24, 0.22f, 0.20f, 1.0f);
const auto TEXT_COLOR = vmath::vec4(0.95f, 0.90f, 0.82f, 1.00f);

constexpr float HUD_FONT = 32.0f;
constexpr float HUD_PAD_X = 8.0f;
constexpr float HUD_PAD_Y = 8.0f;


#endif //CS370_FA26_THEME_H
