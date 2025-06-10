//
// Created by clcc777 on 25-6-11.
//

#pragma once
#include "TransformC.h"

class SceneHierarchy {
private:
    TransformC *m_selected = nullptr;
    std::vector<TransformC *> m_roots; // 存储场景中的根节点
public:
    void Draw();
    void DrawSceneNode(const TransformC *root, const TransformC *&selected);
};
