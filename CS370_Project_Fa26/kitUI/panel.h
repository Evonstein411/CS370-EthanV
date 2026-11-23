#ifndef CS370_FA26_PANEL_H
#define CS370_FA26_PANEL_H

#include "widget.h"
#include "rendercontext.h"
#include "layout.h"
#include <memory>
#include <vector>


struct Panel : Widget {
    struct Cell {
        Widget* widget = nullptr;
        int col = 0;
        int row = 0;
        int colSpan = 1;
        int rowSpan = 1;
        Alignment xAlign = Alignment::START;
        Alignment yAlign = Alignment::START;
    };

    std::vector<Cell> cells;
    float padX = HUD_PAD_X;
    float padY = HUD_PAD_Y;
    float gapX = 6.0f;
    float gapY = 12.0f;
    bool drawBackground = true;
    vmath::vec4 color = HUD_PANEL;

    Panel(float w = 0.0f, float h = 0.0f);
    Panel(float x, float y, float w, float h);   // convenience
    ~Panel() override;

    void add(Widget* w, int col, int row,
             int colSpan = 1, int rowSpan = 1,
             Alignment xAlign = Alignment::START,
             Alignment yAlign = Alignment::START);

    virtual void layout();
    void set_layout(std::unique_ptr<Layout> newLayout);
    void draw_self(const RenderContext& ctx) override;

    private:
        std::unique_ptr<Layout> layout_;
};

#endif