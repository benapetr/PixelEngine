//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "peopenglwidget.h"
#include "qopenglrenderer.h"
#include "../world.h"
#include <QDateTime>
#include <algorithm>

using namespace PE;

PEOpenGLWidget::PEOpenGLWidget(QWidget *parent, World *world) : QOpenGLWidget(parent)
{
    this->fpsStart = QDateTime::currentDateTime().toMSecsSinceEpoch();
    this->world = world;
    this->setAutoFillBackground(false);
}

PEOpenGLWidget::~PEOpenGLWidget()
{
    this->makeCurrent();
    delete this->renderer;
    this->renderer = nullptr;
    this->doneCurrent();
}

World *PEOpenGLWidget::GetWorld() const
{
    return this->world;
}

void PEOpenGLWidget::SetWorld(World *world)
{
    this->world = world;
}

float PEOpenGLWidget::GetFPS() const
{
    return this->lastFPS;
}

RendererStats PEOpenGLWidget::GetRendererStats() const
{
    if (!this->renderer)
        return RendererStats();
    return this->renderer->GetStats();
}

void PEOpenGLWidget::initializeGL()
{
    this->recreateRenderer(this->width(), this->height());
}

void PEOpenGLWidget::paintGL()
{
    if (!this->world || !this->renderer)
        return;

    qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if (currentTime - this->fpsStart > this->fpsSamplingRate)
    {
        this->lastFPS = static_cast<float>(this->currentFPS) / (this->fpsSamplingRate / 1000);
        this->currentFPS = 0;
        this->fpsStart = currentTime;
    }
    this->currentFPS++;

    this->renderer->SetContext(this->context());
    this->renderer->Begin();
    this->world->Render(this->renderer);
    this->renderer->End();
    this->renderer->HasUpdate = false;
}

void PEOpenGLWidget::resizeGL(int width, int height)
{
    this->recreateRenderer(width, height);
}

void PEOpenGLWidget::recreateRenderer(int width, int height)
{
    delete this->renderer;
    this->renderer = new QOpenGLRenderer(std::max(1, width), std::max(1, height), this, this->context());
}
