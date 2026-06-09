//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef PATHNODE_H
#define PATHNODE_H

#include "../../object.h"

namespace PE
{
    class PathNode : public Object
    {
        public:
            PathNode();
            PathNode(const Vector &position, Object *parent = nullptr);

            QString GetClassName() const override;
            void Serialize(Serializer *serializer) const override;
            void Deserialize(Deserializer *deserializer) override;
            void Render(Renderer *r, Camera *c) override;
            void Paint(Renderer *r, Camera *c);
            void PaintPaths(Renderer *r, Camera *c);
            void Event_RegisteredToWorld(World *world) override;
            void Event_UnregisteredFromWorld(World *world) override;
            void Event_Destroyed() override;
            World *GetWorld() const;

            pe_float_t MaxDistance = 200;
            bool RespectColliders = false;
            static bool Debug;

        private:
            World *world = nullptr;
    };
}

#endif // PATHNODE_H
