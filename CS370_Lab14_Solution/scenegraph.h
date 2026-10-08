#include "MatNode.h"
#include "TexNode.h"
#include "Const.h"

// Scene graph nodes
MatNode base;
MatNode lower_arm;
MatNode left_upper_arm;
MatNode right_upper_arm;
TexNode earth;

// Global node angles
GLfloat theta = 0.0f;
GLfloat dtheta = 1.0f;
GLfloat phi = 0.0f;
GLfloat dphi = 1.0f;
GLfloat left_psi = 0.0f;
GLfloat right_psi = 0.0f;
GLfloat dpsi = 1.0f;
GLfloat earth_angle = 0.0f;
GLfloat rpm = 10.0f;

void build_lighting_node(MatNode& node, GLuint obj, GLuint material, GLboolean transparent, mat4 base_trans);
void build_texture_node(TexNode& node, GLuint obj, GLuint texture, mat4 base_trans);

// Create and attach scene graph nodes
void build_scene_graph( ) {
	// TODO: Add base node
    build_lighting_node(base, Cylinder, RedPlastic, false, scale(vec3(BASE_RADIUS, BASE_HEIGHT, BASE_RADIUS)));
	base.set_update_transform(rotate(theta, vec3(0.0f, 1.0f, 0.0f)));
    base.attach_nodes(&lower_arm, &earth);

	// TODO: Add lower arm node (child of base)
    build_lighting_node(lower_arm, Cube, Brass, false, translate(vec3(0.0f, LOWER_HEIGHT/2, 0.0f))*scale(vec3(LOWER_WIDTH, LOWER_HEIGHT, LOWER_DEPTH)));
    lower_arm.set_update_transform(translate(vec3(0.0f, BASE_HEIGHT, 0.0f))*rotate(phi, vec3(1.0f, 0.0f, 0.0f)));
    lower_arm.attach_nodes(&left_upper_arm, NULL);

	// TODO: Add left upper arm node (child of lower arm)
    build_lighting_node(left_upper_arm, Cube, RedPlastic, false, translate(vec3(0.0f, UPPER_HEIGHT/2, 0.0f))*scale(vec3(UPPER_WIDTH, UPPER_HEIGHT, UPPER_DEPTH)));
    left_upper_arm.set_update_transform(translate(vec3((LOWER_WIDTH+UPPER_WIDTH)/2, LOWER_HEIGHT, 0.0f))*rotate(left_psi, vec3(1.0f, 0.0f, 0.0f)));
    left_upper_arm.attach_nodes(NULL, &right_upper_arm);

	// TODO: Add right upper arm node (sibling of left upper arm)
    build_lighting_node(right_upper_arm, Cube, RedPlastic, false, translate(vec3(0.0f, UPPER_HEIGHT/2, 0.0f))*scale(vec3(UPPER_WIDTH, UPPER_HEIGHT, UPPER_DEPTH)));
    right_upper_arm.set_update_transform(translate(vec3((-LOWER_WIDTH-UPPER_WIDTH)/2, LOWER_HEIGHT, 0.0f))*rotate(right_psi, vec3(1.0f, 0.0f, 0.0f)));
    right_upper_arm.attach_nodes(NULL, NULL);

	// TODO: Add earth node (sibling of base)
    build_texture_node(earth, Sphere, Earth, scale(1.5f, 1.5f, 1.5f));
    earth.set_update_transform(translate(vec3(0.0f, 3.0f, 5.0f))*rotate(earth_angle,vec3(0.0f, 1.0f, 0.0f)));
    earth.attach_nodes(NULL, NULL);
}

// Create node with lighting and materials
void build_lighting_node(MatNode& node, GLuint obj, GLuint material, GLboolean transparent, mat4 base_trans){
    // Set shader program and matrix references
    node.set_shader(lighting_program, lighting_proj_mat_loc, lighting_camera_mat_loc, lighting_norm_mat_loc, lighting_model_mat_loc);
    // Set object buffers
    node.set_buffers(VAOs[obj], ObjBuffers[obj][PosBuffer], lighting_vPos, posCoords, ObjBuffers[obj][NormBuffer], lighting_vNorm, normCoords, numVertices[obj]);
    // Set material buffers and material
    node.set_materials(MaterialBuffers[MaterialBuffer], lighting_materials_block_idx, Materials.size()*sizeof(MaterialProperties), lighting_material_loc, material, transparent);
    // Set light buffers
    node.set_lights(LightBuffers[LightBuffer], lighting_lights_block_idx, Lights.size()*sizeof(LightProperties), lighting_num_lights_loc, Lights.size(), lighting_light_on_loc, lightOn);
    // Set eye position
    node.set_eye(lighting_eye_loc, eye);
    // Set base transform
    node.set_base_transform(base_trans);
    // Set default update transform and nodes
    node.set_update_transform(mat4().identity());
    node.attach_nodes(NULL, NULL);
}

// Create node with texture
void build_texture_node(TexNode& node, GLuint obj, GLuint texture, mat4 base_trans){
    // Set shader program and matrix references
    node.set_shader(texture_program, texture_proj_mat_loc, texture_camera_mat_loc, texture_model_mat_loc);
    // Set object buffers
    node.set_buffers(VAOs[obj], ObjBuffers[obj][PosBuffer], texture_vPos, posCoords, ObjBuffers[obj][TexBuffer], texture_vTex, texCoords, numVertices[obj]);
    // Set texture
    node.set_texture(TextureIDs[texture]);
    // Set base transform
    node.set_base_transform(base_trans);
    // Default update transform and nodes
    node.set_update_transform(mat4().identity());
    node.attach_nodes(NULL, NULL);
}

// Scene graph traversal algorithm
void traverse_scene_graph(BaseNode *node, mat4 baseTransform) {
    // Depth first traversal of child/sibling tree
    mat4 model_matrix;

    // Stop when at bottom of branch
    if (node == NULL) {
        return;
    }

    // Apply local transformation and render
    model_matrix = baseTransform*node->ModelTransform;

    node->draw(proj_matrix, camera_matrix, model_matrix);

    // Recurse vertically if possible (depth-first)
    if (node->Child != NULL) {
        traverse_scene_graph(node->Child, model_matrix);
    }

    // Remove local transformation and recurse horizontal
    if (node->Sibling != NULL) {
        traverse_scene_graph(node->Sibling, baseTransform);
    }
}
