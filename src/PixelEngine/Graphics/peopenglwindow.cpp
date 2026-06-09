//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "peopenglwindow.h"
#include "qopenglrenderer.h"
#include "../world.h"
#include <QDateTime>
#include <algorithm>

using namespace PE;

PEOpenGLWindow::PEOpenGLWindow() : QOpenGLWindow(QOpenGLWindow::NoPartialUpdate)
{
    this->fps_start = QDateTime::currentDateTime().toMSecsSinceEpoch();
}

PEOpenGLWindow::~PEOpenGLWindow()
{
    this->makeCurrent();
    delete this->renderer;
    this->renderer = nullptr;
    this->doneCurrent();
}

World *PEOpenGLWindow::GetWorld()
{
    return this->world;
}

void PEOpenGLWindow::SetWorld(World *w)
{
    this->world = w;
}

float PEOpenGLWindow::GetFPS()
{
    return this->lastFPS;
}

RendererStats PEOpenGLWindow::GetRendererStats() const
{
    if (!this->renderer)
        return RendererStats();
    return this->renderer->GetStats();
}

void PEOpenGLWindow::initializeRenderer()
{
    delete this->renderer;
    this->renderer = new QOpenGLRenderer(std::max(1, this->width()), std::max(1, this->height()), this, this->context());
}

void PEOpenGLWindow::initializeGL()
{
    this->initializeRenderer();
}

void PEOpenGLWindow::resizeGL(int width, int height)
{
    delete this->renderer;
    this->renderer = new QOpenGLRenderer(std::max(1, width), std::max(1, height), this, this->context());
}

void PEOpenGLWindow::paintGL()
{
    if (!this->world || !this->renderer)
        return;

    qint64 current_time = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if (current_time - this->fps_start > this->fps_sampling_rate)
    {
        this->lastFPS = static_cast<float>(this->currentFPS) / (this->fps_sampling_rate / 1000);
        this->currentFPS = 0;
        this->fps_start = current_time;
    }
    this->currentFPS++;

    this->renderer->SetContext(this->context());
    this->renderer->Begin();
    this->world->ForceRender(this->renderer);
    this->renderer->End();
    this->renderer->HasUpdate = false;
    this->requestUpdate();
}
