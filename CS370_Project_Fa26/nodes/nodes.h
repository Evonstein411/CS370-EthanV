//
// Created by ethan on 11/16/2026.
//

#ifndef CS370_FA26_NODES_H
#define CS370_FA26_NODES_H

#include "scenenode.h"
#include "meshnode.h"
#include "lightnode.h"

inline SceneNode* create_node(const string& typeName) {
    if (typeName == "MeshNode") return new MeshNode("node");
    // if (typeName == "LightNode") return new LightNode("light");
    return new MeshNode("node");
}


#endif //CS370_FA26_NODES_H
