
#ifndef CS370_FA26_PANEL_H
#define CS370_FA26_PANEL_H

#include "widget.h"

//Panel widget for widget organization
struct Panel : Widget {
    //cell holds widget and information on placement in panel
    struct Cell {
        Widget* widget = nullptr;
        int col = 0;
        int row = 0;
        int colSpan = 1;
        int rowSpan = 1;
        Alignment xAlign = Alignment::START;
        Alignment yAlign = Alignment::START;
    };

    vector<Cell> cells;
    float padX = HUD_PAD_X;
    float padY = HUD_PAD_Y;
    float gapX = 6.0f;
    float gapY = 12.0f;
    bool drawBackground = true;
    vec4 color = HUD_PANEL;


    Panel(const float x, const float y)
    : Widget(x, y, 0.0f, 0.0f) {

    }

    //destructor handle deleting cells and removing child status from widgets
    ~Panel() override {
        for (const auto & cell : cells)
            remove_child(cell.widget);
        cells.clear();
    }


    //add widget to layout
    void add(Widget* w, const int col, const int row, const int colSpan = 1, const int rowSpan = 1,
        const Alignment xAlign = Alignment::START,
        const Alignment yAlign = Alignment::START) {
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

virtual void layout() {
    const float assignedWidth = stretched ? w : 0.0f;
    stretched = false;

    if (cells.empty()) {
        w = std::max(padX * 2.0f, assignedWidth);
        h = padY * 2.0f;
        return;
    }
    if (hidden) { w = 0.0f; h = 0.0f; return; }

    int maxCol = 0, maxRow = 0;
    for (const auto & cell : cells) {
        const int c = cell.col + cell.colSpan - 1;
        const int r = cell.row + cell.rowSpan - 1;
        if (c > maxCol) maxCol = c;
        if (r > maxRow) maxRow = r;
    }
    const int nCols = maxCol + 1;
    const int nRows = maxRow + 1;

    // preferred size, per row
    vector rowColW(nRows, vector(nCols, 0.0f));
    vector rowH(nRows, 0.0f);

    // 1. Single-span cells drive the base preferred sizes
    for (const auto & c : cells) {
        if (!c.widget || c.widget->hidden) continue;
        const float pw = c.widget->prefW > 0.0f ? c.widget->prefW : c.widget->w;
        const float ph = c.widget->prefH > 0.0f ? c.widget->prefH : c.widget->h;

        if (c.colSpan == 1 && c.rowSpan == 1 && pw > rowColW[c.row][c.col])
            rowColW[c.row][c.col] = pw;
        if (c.rowSpan == 1 && ph > rowH[c.row])
            rowH[c.row] = ph;
    }

    // 2. Multi-span cells expand the columns/rows they cover if their
    //    preferred size is larger than the current sum
    for (const auto & c : cells) {
        if (!c.widget || c.widget->hidden) continue;
        const float pw = c.widget->prefW > 0.0f ? c.widget->prefW : c.widget->w;
        const float ph = c.widget->prefH > 0.0f ? c.widget->prefH : c.widget->h;

        if (c.colSpan > 1) {
            float current = 0.0f;
            for (int k = 0; k < c.colSpan; k++) {
                current += rowColW[c.row][c.col + k];
                if (k + 1 < c.colSpan) current += gapX;
            }
            if (pw > current) {
                const float extra = pw - current;
                const float add = extra / static_cast<float>(c.colSpan);
                for (int k = 0; k < c.colSpan; k++)
                    rowColW[c.row][c.col + k] += add;
            }
        }

        if (c.rowSpan > 1) {
            float current = 0.0f;
            for (int k = 0; k < c.rowSpan; k++) {
                current += rowH[c.row + k];
                if (k + 1 < c.rowSpan) current += gapY;
            }
            if (ph > current) {
                const float extra = ph - current;
                const float add = extra / static_cast<float>(c.rowSpan);
                for (int k = 0; k < c.rowSpan; k++)
                    rowH[c.row + k] += add;
            }
        }
    }

    auto rowWidth = [&](const int r) {
        float s = 0.0f;
        for (int c = 0; c < nCols; c++) {
            s += rowColW[r][c];
            if (c + 1 < nCols && rowColW[r][c] > 0.0f) s += gapX;
        }
        return s;
    };

    // panel content width = widest row, or whatever a parent stretched us to
    float contentW = 0.0f;
    for (int r = 0; r < nRows; r++)
        contentW = std::max(contentW, rowWidth(r));
    if (assignedWidth > contentW + padX * 2.0f)
        contentW = assignedWidth - padX * 2.0f;

    // a short row gives its leftover to STRETCH cells in that row
    for (int r = 0; r < nRows; r++) {
        const float extra = contentW - rowWidth(r);
        if (extra <= 1.0f) continue;

        int growers = 0;
        for (const auto & c : cells) {
            if (!c.widget || c.widget->hidden || c.row != r || c.rowSpan != 1) continue;
            if (c.xAlign == Alignment::STRETCH) growers++;
        }
        if (growers == 0) continue;

        const float add = extra / static_cast<float>(growers);
        for (const auto & c : cells) {
            if (!c.widget || c.widget->hidden || c.row != r || c.rowSpan != 1) continue;
            if (c.xAlign != Alignment::STRETCH) continue;
            for (int k = 0; k < c.colSpan; k++)
                rowColW[r][c.col + k] += add / static_cast<float>(c.colSpan);
        }
    }

    // positions from the (now stretched) per-row columns
    vector rowColX(nRows, vector(nCols + 1, 0.0f));
    vector rowY(nRows + 1, 0.0f);
    for (int r = 0; r < nRows; r++)
        for (int c = 0; c < nCols; c++)
            rowColX[r][c + 1] = rowColX[r][c] + rowColW[r][c]
                              + (c < nCols - 1 && rowColW[r][c] > 0.0f ? gapX : 0.0f);
    for (int r = 0; r < nRows; r++)
        rowY[r + 1] = rowY[r] + rowH[r] + (r < nRows - 1 && rowH[r] > 0.0f ? gapY : 0.0f);

    for (auto &[widget, col, row, colSpan, rowSpan, xAlign, yAlign] : cells) {
        if (!widget) continue;

        float cw = 0.0f, ch = 0.0f;
        for (int k = 0; k < colSpan; k++) {
            cw += rowColW[row][col + k];
            if (k + 1 < colSpan) cw += gapX;
        }
        for (int k = 0; k < rowSpan; k++) {
            ch += rowH[row + k];
            if (k + 1 < rowSpan) ch += gapY;
        }
        const float px = x + padX + rowColX[row][col];
        const float py = y + padY + rowY[row];

        if (xAlign == Alignment::STRETCH) {
            widget->w = cw;
            widget->x = px;
            widget->stretched = true;
        } else if (xAlign == Alignment::CENTER) {
            widget->x = px + (cw - widget->w) * 0.5f;
        } else if (xAlign == Alignment::END) {
            widget->x = px + (cw - widget->w);
        } else {
            widget->x = px;
        }

        if (yAlign == Alignment::STRETCH) {
            widget->h = ch;
            widget->y = py;
        } else if (yAlign == Alignment::CENTER) {
            widget->y = py + (ch - widget->h) * 0.5f;
        } else if (yAlign == Alignment::END) {
            widget->y = py + (ch - widget->h);
        } else {
            widget->y = py;
        }
    }

    w = padX * 2.0f + contentW;
    if (assignedWidth > w) w = assignedWidth;
    h = padY * 2.0f + rowY[nRows];
}//end layout

    void draw_self() override {
        //update layout before drawing
        layout();
        if (drawBackground) {
            const mat4 transMatrix = translate(x, static_cast<float>(hh) - h - y, 0.0f);
            const mat4 scaleMatrix = scale(w, h, 1.0f);
            modelMatrix = transMatrix * scaleMatrix;
            draw_solid_hud_quad(color);
        }

    }//end draw()

};//end Panel


#endif //CS370_FA26_PANEL_H
