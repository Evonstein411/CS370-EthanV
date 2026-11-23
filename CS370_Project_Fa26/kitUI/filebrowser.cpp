#include "filebrowser.h"
#include "hud.h"
#include <algorithm>
#include <cstdlib>

FileBrowser::FileBrowser()
    : Widget(0.0f, 0.0f) {
    priority = 2;

    content = new Panel(0.0f, 0.0f);
    add_child(content);

    pathLabel = new Label(320.0f, 32.0f, "");
    content->add(pathLabel, 0, 0, 1, 1, Alignment::STRETCH, Alignment::START);

    closeButton = new Button(32.0f, 32.0f, "X");
    closeButton->on_press = [this]() { close_browser(); };
    content->add(closeButton, 1, 0, 1, 1, Alignment::END, Alignment::START);

    listPanel = new ScrollPanel(480.0f, 480.0f, 480.0f);
    content->add(listPanel, 0, 1, 2, 1, Alignment::STRETCH, Alignment::START);

    selectedLabel = new Label(200.0f, 32.0f, "(none)");
    content->add(selectedLabel, 0, 2, 1, 1, Alignment::STRETCH, Alignment::START);

    filterLabel = new Label(120.0f, 32.0f, "");
    content->add(filterLabel, 1, 2);

    content->hide();
    hide();
    open = false;

    currentPath = get_default_path();
}

FileBrowser::~FileBrowser() {
    if (content) {
        clear_list();
        delete content;
        content = nullptr;
    }
}

void FileBrowser::set_start_location(StartLocation loc) {
    startLocation = loc;
}

std::string FileBrowser::get_default_path() const {
    if (startLocation == StartLocation::CWD)
        return fs::current_path().string();

    const char* home = getenv("HOME");
    if (!home) home = getenv("USERPROFILE");
    return home ? std::string(home) : fs::current_path().string();
}

void FileBrowser::set_filter(const std::vector<std::string>& extensions) {
    allowedExtensions = extensions;
    for (auto& ext : allowedExtensions) {
        if (!ext.empty() && ext[0] != '.')
            ext = "." + ext;
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    }
    update_filter_label();
}

void FileBrowser::update_filter_label() {
    if (!filterLabel) return;
    if (allowedExtensions.empty()) {
        filterLabel->set_text("(any)");
        return;
    }
    std::string s;
    for (size_t i = 0; i < allowedExtensions.size(); ++i) {
        if (i) s += ' ';
        s += allowedExtensions[i];
    }
    filterLabel->set_text(s);
}

void FileBrowser::open_browser(const std::string& startPath) {
    show();
    currentPath = startPath.empty() ? get_default_path() : startPath;
    open = true;
    content->show();
    refresh();
    center_on_screen();
}

void FileBrowser::close_browser() {
    open = false;
    content->hide();
    hide();
}

void FileBrowser::center_on_screen() {
    if (!content) return;
    content->layout();
    w = content->w;
    h = content->h;

    // screen size now lives on the HUD
    float sw = static_cast<float>(HUD::instance().get_screen_width());
    float sh = static_cast<float>(HUD::instance().get_screen_height());
    x = (sw - w) * 0.5f;
    y = (sh - h) * 0.5f;
    content->x = x;
    content->y = y;
}

void FileBrowser::clear_list() {
    if (!listPanel) return;
    for (auto& cell : listPanel->cells) {
        if (cell.widget) {
            listPanel->remove_child(cell.widget);
            delete cell.widget;
        }
    }
    listPanel->cells.clear();
}

bool FileBrowser::passes_filter(const fs::path& p) const {
    if (allowedExtensions.empty()) return true;
    std::string ext = p.extension().string();
    std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
    for (const auto& allowed : allowedExtensions)
        if (ext == allowed) return true;
    return false;
}

void FileBrowser::refresh() {
    if (!listPanel || !pathLabel) return;

    clear_list();
    pathLabel->set_text(currentPath);

    int row = 0;

    // ".." entry
    auto* up = new Button(280.0f, 32.0f, "..");
    up->on_press = [this]() {
        fs::path p(currentPath);
        if (p.has_parent_path()) {
            currentPath = p.parent_path().string();
            refresh();
        }
    };
    listPanel->add(up, 0, row++);

    if (fs::exists(currentPath) && fs::is_directory(currentPath)) {
        std::error_code ec;
        for (fs::directory_iterator it(currentPath, fs::directory_options::skip_permission_denied, ec), end;
             !ec && it != end; it.increment(ec)) {
            const auto& entry = *it;
            std::string name = entry.path().filename().string();
            std::string full = entry.path().string();

            if (entry.is_directory()) {
                auto* b = new Button(280.0f, 32.0f, name + "/");
                b->on_press = [this, full]() {
                    currentPath = full;
                    refresh();
                };
                listPanel->add(b, 0, row++);
            } else if (passes_filter(entry.path())) {
                auto* b = new Button(280.0f, 32.0f, name);
                b->on_press = [this, full]() {
                    selectedPath = full;
                    selectedLabel->set_text(fs::path(full).filename().string());
                    if (onFileSelected) onFileSelected(full);
                    close_browser();
                };
                listPanel->add(b, 0, row++);
            }
        }
    }

    listPanel->layout();
    content->layout();
    w = content->w;
    h = content->h;
    center_on_screen();
}

void FileBrowser::draw_self(const RenderContext& /*ctx*/) {
    // drawing is handled by children
}