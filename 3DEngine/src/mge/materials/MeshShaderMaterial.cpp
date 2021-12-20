#include "glm.hpp"

#include "MeshShaderMaterial.hpp"
#include "mge/core/ShaderProgram.hpp"
#include "mge/config.hpp"

ShaderProgram* MeshShaderMaterial::_shader = NULL;
std::vector<glm::vec4> _vertices = std::vector<glm::vec4>();
std::vector<glm::vec4> _colors = std::vector<glm::vec4>();


MeshShaderMaterial::MeshShaderMaterial() {
    happly::PLYData plyIn(config::MGE_POINT_CLOUDS_PATH + "dutch_bridge_1m.ply");

    _vertices = plyIn.getVertexPositions();
    _colors = plyIn.getVertexColors();

    std::cout << "Loaded point cloud with: " << _vertices.size() << " vertices." << std::endl;

    //every time we create an instance of MeshShaderMaterial we check if the corresponding shader has already been loaded
    _lazyInitializeShader();
    _initializeSSBOs();

    std::cout << "GL_GET_ERROR : " << glGetError() << std::endl;
}

void MeshShaderMaterial::_lazyInitializeShader() {
    //this shader contains everything the material can do
    if (!_shader) {
        _shader = new ShaderProgram();
        _shader->addShader(GL_MESH_SHADER_NV, config::MGE_SHADERS_PATH + "ms.glsl");
        _shader->addShader(GL_FRAGMENT_SHADER, config::MGE_SHADERS_PATH + "ps.glsl");
        _shader->finalize();
        
        _debug(); // prints information on mesh shaders
    }
}

void MeshShaderMaterial::_initializeSSBOs() {
    // Define buffer indices 
    GLuint ssboPosition;
    GLuint ssboColor;

    // Setup storage buffers for both position and color
    _shader->setupBufferWithIndex(&ssboPosition, GL_SHADER_STORAGE_BUFFER, 1, _vertices.size() * sizeof(glm::vec4), &_vertices.front(), GL_STATIC_DRAW);
    _shader->setupBufferWithIndex(&ssboColor, GL_SHADER_STORAGE_BUFFER, 2, _colors.size() * sizeof(glm::vec4), &_colors.front(), GL_STATIC_DRAW);
}

void MeshShaderMaterial::_debug() {
    // printing turing architecture mesh shader info
    std::cout << "----------------------------------" << std::endl;
    std::cout << "(Currently Active) Mesh Shader info:" << std::endl;
    std::cout << "----------------------------------" << std::endl;
    GLint meshVerticesOut;
    GLint meshPrimtivesOut;
    GLint meshOutputType;

    _shader->getProgramiv(GL_MESH_VERTICES_OUT_NV, &meshVerticesOut);
    _shader->getProgramiv(GL_MESH_PRIMITIVES_OUT_NV, &meshPrimtivesOut);
    _shader->getProgramiv(GL_MESH_OUTPUT_TYPE_NV, &meshOutputType);

    printf("GL_MESH_VERTICES_OUT_NV : %d\n", meshVerticesOut);
    printf("GL_MESH_PRIMITIVES_OUT_NV : %d\n", meshPrimtivesOut);
    printf("GL_MESH_OUTPUT_TYPE_NV : %d (%s)\n", meshOutputType, (meshOutputType == 0) ? "POINTS" : (meshOutputType == 1) ? "LINES" : (meshOutputType == 2) ? "TRIANGLES" : "UNDEFINED");

    std::cout << "----------------------------------" << std::endl;
    std::cout << "General Mesh Shader info:" << std::endl;
    std::cout << "----------------------------------" << std::endl;
    GLint maxDrawMeshTaskCount;
    GLint maxMeshWorkGroupSizeX, maxMeshWorkGroupSizeY, maxMeshWorkGroupSizeZ;
    GLint maxMeshOutputVertices, maxMeshOutputPrimitives, maxTaskOutputCount;
    GLint maxMeshWorkGroupInvocations;
    GLint64 maxShaderStorageBlockSize;

    glGetIntegerv(GL_MAX_DRAW_MESH_TASKS_COUNT_NV, &maxDrawMeshTaskCount);
    glGetIntegeri_v(GL_MAX_MESH_WORK_GROUP_SIZE_NV, 0, &maxMeshWorkGroupSizeX);
    glGetIntegeri_v(GL_MAX_MESH_WORK_GROUP_SIZE_NV, 1, &maxMeshWorkGroupSizeY);
    glGetIntegeri_v(GL_MAX_MESH_WORK_GROUP_SIZE_NV, 2, &maxMeshWorkGroupSizeZ);
    glGetIntegerv(GL_MAX_MESH_OUTPUT_VERTICES_NV, &maxMeshOutputVertices);
    glGetIntegerv(GL_MAX_MESH_OUTPUT_PRIMITIVES_NV, &maxMeshOutputPrimitives);
    glGetIntegerv(GL_MAX_MESH_WORK_GROUP_INVOCATIONS_NV, &maxMeshWorkGroupInvocations);
    glGetInteger64v(GL_MAX_SHADER_STORAGE_BLOCK_SIZE, &maxShaderStorageBlockSize);

    printf("GL_MAX_DRAW_MESH_TASKS_COUNT_NV : %d\n", maxDrawMeshTaskCount);
    printf("GL_MAX_MESH_WORK_GROUP_SIZE_NV : %d x %d x %d\n", maxMeshWorkGroupSizeX, maxMeshWorkGroupSizeY, maxMeshWorkGroupSizeZ);
    printf("GL_MAX_MESH_OUTPUT_VERTICES_NV : %d\n", maxMeshOutputVertices);
    printf("GL_MAX_MESH_OUTPUT_PRIMITIVES_NV : %d\n", maxMeshOutputPrimitives);
    printf("GL_MAX_MESH_WORK_GROUP_INVOCATIONS_NV : %d\n", maxMeshWorkGroupInvocations);
    printf("GL_MAX_SHADER_STORAGE_BLOCK_SIZE : %d\n", maxShaderStorageBlockSize);
    std::cout << "----------------------------------" << std::endl << std::endl;

    std::cout << "GL_GET_ERROR : " << glGetError() << std::endl;
}

void MeshShaderMaterial::render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) {
    _shader->use();

    glUniformMatrix4fv(_shader->getUniformLocation("projectionMatrix"), 1, GL_FALSE, glm::value_ptr(pProjectionMatrix));
    glUniformMatrix4fv(_shader->getUniformLocation("viewMatrix"), 1, GL_FALSE, glm::value_ptr(pViewMatrix));
    glUniformMatrix4fv(_shader->getUniformLocation("modelMatrix"), 1, GL_FALSE, glm::value_ptr(pModelMatrix));

    _shader->glDrawMeshTasksNV(0, 3907); // 1m points -- 256 vertices per workgroup
    // _shader->glDrawMeshTasksNV(0, 19532); // 5m points -- 256 vertices per workgroup
    // _shader->glDrawMeshTasksNV(0, 39063);  // 10m points -- 256 vertices per workgroup
    // _shader->glDrawMeshTasksNV(0, 58594); // 15m points -- 256 vertices per workgroup 
    // _shader->glDrawMeshTasksNV(0, 78.125); // 20m points -- 256 vertices per workgroup
}

MeshShaderMaterial::~MeshShaderMaterial() {
    // dtor
}
