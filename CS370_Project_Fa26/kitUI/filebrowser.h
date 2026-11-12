#ifndef CS370_FA26_FILEBROWSER_H
#define CS370_FA26_FILEBROWSER_H

struct FileBrowser : Widget {
    string currentPath;
    bool open = false;

    vector<string> allowedExtensions;

    Panel* content = nullptr;       // the visible panel
    Label* pathLabel = nullptr;
    ScrollPanel* listPanel = nullptr;     // holds the entry buttons
    Button* closeButton = nullptr;
    Label* selectedLabel = nullptr;
    Label* filterLabel = nullptr;
    string selectedPath;

    // Optional: callback when a file is chosen
    std::function<void(const string& path)> onFileSelected;

    FileBrowser()
            : Widget(0, 0, 0, 0) {
        priority = 2;
        content = new Panel(x, y);
        add_child(content);

        pathLabel = new Label(0, 0, 320, 32, "");
        content->add(pathLabel, 0, 0, 1, 1, Alignment::STRETCH, Alignment::START);

        closeButton = new Button(0, 0, 32, 32,
            [this]() { close_browser(); }, "X");
        content->add(closeButton, 1, 0, 1, 1, Alignment::END, Alignment::START);

        listPanel = new ScrollPanel(0, 0, 480.0f);
        content->add(listPanel, 0, 1, 2, 1, Alignment::STRETCH, Alignment::START);

        selectedLabel = new Label(0, 0, 200, 32, "(none)");
        content->add(selectedLabel, 0, 2, 1, 1, Alignment::STRETCH, Alignment::START);

        filterLabel = new Label(0, 0, 120, 32, "");
        content->add(filterLabel, 1, 2);

        content->hide();
        hide();
        open = false;

        const char* home = getenv("HOME");          // Linux/macOS (losers)
        if (!home) home = getenv("USERPROFILE");    // Windows (chads)
        currentPath = home ? home : fs::current_path().string();
    }

    ~FileBrowser() {
        // Panel destructor unparents; we explicitly delete what we own
        if (content) {
            // Delete entry buttons first (same pattern as Dropdown::setOptions)
            clear_list();
            delete content;
            content = nullptr;
        }
    };

    //set allowed file extensions
    void set_filter(const vector<string>& extensions) {
        allowedExtensions = extensions;
        // Normalize to lowercase with leading dot
        for (auto& ext : allowedExtensions) {
            if (!ext.empty() && ext[0] != '.')
                ext = "." + ext;
            std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        }
        update_filter_label();
    }

    void update_filter_label() {
        if (!filterLabel) return;
        if (allowedExtensions.empty()) {
            filterLabel->set_text("(any)");
            return;
        }
        string s;
        for (size_t i = 0; i < allowedExtensions.size(); i++) {
            if (i) s += " ";
            s += allowedExtensions[i];
        }
        filterLabel->set_text(s);
    }

    void open_browser(const string& startPath = "") {
        show();
        if (!startPath.empty())
            currentPath = startPath;
        open = true;
        content->show();
        refresh();
        center_on_screen();
    }

    void close_browser() {
        open = false;
        content->hide();
        this->hide();
    }

    void center_on_screen() {
        if (!content) return;
        content->layout();
        w = content->w;
        h = content->h;
        x = (ww - w) * 0.5f;
        y = (hh - h) * 0.5f;
        content->x = x;
        content->y = y;
    }

    void clear_list() {
        if (!listPanel) return;
        for (auto& cell : listPanel->cells) {
            if (cell.widget) {
                destroy_widget(cell.widget);
            }
        }
        listPanel->cells.clear();
    }

    //helper to check if file follows current extension filter
    bool passes_filter(const fs::path& p) const {
        if (allowedExtensions.empty())
            return true;   // no filter → show all files
        string ext = p.extension().string();
        std::transform(ext.begin(), ext.end(), ext.begin(), ::tolower);
        for (const auto& allowed : allowedExtensions)
            if (ext == allowed)
                return true;
        return false;
    }

    void refresh() {
        if (!listPanel || !pathLabel) return;

        clear_list();
        pathLabel->set_text(currentPath);

        int row = 0;

        // ".." entry
        Button* up = new Button(0, 0, 280, 32,
            [this]() {
                fs::path p(currentPath);
                if (p.has_parent_path()) {
                    currentPath = p.parent_path().string();
                    refresh();
                }
            }, "..");
        listPanel->add(up, 0, row++);

        // Directory contents
        if (fs::exists(currentPath) && fs::is_directory(currentPath)) {
            std::error_code ec;
            for (fs::directory_iterator it(currentPath, fs::directory_options::skip_permission_denied, ec), end;
                 !ec && it != end; it.increment(ec)){
                const auto& entry = *it;
                string name = entry.path().filename().string();
                string full = entry.path().string();

                if (entry.is_directory()) {
                    Button* b = new Button(0, 0, 280, 32,
                        [this, full]() {
                            currentPath = full;
                            refresh();
                        }, name + "/");
                    listPanel->add(b, 0, row++);
                } else if (passes_filter(entry.path())) {
                    Button* b = new Button(0, 0, 280, 32,
                        [this, full]() {
                            selectedPath = full;
                            selectedLabel->set_text(fs::path(full).filename().string());
                            if (onFileSelected)
                                onFileSelected(full);
                            close_browser();
                        }, name);
                    listPanel->add(b, 0, row++);
                }
            }
        }

        listPanel->layout();
        content->layout();
        w = content->w;
        h = content->h;
        center_on_screen();
    }// rebuild the list of entries

    void draw_self() override {
        //draw handled by children
    }

};



#endif //CS370_FA26_FILEBROWSER_H
