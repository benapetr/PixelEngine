// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#include "exception.h"

using namespace PE;

Exception::Exception(const QString &what, const QString &where)
{
    this->Where = where;
    this->What = what;
}

PE::Exception::~Exception()
{

}
