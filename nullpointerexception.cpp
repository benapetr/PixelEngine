// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "nullpointerexception.h"

using namespace PE;

NullPointerException::NullPointerException(const QString& what, const QString& where) : Exception (what, where)
{

}
