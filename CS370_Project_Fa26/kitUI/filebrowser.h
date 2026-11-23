#ifndef CS370_FA26_FILEBROWSER_H
#define CS370_FA26_FILEBROWSER_H

#include "widget.h"
#include "panel.h"
#include "scrollpanel.h"
#include "label.h"
#include "button.h"
#include "rendercontext.h"
#include <string>
#include <vector>
#include <functional>
#include <filesystem>

namespace fs = std::filesystem;

struct FileBrowser : Widget {
    enum class StartLocation { HOME, CWD };
    StartLocation startLocation = StartLocation::HOME;

    std::string currentPath;
    bool open = false;
    std::vector<std::string> allowedExtensions;

    Panel* content = nullptr;
    Label* pathLabel = nullptr;
    ScrollPanel* listPanel = nullptr;
    Button* closeButton = nullptr;
    Label* selectedLabel = nullptr;
    Label* filterLabel = nullptr;
    std::string selectedPath;

    std::function<void(const std::string& path)> onFileSelected;

    FileBrowser();
    ~FileBrowser() override;

    void set_start_location(StartLocation loc);
    std::string get_default_path() const;

    void set_filter(const std::vector<std::string>& extensions);
    void update_filter_label();

    void open_browser(const std::string& startPath = "");
    void close_browser();
    void center_on_screen();
    void clear_list();
    bool passes_filter(const fs::path& p) const;
    void refresh();

    void draw_self(const RenderContext& ctx) override;
};

#endif