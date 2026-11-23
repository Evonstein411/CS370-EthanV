#include "gridlayout.h"
#include "panel.h"
#include <algorithm>
#include <vector>

void GridLayout::apply(Panel& panel) {
    auto& cells = panel.cells;
    const float padX = panel.padX;
    const float padY = panel.padY;
    const float gapX = panel.gapX;
    const float gapY = panel.gapY;

    const float assignedWidth = panel.stretched ? panel.w : 0.0f;
    panel.stretched = false;

    if (cells.empty()) {
        panel.w = std::max(padX * 2.0f, assignedWidth);
        panel.h = padY * 2.0f;
        return;
    }
    if (panel.hidden) {
        panel.w = 0.0f;
        panel.h = 0.0f;
        return;
    }

    int maxCol = 0, maxRow = 0;
    for (const auto& c : cells) {
        maxCol = std::max(maxCol, c.col + c.colSpan - 1);
        maxRow = std::max(maxRow, c.row + c.rowSpan - 1);
    }
    const int nCols = maxCol + 1;
    const int nRows = maxRow + 1;

    std::vector<std::vector<float>> colW(nRows, std::vector<float>(nCols, 0.0f));
    std::vector<float> rowH(nRows, 0.0f);

    auto prefW = [](const Widget* w) { return w->prefW > 0.0f ? w->prefW : w->w; };
    auto prefH = [](const Widget* w) { return w->prefH > 0.0f ? w->prefH : w->h; };

    // pass 1: single-span preferred sizes
    for (const auto& c : cells) {
        if (!c.widget || c.widget->hidden) continue;
        if (c.colSpan == 1 && c.rowSpan == 1)
            colW[c.row][c.col] = std::max(colW[c.row][c.col], prefW(c.widget));
        if (c.rowSpan == 1)
            rowH[c.row] = std::max(rowH[c.row], prefH(c.widget));
    }

    // pass 2: multi-span expansion
    auto expand = [](auto& tracks, int start, int span, float needed, float gap) {
        float current = 0.0f;
        for (int k = 0; k < span; ++k) {
            current += tracks[start + k];
            if (k + 1 < span) current += gap;
        }
        if (needed > current) {
            float add = (needed - current) / static_cast<float>(span);
            for (int k = 0; k < span; ++k) tracks[start + k] += add;
        }
    };

    for (const auto& c : cells) {
        if (!c.widget || c.widget->hidden) continue;
        if (c.colSpan > 1)
            expand(colW[c.row], c.col, c.colSpan, prefW(c.widget), gapX);
        if (c.rowSpan > 1)
            expand(rowH, c.row, c.rowSpan, prefH(c.widget), gapY);
    }

    auto rowWidth = [&](int r) {
        float s = 0.0f;
        for (int c = 0; c < nCols; ++c) {
            s += colW[r][c];
            if (c + 1 < nCols && colW[r][c] > 0.0f) s += gapX;
        }
        return s;
    };

    float contentW = 0.0f;
    for (int r = 0; r < nRows; ++r)
        contentW = std::max(contentW, rowWidth(r));
    if (assignedWidth > contentW + padX * 2.0f)
        contentW = assignedWidth - padX * 2.0f;

    // distribute leftover to STRETCH cells
    for (int r = 0; r < nRows; ++r) {
        float extra = contentW - rowWidth(r);
        if (extra <= 1.0f) continue;

        int growers = 0;
        for (const auto& c : cells)
            if (c.widget && !c.widget->hidden && c.row == r &&
                c.rowSpan == 1 && c.xAlign == Alignment::STRETCH)
                ++growers;
        if (!growers) continue;

        float add = extra / static_cast<float>(growers);
        for (const auto& c : cells) {
            if (!c.widget || c.widget->hidden || c.row != r || c.rowSpan != 1) continue;
            if (c.xAlign != Alignment::STRETCH) continue;
            for (int k = 0; k < c.colSpan; ++k)
                colW[r][c.col + k] += add / static_cast<float>(c.colSpan);
        }
    }

    // prefix sums
    std::vector<std::vector<float>> colX(nRows, std::vector<float>(nCols + 1, 0.0f));
    std::vector<float> rowY(nRows + 1, 0.0f);

    for (int r = 0; r < nRows; ++r)
        for (int c = 0; c < nCols; ++c)
            colX[r][c + 1] = colX[r][c] + colW[r][c] +
                             (c < nCols - 1 && colW[r][c] > 0.0f ? gapX : 0.0f);

    for (int r = 0; r < nRows; ++r)
        rowY[r + 1] = rowY[r] + rowH[r] +
                      (r < nRows - 1 && rowH[r] > 0.0f ? gapY : 0.0f);

    // place children
    for (auto& c : cells) {
        if (!c.widget) continue;

        float cw = 0.0f, ch = 0.0f;
        for (int k = 0; k < c.colSpan; ++k) {
            cw += colW[c.row][c.col + k];
            if (k + 1 < c.colSpan) cw += gapX;
        }
        for (int k = 0; k < c.rowSpan; ++k) {
            ch += rowH[c.row + k];
            if (k + 1 < c.rowSpan) ch += gapY;
        }

        float px = panel.x + padX + colX[c.row][c.col];
        float py = panel.y + padY + rowY[c.row];

        if (c.xAlign == Alignment::STRETCH) {
            c.widget->w = cw;
            c.widget->x = px;
            c.widget->stretched = true;
        } else if (c.xAlign == Alignment::CENTER) {
            c.widget->x = px + (cw - c.widget->w) * 0.5f;
        } else if (c.xAlign == Alignment::END) {
            c.widget->x = px + (cw - c.widget->w);
        } else {
            c.widget->x = px;
        }

        if (c.yAlign == Alignment::STRETCH) {
            c.widget->h = ch;
            c.widget->y = py;
        } else if (c.yAlign == Alignment::CENTER) {
            c.widget->y = py + (ch - c.widget->h) * 0.5f;
        } else if (c.yAlign == Alignment::END) {
            c.widget->y = py + (ch - c.widget->h);
        } else {
            c.widget->y = py;
        }
    }

    panel.w = padX * 2.0f + contentW;
    if (assignedWidth > panel.w) panel.w = assignedWidth;
    panel.h = padY * 2.0f + rowY[nRows];
}