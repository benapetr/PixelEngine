// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "collectable_smartptr.h"
#include "../exception.h"
#include "../object.h"
#include "../collider.h"
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
