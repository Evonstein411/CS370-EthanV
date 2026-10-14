
enum DrawType {SOLID_COLOR, MATERIAL, TEXTURE};
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
    GLuint texture;
    mat4 model_matrix;
    mat4 normal_matrix;
    string name;

    SceneObject(GLuint mesh, string name, vec3 position, vec3 scale, vec3 rotation, vec4 color)
    : mesh(mesh), name(name), position(position), rotation(rotation), scale(scale), color(color) {
        objects.push_back(this);
        drawType = SOLID_COLOR;
        glGenBuffers(1, &colorBuffer);
        build_solid_color_buffer(numVertices[mesh], color, colorBuffer);
    }

    SceneObject(GLuint mesh, string name, vec3 position, vec3 scale, vec3 rotation, MaterialProperties materialProperties)
    : mesh(mesh), name(name), position(position), rotation(rotation), scale(scale), materialProperties(materialProperties) {
        objects.push_back(this);
        drawType = MATERIAL;
        DynamicMaterials.push_back(materialProperties);



    }

    SceneObject(GLuint mesh, string name, vec3 position, vec3 scale, vec3 rotation, GLuint texture)
    : mesh(mesh), name(name), position(position), rotation(rotation), scale(scale), texture(texture) {
        objects.push_back(this);
        drawType = TEXTURE;

    }

    void draw(mat4 model_matrix, mat4 normal_matrix) {
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
            draw_mat_object(mesh, material);
        }else if(drawType == TEXTURE) {
            draw_tex_object(mesh, texture);
        }






    }


};
