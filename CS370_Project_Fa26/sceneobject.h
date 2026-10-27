
enum DrawType {SOLID_COLOR, MATERIAL, TEXTURE};
static const vector<string> drawTypeNames={
    "Solid Color",
    "Material",
    "Texture"
};
struct SceneObject;
extern vector<SceneObject*> objects;


struct SceneObject {
    GLuint mesh;
    DrawType drawType;
    vec3 position;
    vec3 rotation;
    vec3 scale;
    vec4 color;
    GLuint colorBuffer;
    MaterialProperties materialProperties;
    GLuint material;
    GLuint texture = 0;
    string name;
    bool hasMaterial = false, hasColor = false;

    //solid color
    SceneObject(GLuint mesh, string name, vec3 position, vec3 scale, vec3 rotation, vec4 color)
    : mesh(mesh), name(name), position(position), rotation(rotation), scale(scale), color(color) {
        objects.push_back(this);
        drawType = SOLID_COLOR;


        GLuint buf;
        glGenBuffers(1, &buf);
        ColorBuffers.push_back(buf);
        colorBuffer = (GLuint)ColorBuffers.size() - 1;   // index, like material
        build_solid_color_buffer(numVertices[mesh], color, colorBuffer);
        hasColor = true;

    }

    //materials
    SceneObject(GLuint mesh, string name, vec3 position, vec3 scale, vec3 rotation, MaterialProperties materialProperties)
    : mesh(mesh), name(name), position(position), rotation(rotation), scale(scale), materialProperties(materialProperties) {
        objects.push_back(this);
        drawType = MATERIAL;

        material = (GLuint)DynamicMaterials.size();   // index in the dynamic list
        DynamicMaterials.push_back(materialProperties);
        hasMaterial = true;


    }

    //texture
    SceneObject(GLuint mesh, string name, vec3 position, vec3 scale, vec3 rotation, GLuint texture)
    : mesh(mesh), name(name), position(position), rotation(rotation), scale(scale), texture(texture) {
        objects.push_back(this);
        drawType = TEXTURE;

    }

    void initializeType(DrawType newType) {
        //solid color add color if none and build buffer
        if (newType == SOLID_COLOR) {
            if(!hasColor) {
                color = {1.0f, 1.0f, 1.0f, 1.0f,}; //set to white if no color set
                GLuint buf;
                glGenBuffers(1, &buf);
                ColorBuffers.push_back(buf);
                colorBuffer = (GLuint)ColorBuffers.size() - 1;
                hasColor = true;
            }
            build_solid_color_buffer(numVertices[mesh], color, colorBuffer);
        //material add material properties if none
        } else if (newType == MATERIAL) {
            if(!hasMaterial) {
                materialProperties = baseMaterial; //use base material
                material = (GLuint)DynamicMaterials.size();   // index in the dynamic list
                DynamicMaterials.push_back(materialProperties);
                hasMaterial = true;
            }

        } else if (newType == TEXTURE) {
            if(!texture) {
                texture = TextureIDs[Blank];; //use blank texture if none
            }
        }
    }//end init type

    void setColor(vec4 new_color) {
        color = new_color;
        build_solid_color_buffer(numVertices[mesh], color, colorBuffer);

    }

    void draw() {
        mat4 trans_matrix = translate(position);
        mat4 rotate_x_matrix = vmath::rotate(rotation[0], vec3(1, 0, 0));
        mat4 rotate_y_matrix = vmath::rotate(rotation[1], vec3(0, 1, 0));
        mat4 rotate_z_matrix = vmath::rotate(rotation[2], vec3(0, 0, 1));
        mat4 scale_matrix = vmath::scale(scale);

        model_matrix = trans_matrix*rotate_x_matrix*rotate_y_matrix*rotate_z_matrix*scale_matrix;
        if (drawType == SOLID_COLOR) {
            draw_color_object(mesh, colorBuffer);
        }else if(drawType == MATERIAL) {
            normal_matrix = model_matrix.inverse().transpose();
            GLuint shaderIndex = (GLuint)Materials.size() + material;
            draw_mat_object(mesh, shaderIndex);
        }else if(drawType == TEXTURE) {
            draw_tex_object(mesh, texture);
        }
    }//end draw


};//end scene object
