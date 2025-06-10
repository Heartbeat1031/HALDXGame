//
// Created by clcc777 on 25-6-11.
//

#pragma once
#include "TransformC.h"

class SceneHierarchy {
private:
    TransformC *m_selected = nullptr;
public:
    void Draw();
    void DrawSceneNode(const TransformC *root, const TransformC *&selected);

};
