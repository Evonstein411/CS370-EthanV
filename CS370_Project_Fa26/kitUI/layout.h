//
// Created by ethan on 11/23/2026.
//

#ifndef CS370_FA26_LAYOUT_H
#define CS370_FA26_LAYOUT_H

class Panel;

struct Layout {
    virtual ~Layout() = default;
    virtual void apply(Panel& panel) = 0;
};


#endif //CS370_FA26_LAYOUT_H
