#ifndef MESHSHADERMATERIAL_HPP
#define MESHSHADERMATERIAL_HPP

#include "GL/glew.h"
#include "mge/materials/AbstractMaterial.hpp"
#include "mge/util/happly.h"

class ShaderProgram;

class MeshShaderMaterial : public AbstractMaterial {
    public:
        MeshShaderMaterial();
        virtual void render(World* pWorld, Mesh* pMesh, const glm::mat4& pModelMatrix, const glm::mat4& pViewMatrix, const glm::mat4& pProjectionMatrix) override;
        virtual ~MeshShaderMaterial();

    private:
        //all the static properties are shared between instances of ColorMaterial
        //note that they are all PRIVATE, we do not expose this static info to the outside world
        static ShaderProgram* _shader;
        static void _lazyInitializeShader();
        static void _initializeSSBOs();
        static void _debug();
};

#endif // MESHSHADERMATERIAL_HPP