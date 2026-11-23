
#ifndef CS370_FA26_LABEL_H
#define CS370_FA26_LABEL_H

#include <utility>

#include "widget.h"


#include "widget.h"
#include "rendercontext.h"
#include <string>

struct Label : Widget {
    std::string label;
    float textSize = HUD_FONT;
    vmath::vec4 color = HUD_LABEL;

    Label(float w, float h, std::string label = "");

    void set_text(const std::string& newText);
    void fit(const RenderContext& ctx);

    void draw_self(const RenderContext& ctx) override;
};



#endif //CS370_FA26_LABEL_H
