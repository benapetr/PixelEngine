//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "collectable_smartptr.h"
#include "../exception.h"
#include "../object.h"
#include "../Physics/collider.h"
#include "../actor.h"
#include "../terrain.h"
#include "gc.h"

// Required by MSVC
namespace PE
{
    template class Collectable_SmartPtr<Object>;
    template class Collectable_SmartPtr<Actor>;
    template class Collectable_SmartPtr<Collider>;
    template class Collectable_SmartPtr<Terrain>;
}
