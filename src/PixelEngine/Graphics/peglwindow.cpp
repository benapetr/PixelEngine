//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "qglrenderer.h"
#include "../world.h"
#include <QDateTime>
#include "peglwindow.h"

using namespace PE;

PEGLWindow::PEGLWindow() : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate)
{
    this->fps_start = QDateTime::currentDateTime().toMSecsSinceEpoch();
    this->renderer = nullptr;
    this->world = nullptr;
}

PEGLWindow::~PEGLWindow()
{
    delete this->renderer;
}

World *PEGLWindow::GetWorld()
{
    return this->world;
}

void PEGLWindow::SetWorld(World *w)
{
    this->world = w;
}

double PEGLWindow::GetFPS()
{
    return this->lastFPS;
}

void PEGLWindow::initializeRenderer()
{
    this->renderer = new QGLRenderer(this->width(), this->height(), this);
}

void PEGLWindow::paintGL()
{
    if (this->world == nullptr)
        return;

    // FPS counter
    qint64 current_time = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if (current_time - this->fps_start > this->fps_sampling_rate)
    {
        this->lastFPS = static_cast<double>(this->currentFPS) / (this->fps_sampling_rate / 1000);
        this->currentFPS = 0;
        this->fps_start = current_time;
    }
    this->currentFPS++;

    this->renderer->Begin();
    this->world->ForceRender(this->renderer);
    this->renderer->End();
    this->renderer->HasUpdate = false;
    this->requestUpdate();
}

