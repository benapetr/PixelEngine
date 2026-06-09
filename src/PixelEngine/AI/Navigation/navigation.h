//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef NAVIGATION_H
#define NAVIGATION_H

#include "../../definitions.h"
#include <QHash>
#include <QList>

namespace PE
{
    class PathNode;

    class Navigation
    {
        public:
            static Navigation *GetDefault();
            static void SetDebug(bool enabled);
            static bool IsDebug();

            void RegisterNode(PathNode *node);
            void UnregisterNode(PathNode *node);
            void Clear();
            void BuildPaths();
            void Rebuild();
            bool IsDirty() const;
            QList<PathNode*> GetNodes() const;
            QList<PathNode*> GetNeighbors(PathNode *node) const;

        private:
            bool CanConnect(PathNode *source, PathNode *target) const;

            QList<PathNode*> nodes;
            QHash<PathNode*, QList<PathNode*>> paths;
            bool dirty = false;
            static Navigation *defaultNavigation;
            static bool debug;
    };
}

#endif // NAVIGATION_H
