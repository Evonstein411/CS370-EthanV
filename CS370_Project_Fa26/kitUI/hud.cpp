#include "hud.h"
#include <vector>
#include <algorithm>

namespace {
    std::vector<Widget*> widgets;
    std::vector<Widget*> trash;
    bool handlingEvent = false;
    int eventDepth = 0;
    Widget* captured = nullptr;
}

void register_widget(Widget* w) {
    if (w) widgets.push_back(w);
}

void destroy_widget(Widget* w) {
    if (!w) return;
    if (handlingEvent) {
        w->pendingDelete = true;
        w->hide();
        trash.push_back(w);
    } else {
        delete w;
    }
}

void flush_destroyed() {
    for (Widget* w : trash)
        delete w;
    trash.clear();
}



