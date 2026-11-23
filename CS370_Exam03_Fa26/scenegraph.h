#include "MatNode.h"
#include "TexNode.h"

// Scene graph nodes
MatNode base;
TexNode tree;
MatNode star;
TexNode skirt;
MatNode tray;
TexNode turkey;
TexNode table;
TexNode present;

// Global node angles
GLfloat theta = 0.0f;
GLfloat rpm = 25.0f;

void build_lighting_node(MatNode& node, GLuint obj, GLuint material, GLboolean transparent, mat4 base_trans);
void build_texture_node(TexNode& node, GLuint obj, GLuint texture, mat4 base_trans);

// Create and attach scene graph nodes
void build_scene_graph( ) {

    if (mode > 1) {
        /******************************************/
        /*       INSERT (d) CODE HERE             */
        /******************************************/
        // TODO: Add base node (lighting)
        mat4 baseMatrix = scale(BASE_WIDTH, BASE_HEIGHT, BASE_DEPTH);
        build_lighting_node(base, Cylinder, GreenPlastic, false, baseMatrix);
        base.set_update_transform(translate(TREE_X, BASE_HEIGHT, TREE_Z));


        /******************************************/
        /*       INSERT (e) CODE HERE             */
        /******************************************/
        // TODO: Add tree node (texture)
        mat4 treeMatrix = scale(TREE_WIDTH, TREE_HEIGHT, TREE_DEPTH);
        build_texture_node(tree, Cone, Tree, treeMatrix);
        tree.set_update_transform(translate(0.0f, BASE_HEIGHT + TREE_HEIGHT, 0.0f));


        /******************************************/
        /*       INSERT (f) CODE HERE             */
        /******************************************/
        // TODO: Add star node (lighting)
        mat4 starMatrix = scale(STAR_WIDTH, STAR_HEIGHT, STAR_DEPTH);
        build_lighting_node(star, Octahedron, WhitePlastic, false, starMatrix);
        star.set_update_transform(translate(0.0f,  STAR_HEIGHT + TREE_HEIGHT, 0.0f));


        /******************************************/
        /*       INSERT (g) CODE HERE             */
        /******************************************/
        // TODO: Attach base node
        base.attach_nodes(&tree, NULL);

        // TODO: Attach tree node
        tree.attach_nodes(&star, NULL);

        // TODO: Attach star node
        star.attach_nodes(NULL, NULL);

    }
    if (mode > 2) {
        /******************************************/
        /*       INSERT (i) CODE HERE             */
        /******************************************/
        // TODO: Add skirt node (texture)
        mat4 skirtMatrix = scale(SKIRT_WIDTH, SKIRT_HEIGHT, SKIRT_DEPTH);
        build_texture_node(skirt, Circle, Skirt, skirtMatrix);
        skirt.set_update_transform(translate(TREE_X,  0.11f, TREE_Z));


        // TODO: Attach skirt (and base) node
        base.attach_nodes(&tree, &skirt);
        skirt.attach_nodes(NULL, NULL);

    }
    if (mode > 3) {
        /******************************************/
        /*       INSERT (k) CODE HERE             */
        /******************************************/
        // TODO: Add table node (texture)
        mat4 tableMatrix = scale(TABLE_WIDTH, TABLE_HEIGHT, TABLE_DEPTH);
        build_texture_node(table, TableCube, TableCloth, tableMatrix);
        table.set_update_transform(translate(0.0f, TABLE_HEIGHT, 0.0f));

        // TODO: Attach table (and skirt) node
        skirt.attach_nodes(NULL, &table);
        table.attach_nodes(NULL, NULL);

    }
    if (mode > 4) {
        /******************************************/
        /*       INSERT (l) CODE HERE             */
        /******************************************/
        // TODO: Add tray node (lighting)
        mat4 trayMatrix = scale(TRAY_WIDTH, TRAY_HEIGHT, TRAY_DEPTH);
        build_lighting_node(tray, Tray, Silver, false, trayMatrix);
        tray.set_update_transform(translate(0.0f, TABLE_HEIGHT/2.0f, 0.0f));

        /******************************************/
        /*       INSERT (m) CODE HERE             */
        /******************************************/
        // TODO: Add turkey node (texture)
        mat4 turkeyMatrix = scale(TURKEY_WIDTH, TURKEY_HEIGHT, TURKEY_DEPTH);
        build_texture_node(turkey, Turkey, TurkeyTex, turkeyMatrix);
        turkey.set_update_transform(translate(0.0f, TURKEY_HEIGHT/2.0f, 0.0f)*rotate(-90.0f, vec3(1.0f, 0.0f, 0.0f)));

        // TODO: Attach tray, turkey (and table) nodes
        table.attach_nodes(&tray, NULL);
        tray.attach_nodes(&turkey, NULL);
        turkey.attach_nodes(NULL, NULL);

    }
    if (mode > 6) {
        /******************************************/
        /*       INSERT (o) CODE HERE             */
        /******************************************/
        // TODO: Add present node

        // TODO: Attach present (and skirt) nodes

    }
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

void detach_nodes() {
    base.attach_nodes(NULL, NULL);
    tree.attach_nodes(NULL, NULL);
    star.attach_nodes(NULL, NULL);
    skirt.attach_nodes(NULL, NULL);
    table.attach_nodes(NULL, NULL);
    tray.attach_nodes(NULL, NULL);
    turkey.attach_nodes(NULL, NULL);
    present.attach_nodes(NULL, NULL);
}

