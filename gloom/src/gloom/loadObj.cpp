//
// Created by martin on 2020-04-29.
//

#include "loadObj.h"
#include "mesh.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include "gloom/tiny_obj_loader.h"

Mesh loadMeshFromObj(std::string filename){
    // tinyObjLoader callback variables.
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str());
    Mesh mesh;


    // load koala mesh
    if (ret) {
        size_t index_offset = 0;

        glm::vec2 tmpTC;
        glm::vec3 tmpVert;
        glm::vec3 tmpNorm;

        // loops over the shapes
        for (size_t f = 0; f < shapes[0].mesh.num_face_vertices.size(); f++) {
            int fv = shapes[0].mesh.num_face_vertices[f];

            // loops over the faces
            for (size_t v = 0; v < fv; v++) {
                // texture coordinates
                tinyobj::index_t idx = shapes[0].mesh.indices[index_offset + v];
                tmpTC.x = attrib.texcoords[2 * idx.texcoord_index + 0];
                tmpTC.y = 1 - attrib.texcoords[2 * idx.texcoord_index + 1];
                mesh.textureCoordinates.push_back(tmpTC);

                // vertices
                tmpVert.x = attrib.vertices[3 * idx.vertex_index];
                tmpVert.y = attrib.vertices[3 * idx.vertex_index + 1];
                tmpVert.z = attrib.vertices[3 * idx.vertex_index + 2];
                mesh.vertices.push_back(tmpVert);

                // normals
                tmpNorm.x = attrib.normals[3 * idx.normal_index];
                tmpNorm.y = attrib.normals[3 * idx.normal_index + 1];
                tmpNorm.z = attrib.normals[3 * idx.normal_index + 2];
                mesh.normals.push_back(tmpNorm);
            }
            index_offset += fv;
        }

        // indices.
        // the vertices, normals, and texture coordinates are now sorted, so the indices are just an ascending array of the mesh size.
        for(int i = 0; i < shapes[0].mesh.indices.size(); i++){
            mesh.indices.push_back(i);
        }
    }
    printf("Mesh loaded with: \nvert: %zu, norm: %zu, ind: %zu, texCo: %zu\n", mesh.vertices.size(), mesh.normals.size(), mesh.indices.size(), mesh.textureCoordinates.size());
    return mesh;
}