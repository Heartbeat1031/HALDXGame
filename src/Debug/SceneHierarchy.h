//
// Created by clcc777 on 25-6-11.
//

#pragma once
#include "SoAStorage.h"

class SceneHierarchy {
private:
     UID m_selectedid = -1;
    std::vector<UID> m_rootids;
    int m_rawindex = 0;
public:
    SceneHierarchy();
    ~SceneHierarchy() = default;
    void Begin();
    void AddRoot(UID rootid);
    void Draw();
    void DrawSceneNode(UID rootid, UID &selectedid);
};
