#ifndef CS370_FA26_SCENE_H
#define CS370_FA26_SCENE_H


#include <fstream>
#include <sstream>
#include <filesystem>

extern SceneObject* selectedObject;

namespace fs = std::filesystem;

void clear_scene() {
    for (SceneObject* obj : objects)
        delete obj;
    objects.clear();
    selectedObject = nullptr;
}

void save_scene_file(const string& filename) {
// ensure scenes/ exists (relative to bin/)
    fs::path scenesDir = "../scenes";
    if (!fs::exists(scenesDir)) {
        fs::create_directories(scenesDir);
    }

    fs::path fullPath = scenesDir / filename;
    // make sure it ends with .df
    if (fullPath.extension() != ".df")
        fullPath += ".df";

    ofstream out(fullPath);
    if (!out) {
        fprintf(stderr, "Failed to open %s for writing\n", fullPath.string().c_str());
        return;
    }

    out << "DESERTFOX_SCENE 1\n";
    out << "objects " << objects.size() << "\n\n";

    for (SceneObject* obj : objects) {
        if (!obj) continue;

        out << "OBJECT\n";
        out << "name " << obj->name << "\n";

        string meshName = "none";
        if (obj->mesh != (GLuint)-1 && obj->mesh < meshNames.size())
            meshName = meshNames[obj->mesh];
        out << "mesh " << meshName << "\n";

        out << "drawType " << drawTypeNames[obj->drawType] << "\n";

        out << "position " << obj->position[0] << " " << obj->position[1] << " " << obj->position[2] << "\n";
        out << "rotation " << obj->rotation[0] << " " << obj->rotation[1] << " " << obj->rotation[2] << "\n";
        out << "scale "    << obj->scale[0]    << " " << obj->scale[1]    << " " << obj->scale[2]    << "\n";

        if (obj->drawType == SOLID_COLOR) {
            out << "color " << obj->color[0] << " " << obj->color[1] << " "
                            << obj->color[2] << " " << obj->color[3] << "\n";
        }
        else if (obj->drawType == MATERIAL) {
            const auto& m = obj->materialProperties;
            out << "material\n";
            out << "ambient "   << m.ambient[0]   << " " << m.ambient[1]   << " " << m.ambient[2]   << " " << m.ambient[3]   << "\n";
            out << "diffuse "   << m.diffuse[0]   << " " << m.diffuse[1]   << " " << m.diffuse[2]   << " " << m.diffuse[3]   << "\n";
            out << "specular "  << m.specular[0]  << " " << m.specular[1]  << " " << m.specular[2]  << " " << m.specular[3]  << "\n";
            out << "shininess " << m.shininess << "\n";
        }
        else if (obj->drawType == TEXTURE) {
            string texName = "blank";
            for (size_t i = 0; i < TextureIDs.size(); ++i) {
                if (TextureIDs[i] == obj->texture) {
                    texName = textureNames[i];
                    break;
                }
            }
            out << "texture " << texName << "\n";
        }

        out << "END\n\n";
    }

    out.close();
    printf("\nSaved scene to %s\n", fullPath.string().c_str());
}

void load_scene_file(const string& path) {
    ifstream in(path);
    if (!in) {
        fprintf(stderr, "Failed to open %s for reading\n", path.c_str());
        return;
    }

    clear_scene();

    string line;
    string currentName, meshName, drawTypeStr, texName;
    vec3 position{0}, rotation{0}, scale{1};
    vec4 color{1};
    MaterialProperties mat = baseMaterial;
    bool inObject = false;
    bool readingMaterial = false;

    auto trim = [](string s) {
        s.erase(s.begin(), find_if(s.begin(), s.end(), [](unsigned char c){ return !isspace(c); }));
        s.erase(find_if(s.rbegin(), s.rend(), [](unsigned char c){ return !isspace(c); }).base(), s.end());
        return s;
    };

    while (getline(in, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#') continue;

        if (line == "OBJECT") {
            inObject = true;
            readingMaterial = false;
            currentName.clear();
            meshName = "none";
            drawTypeStr = "None";
            texName = "blank";
            position = {0,0,0};
            rotation = {0,0,0};
            scale = {1,1,1};
            color = {1,1,1,1};
            mat = baseMaterial;
            continue;
        }
        if (line == "END" && inObject) {
            // create the object
            int meshIdx = find_mesh(meshName);
            if (meshIdx < 0) {
                fprintf(stderr, "Warning: mesh '%s' not found, skipping object '%s'\n",
                        meshName.c_str(), currentName.c_str());
                inObject = false;
                continue;
            }

            SceneObject* obj = new SceneObject(currentName);   // empty ctor
            obj->mesh = (GLuint)meshIdx;
            obj->position = position;
            obj->rotation = rotation;
            obj->scale = scale;

            DrawType dt = NONE;
            for (int i = 0; i < (int)drawTypeNames.size(); ++i)
                if (drawTypeNames[i] == drawTypeStr) { dt = (DrawType)i; break; }

            obj->drawType = dt;
            if (dt == SOLID_COLOR) {
                obj->color = color;
                obj->hasColor = true;
                obj->initializeType(SOLID_COLOR);
            }
            else if (dt == MATERIAL) {
                obj->materialProperties = mat;
                obj->initializeType(MATERIAL);
            }
            else if (dt == TEXTURE) {
                // resolve texture name → GLuint
                GLuint tex = TextureIDs[Blank];
                for (size_t i = 0; i < textureNames.size(); ++i) {
                    if (textureNames[i] == texName) {
                        tex = TextureIDs[i];
                        break;
                    }
                }
                obj->texture = tex;
                obj->initializeType(TEXTURE);
            }
            else {
                obj->initializeType(NONE);
            }

            inObject = false;
            continue;
        }

        if (!inObject) continue;

        istringstream iss(line);
        string key;
        iss >> key;

        if (key == "name") {
            getline(iss, currentName);
            currentName = trim(currentName);
        }
        else if (key == "mesh") {
            iss >> meshName;
        }
        else if (key == "drawType") {
            getline(iss, drawTypeStr);
            drawTypeStr = trim(drawTypeStr);
        }
        else if (key == "position") {
            iss >> position[0] >> position[1] >> position[2];
        }
        else if (key == "rotation") {
            iss >> rotation[0] >> rotation[1] >> rotation[2];
        }
        else if (key == "scale") {
            iss >> scale[0] >> scale[1] >> scale[2];
        }
        else if (key == "color") {
            iss >> color[0] >> color[1] >> color[2] >> color[3];
        }
        else if (key == "material") {
            readingMaterial = true;
        }
        else if (key == "ambient" && readingMaterial) {
            iss >> mat.ambient[0] >> mat.ambient[1] >> mat.ambient[2] >> mat.ambient[3];
        }
        else if (key == "diffuse" && readingMaterial) {
            iss >> mat.diffuse[0] >> mat.diffuse[1] >> mat.diffuse[2] >> mat.diffuse[3];
        }
        else if (key == "specular" && readingMaterial) {
            iss >> mat.specular[0] >> mat.specular[1] >> mat.specular[2] >> mat.specular[3];
        }
        else if (key == "shininess" && readingMaterial) {
            iss >> mat.shininess;
        }
        else if (key == "texture") {
            iss >> texName;
        }
    }

    in.close();
    printf("Loaded scene from %s (%zu objects)\n", path.c_str(), objects.size());
}



#endif //CS370_FA26_SCENE_H
