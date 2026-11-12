#include "MatNode.h"
#include "TexNode.h"

// Scene graph nodes
TexNode court;
TexNode torso;
TexNode head;
MatNode left_upper_arm;
MatNode right_upper_arm;
MatNode left_lower_arm;
MatNode right_lower_arm;
MatNode left_upper_leg;
MatNode right_upper_leg;
MatNode left_lower_leg;
MatNode right_lower_leg;
MatNode box;
TexNode bball;

// Global node angles
GLfloat head_theta = 0.0f;
GLfloat upper_left_arm_phi=150.0f;
GLfloat upper_right_arm_phi=210.0f;
GLfloat lower_left_arm_psi=-30.0f;
GLfloat lower_right_arm_psi=0.0f;
GLfloat upper_left_leg_phi=30.0f;
GLfloat upper_right_leg_phi=-30.0f;
GLfloat lower_left_leg_psi=0.0f;
GLfloat lower_right_leg_psi=30.0f;
GLfloat bball_theta = 0.0f;
GLfloat bball_dy = (UPPER_LEG_HEIGHT+LOWER_LEG_HEIGHT);
GLboolean bball_bounce = true;
GLfloat bball_rpm = 10.0f;

void build_lighting_node(MatNode& node, GLuint obj, GLuint material, GLboolean transparent, mat4 base_trans);
void build_texture_node(TexNode& node, GLuint obj, GLuint texture, mat4 base_trans);

// Create and attach scene graph nodes
void build_scene_graph( ) {
    mat4 torso_base = scale(TORSO_WIDTH, TORSO_HEIGHT, TORSO_DEPTH);
    build_texture_node(torso, TexCube, Shirt, torso_base);
    torso.set_update_transform(translate(0.0f, TORSO_HEIGHT * 0.5f, 0.0f));

    mat4 head_base = scale(HEAD_WIDTH, HEAD_HEIGHT, HEAD_DEPTH);
    build_texture_node(head, Sphere, Face, head_base);
    head.set_update_transform(
        translate(0.0f, TORSO_HEIGHT * 0.5f + HEAD_HEIGHT, 0.0f)
        * rotate(180.0f, vec3(1.0f, 0.0f, 0.0f))
    );
    torso.attach_nodes(&head, NULL);

    mat4 left_arm_base = scale(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, UPPER_ARM_DEPTH);
    build_lighting_node(left_upper_arm, Cube, ArmMaterial, false, left_arm_base);
    left_upper_arm.set_update_transform(
        translate(TORSO_WIDTH * 0.5f + UPPER_ARM_WIDTH * 0.5f, TORSO_HEIGHT * 0.4f, 0.0f)
        * rotate(upper_left_arm_phi, vec3(1.0f, 0.0f, 0.0f))
    );
    head.attach_nodes(NULL, &left_upper_arm);

    mat4 right_arm_base = scale(UPPER_ARM_WIDTH, UPPER_ARM_HEIGHT, UPPER_ARM_DEPTH);
    build_lighting_node(right_upper_arm, Cube, ArmMaterial, false, right_arm_base);
    right_upper_arm.set_update_transform(
        translate(-(TORSO_WIDTH * 0.5f + UPPER_ARM_WIDTH * 0.5f), TORSO_HEIGHT * 0.4f, 0.0f)
        * rotate(upper_right_arm_phi, vec3(1.0f, 0.0f, 0.0f))
    );
    left_upper_arm.attach_nodes(NULL, &right_upper_arm);

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
