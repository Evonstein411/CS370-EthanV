
#ifndef CS370_FA26_PANEL_H
#define CS370_FA26_PANEL_H

struct Panel;
//Panel widget for widget organization
struct Panel : Widget {
    //cell holds widget and information on placement in panel
    struct Cell {
        Widget* widget = NULL;
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


    Panel(float x, float y)
    : Widget(x, y, 0.0f, 0.0f) {

    }

    //destructor handle deleting cells and removing child status from widgets
    ~Panel() {
        for (size_t i = 0; i < cells.size(); i++)
            removeChild(cells[i].widget);
        cells.clear();
    }


    //add widget to layout
    void add(Widget* w, int col, int row, int colSpan = 1, int rowSpan = 1,
        Alignment xAlign = Alignment::START,
        Alignment yAlign = Alignment::START) {
        Cell c;
        c.widget = w;
        c.col = col;
        c.row = row;
        c.colSpan = colSpan;
        c.rowSpan = rowSpan;
        c.xAlign = xAlign;
        c.yAlign = yAlign;
        cells.push_back(c);
        addChild(w);
    }

virtual void layout() {
    float assignedWidth = stretched ? w : 0.0f;
    stretched = false;

    if (cells.empty()) {
        w = std::max(padX * 2.0f, assignedWidth);
        h = padY * 2.0f;
        return;
    }
    if (hidden) { w = 0.0f; h = 0.0f; return; }

    int maxCol = 0, maxRow = 0;
    for (size_t i = 0; i < cells.size(); i++) {
        int c = cells[i].col + cells[i].colSpan - 1;
        int r = cells[i].row + cells[i].rowSpan - 1;
        if (c > maxCol) maxCol = c;
        if (r > maxRow) maxRow = r;
    }
    int nCols = maxCol + 1;
    int nRows = maxRow + 1;

    // preferred size, per row
    vector<vector<float>> rowColW(nRows, vector<float>(nCols, 0.0f));
    vector<float> rowH(nRows, 0.0f);

    // 1. Single-span cells drive the base preferred sizes
    for (size_t i = 0; i < cells.size(); i++) {
        Cell& c = cells[i];
        if (!c.widget || c.widget->hidden) continue;
        float pw = c.widget->prefW > 0.0f ? c.widget->prefW : c.widget->w;
        float ph = c.widget->prefH > 0.0f ? c.widget->prefH : c.widget->h;

        if (c.colSpan == 1 && c.rowSpan == 1 && pw > rowColW[c.row][c.col])
            rowColW[c.row][c.col] = pw;
        if (c.rowSpan == 1 && ph > rowH[c.row])
            rowH[c.row] = ph;
    }

    // 2. Multi-span cells expand the columns/rows they cover if their
    //    preferred size is larger than the current sum
    for (size_t i = 0; i < cells.size(); i++) {
        Cell& c = cells[i];
        if (!c.widget || c.widget->hidden) continue;
        float pw = c.widget->prefW > 0.0f ? c.widget->prefW : c.widget->w;
        float ph = c.widget->prefH > 0.0f ? c.widget->prefH : c.widget->h;

        if (c.colSpan > 1) {
            float current = 0.0f;
            for (int k = 0; k < c.colSpan; k++) {
                current += rowColW[c.row][c.col + k];
                if (k + 1 < c.colSpan) current += gapX;
            }
            if (pw > current) {
                float extra = pw - current;
                float add = extra / (float)c.colSpan;
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
                float extra = ph - current;
                float add = extra / (float)c.rowSpan;
                for (int k = 0; k < c.rowSpan; k++)
                    rowH[c.row + k] += add;
            }
        }
    }

    auto rowWidth = [&](int r) {
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
        float extra = contentW - rowWidth(r);
        if (extra <= 1.0f) continue;

        int growers = 0;
        for (size_t i = 0; i < cells.size(); i++) {
            Cell& c = cells[i];
            if (!c.widget || c.widget->hidden || c.row != r || c.rowSpan != 1) continue;
            if (c.xAlign == Alignment::STRETCH) growers++;
        }
        if (growers == 0) continue;

        float add = extra / (float)growers;
        for (size_t i = 0; i < cells.size(); i++) {
            Cell& c = cells[i];
            if (!c.widget || c.widget->hidden || c.row != r || c.rowSpan != 1) continue;
            if (c.xAlign != Alignment::STRETCH) continue;
            for (int k = 0; k < c.colSpan; k++)
                rowColW[r][c.col + k] += add / (float)c.colSpan;
        }
    }

    // positions from the (now stretched) per-row columns
    vector<vector<float>> rowColX(nRows, vector<float>(nCols + 1, 0.0f));
    vector<float> rowY(nRows + 1, 0.0f);
    for (int r = 0; r < nRows; r++)
        for (int c = 0; c < nCols; c++)
            rowColX[r][c + 1] = rowColX[r][c] + rowColW[r][c]
                              + ((c < nCols - 1 && rowColW[r][c] > 0.0f) ? gapX : 0.0f);
    for (int r = 0; r < nRows; r++)
        rowY[r + 1] = rowY[r] + rowH[r] + ((r < nRows - 1 && rowH[r] > 0.0f) ? gapY : 0.0f);

    for (size_t i = 0; i < cells.size(); i++) {
        Cell& c = cells[i];
        if (!c.widget) continue;

        float cw = 0.0f, ch = 0.0f;
        for (int k = 0; k < c.colSpan; k++) {
            cw += rowColW[c.row][c.col + k];
            if (k + 1 < c.colSpan) cw += gapX;
        }
        for (int k = 0; k < c.rowSpan; k++) {
            ch += rowH[c.row + k];
            if (k + 1 < c.rowSpan) ch += gapY;
        }
        float px = x + padX + rowColX[c.row][c.col];
        float py = y + padY + rowY[c.row];

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

    w = padX * 2.0f + contentW;
    if (assignedWidth > w) w = assignedWidth;
    h = padY * 2.0f + rowY[nRows];
}//end layout

    void drawSelf() override {
        //update layout before drawing
        layout();
        if (drawBackground) {
            mat4 trans_matrix = translate(x, (float)hh - h - y, 0.0f);
            mat4 scale_matrix = scale(w, h, 1.0f);
            model_matrix = trans_matrix * scale_matrix;
            draw_solid_hud_quad(color);
        }

    }//end draw()

};//end Panel





#endif //CS370_FA26_PANEL_H
