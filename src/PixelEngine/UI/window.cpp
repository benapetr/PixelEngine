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
    int titleBarY = position.Y2int() + static_cast<int>(this->Height) - this->TitleBarHeight;
    int titleTextY = titleBarY + ((this->TitleBarHeight - this->FontSize) / 2);
    this->DrawBox(r, position.X2int(), position.Y2int(), static_cast<int>(this->Width), static_cast<int>(this->Height), 1, this->BackgroundColor, true);
    if (this->ShowTitleBar)
        r->DrawRect(position.X2int(), titleBarY, static_cast<int>(this->Width), this->TitleBarHeight, 1, this->TitleBarColor, true);
    this->DrawBox(r, position.X2int(), position.Y2int(), static_cast<int>(this->Width), static_cast<int>(this->Height), 1, this->BorderColor);
    if (this->ShowTitleBar)
    {
        r->DrawText(position.X2int() + 8, titleTextY, this->Title, this->TextColor, this->FontSize);
        if (this->IsCloseButtonVisible())
        {
            int padding = (this->TitleBarHeight - this->CloseButtonSize) / 2;
            int x = position.X2int() + static_cast<int>(this->Width) - this->CloseButtonSize - padding;
            int y = titleBarY + padding;
            QColor closeColor = this->CloseButtonHovered ? this->CloseButtonHoverColor : this->CloseButtonColor;
            this->DrawBox(r, x, y, this->CloseButtonSize, this->CloseButtonSize, 1, closeColor, true);
            r->DrawText(x + 5, y + ((this->CloseButtonSize - this->FontSize) / 2), "x", this->CloseButtonTextColor, this->FontSize);
        }
    }

    foreach (UIElement *control, this->controls)
        control->Render(r, c);
}

void Window::AddControl(UIElement *control)
{
    if (control == nullptr)
        return;

    if (control->GetUIParent() == this && this->IndexOfControl(control) >= 0)
        return;

    if (control->GetUIParent() != nullptr)
        control->GetUIParent()->RemoveControl(control);

    control->RelativePosition = control->Position;
    control->SetPosition(Vector(this->Position.X + control->RelativePosition.X,
                                this->Position.Y + this->Height - control->RelativePosition.Y - control->Height));
    control->SetUIParent(this);
    this->controls.append(control);
    this->AddChildren(control);
    this->RedrawNeeded = true;
}

void Window::RemoveControl(UIElement *control)
{
    if (control == nullptr)
        return;

    int index = this->IndexOfControl(control);
    if (index < 0)
        return;

    this->controls.removeAt(index);
    this->RemoveChildren(control);
    control->SetUIParent(nullptr);
    if (this->focusedControl == control)
        this->focusedControl = nullptr;
    this->RedrawNeeded = true;
}

void Window::BringToFront(UIElement *control)
{
    int index = this->IndexOfControl(control);
    if (index < 0 || index == this->controls.size() - 1)
        return;

    Collectable_SmartPtr<UIElement> item = this->controls[index];
    this->controls.removeAt(index);
    this->controls.append(item);
    this->RedrawNeeded = true;
}

void Window::SendToBack(UIElement *control)
{
    int index = this->IndexOfControl(control);
    if (index <= 0)
        return;

    Collectable_SmartPtr<UIElement> item = this->controls[index];
    this->controls.removeAt(index);
    this->controls.prepend(item);
    this->RedrawNeeded = true;
}

void Window::MoveForward(UIElement *control)
{
    int index = this->IndexOfControl(control);
    if (index < 0 || index == this->controls.size() - 1)
        return;

    this->controls.swapItemsAt(index, index + 1);
    this->RedrawNeeded = true;
}

void Window::MoveBackward(UIElement *control)
{
    int index = this->IndexOfControl(control);
    if (index <= 0)
        return;

    this->controls.swapItemsAt(index, index - 1);
    this->RedrawNeeded = true;
}

UIElement *Window::GetFocusedControl() const
{
    return this->focusedControl.GetPtr();
}

bool Window::IsCloseButtonVisible() const
{
    return this->ShowTitleBar && this->ShowCloseButton && this->CloseButtonSize > 0;
}

bool Window::IsPointOnCloseButton(const Vector &point) const
{
    if (!this->IsCloseButtonVisible())
        return false;

    pe_float_t padding = (this->TitleBarHeight - this->CloseButtonSize) / 2;
    pe_float_t x = this->Position.X + this->Width - this->CloseButtonSize - padding;
    pe_float_t y = this->Position.Y + this->Height - this->TitleBarHeight + padding;

    return point.X >= x &&
           point.Y >= y &&
           point.X <= x + this->CloseButtonSize &&
           point.Y <= y + this->CloseButtonSize;
}

void Window::MousePress(const Vector &point)
{
    UIElement::MousePress(point);
    this->focusedControl = nullptr;
    this->CloseButtonPressed = false;

    if (this->IsPointOnCloseButton(point))
    {
        foreach (UIElement *control, this->controls)
            control->FocusLost();
        this->CloseButtonPressed = true;
        this->RedrawNeeded = true;
        return;
    }

    UIElement *hitControl = nullptr;
    for (int i = this->controls.size() - 1; i >= 0; --i)
    {
        UIElement *control = this->controls[i].GetPtr();
        if (!control->Visible || !control->Enabled || !control->ContainsPoint(point))
            continue;

        hitControl = control;
        break;
    }

    foreach (UIElement *control, this->controls)
    {
        if (control != hitControl)
            control->FocusLost();
    }

    if (hitControl != nullptr)
    {
        hitControl->MousePress(point);
        this->focusedControl = hitControl;
        if (hitControl->BringToFrontOnFocus)
            this->BringToFront(hitControl);
    }
    this->RedrawNeeded = true;
}

void Window::MouseRelease(const Vector &point)
{
    bool closeClicked = this->CloseButtonPressed && this->IsPointOnCloseButton(point);
    this->CloseButtonPressed = false;
    if (closeClicked)
    {
        this->RedrawNeeded = true;
        if (this->OnClose)
            this->OnClose();
        return;
    }

    foreach (UIElement *control, this->controls)
        control->MouseRelease(point);
    this->RedrawNeeded = true;
}

void Window::MouseMove(const Vector &point)
{
    bool closeHovered = this->IsPointOnCloseButton(point);
    if (this->CloseButtonHovered != closeHovered)
    {
        this->CloseButtonHovered = closeHovered;
        this->RedrawNeeded = true;
    }

    foreach (UIElement *control, this->controls)
    {
        control->UpdateHover(point);
        control->MouseMove(point);
    }
    this->RedrawNeeded = true;
}

bool Window::MouseWheel(const Vector &point, pe_float_t delta)
{
    if (!this->Visible || !this->Enabled || !this->ContainsPoint(point))
        return false;

    for (int i = this->controls.size() - 1; i >= 0; --i)
    {
        UIElement *control = this->controls[i].GetPtr();
        if (!control->Visible || !control->Enabled || !control->ContainsPoint(point))
            continue;

        if (control->MouseWheel(point, delta))
        {
            this->RedrawNeeded = true;
            return true;
        }
    }

    return false;
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
    serializer->WriteInteger("closeButtonSize", this->CloseButtonSize);
    serializer->WriteInteger("fontSize", this->FontSize);
    serializer->WriteBool("showTitleBar", this->ShowTitleBar);
    serializer->WriteBool("showCloseButton", this->ShowCloseButton);
}

void Window::Deserialize(Deserializer *deserializer)
{
    UIElement::Deserialize(deserializer);
    this->Title = deserializer->ReadString("title", this->Title);
    this->TitleBarHeight = deserializer->ReadInteger("titleBarHeight", this->TitleBarHeight);
    this->CloseButtonSize = deserializer->ReadInteger("closeButtonSize", this->CloseButtonSize);
    this->FontSize = deserializer->ReadInteger("fontSize", this->FontSize);
    this->ShowTitleBar = deserializer->ReadBool("showTitleBar", this->ShowTitleBar);
    this->ShowCloseButton = deserializer->ReadBool("showCloseButton", this->ShowCloseButton);
}

int Window::IndexOfControl(UIElement *control) const
{
    for (int i = 0; i < this->controls.size(); ++i)
    {
        if (this->controls[i] == control)
            return i;
    }
    return -1;
}
