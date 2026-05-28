//! \file window.cpp
//! \brief Implementation of UI window.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "window.h"
#include "../camera.h"
#include "../Graphics/renderer.h"
#include "../Serialization/deserializer.h"
#include "../Serialization/serializer.h"

using namespace PE;

Window::Window()
{
    this->Width = 320;
    this->Height = 220;
}

Window::Window(const QString &title, const Vector &position, pe_float_t width, pe_float_t height, Object *parent) : UIElement(position, width, height, parent)
{
    this->Title = title;
}

QString Window::GetClassName() const
{
    return "PE::Window";
}

void Window::Render(Renderer *r, Camera *c)
{
    if (!this->Visible)
        return;

    Vector position = c->ProjectedPosition(this->Position);
    r->DrawRect(position.X2int(), position.Y2int(), static_cast<int>(this->Width), static_cast<int>(this->Height), 1, this->BackgroundColor, true);
    r->DrawRect(position.X2int(), position.Y2int(), static_cast<int>(this->Width), this->TitleBarHeight, 1, this->TitleBarColor, true);
    r->DrawRect(position.X2int(), position.Y2int(), static_cast<int>(this->Width), static_cast<int>(this->Height), 1, this->BorderColor);
    r->DrawText(position.X2int() + 8, position.Y2int() + this->FontSize + 5, this->Title, this->TextColor, this->FontSize);

    foreach (UIElement *control, this->controls)
        control->Render(r, c);
}

void Window::AddControl(UIElement *control)
{
    this->controls.append(control);
    this->AddChildren(control);
    this->RedrawNeeded = true;
}

void Window::RemoveControl(UIElement *control)
{
    this->controls.removeAll(control);
    this->RemoveChildren(control);
    if (this->focusedControl == control)
        this->focusedControl = nullptr;
    this->RedrawNeeded = true;
}

UIElement *Window::GetFocusedControl() const
{
    return this->focusedControl.GetPtr();
}

void Window::MousePress(const Vector &point)
{
    UIElement::MousePress(point);
    this->focusedControl = nullptr;

    for (int i = this->controls.size() - 1; i >= 0; --i)
    {
        UIElement *control = this->controls[i];
        if (!control->Visible || !control->Enabled || !control->ContainsPoint(point))
        {
            control->Focused = false;
            continue;
        }

        control->MousePress(point);
        this->focusedControl = control;
        break;
    }
    this->RedrawNeeded = true;
}

void Window::MouseRelease(const Vector &point)
{
    foreach (UIElement *control, this->controls)
        control->MouseRelease(point);
    this->RedrawNeeded = true;
}

void Window::MouseMove(const Vector &point)
{
    foreach (UIElement *control, this->controls)
    {
        control->MouseMove(point);
        control->RedrawNeeded = true;
    }
    this->RedrawNeeded = true;
}

void Window::KeyPress(int key, const QString &text)
{
    if (this->focusedControl != nullptr)
        this->focusedControl->KeyPress(key, text);
}

void Window::Serialize(Serializer *serializer) const
{
    UIElement::Serialize(serializer);
    serializer->WriteString("title", this->Title);
    serializer->WriteInteger("titleBarHeight", this->TitleBarHeight);
    serializer->WriteInteger("fontSize", this->FontSize);
}

void Window::Deserialize(Deserializer *deserializer)
{
    UIElement::Deserialize(deserializer);
    this->Title = deserializer->ReadString("title", this->Title);
    this->TitleBarHeight = deserializer->ReadInteger("titleBarHeight", this->TitleBarHeight);
    this->FontSize = deserializer->ReadInteger("fontSize", this->FontSize);
}
