#include "panel.h"
#include "gridlayout.h"
#include <algorithm>
#include <memory>

Panel::Panel(float w, float h)
    : Widget(w, h) {
    layout_ = std::make_unique<GridLayout>();
}

Panel::Panel(float x, float y, float w, float h)
    : Widget(w, h){
    layout_ = std::make_unique<GridLayout>();
    this->x = x;
    this->y = y;
}

void Panel::set_layout(std::unique_ptr<Layout> newLayout) {
    layout_ = std::move(newLayout);
}

Panel::~Panel() {
    for (const auto& cell : cells)
        remove_child(cell.widget);
    cells.clear();
}

void Panel::add(Widget* w, int col, int row,
                int colSpan, int rowSpan,
                Alignment xAlign, Alignment yAlign) {
    Cell c;
    c.widget = w;
    c.col = col;
    c.row = row;
    c.colSpan = colSpan;
    c.rowSpan = rowSpan;
    c.xAlign = xAlign;
    c.yAlign = yAlign;
    cells.push_back(c);
    add_child(w);
}

void Panel::layout() {
    if (layout_) layout_->apply(*this);
}


void Panel::draw_self(const RenderContext& ctx) {
    layout();

    if (!drawBackground) return;

    vmath::mat4 model = vmath::translate(x, y, 0.0f) * vmath::scale(w, h, 1.0f);
    ctx.draw_solid_quad(color, model);
}