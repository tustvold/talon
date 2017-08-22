#pragma once
#include <Scene.hpp>
#include "TalonConfig.hpp"

class TestScene : public TALON_NS::Scene {
public:
    TestScene();
    ~TestScene() final;

private:
    std::unique_ptr<TALON_NS::Mesh> testMesh;
    std::unique_ptr<TALON_NS::Material> testMaterial;
};



