//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "peimagewindow.h"
#include "qimagerenderer.h"
#include "../world.h"
#include <QDateTime>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <algorithm>

using namespace PE;

PEImageWindow::PEImageWindow(QWidget *parent) : QWidget(parent)
{
    this->fps_start = QDateTime::currentDateTime().toMSecsSinceEpoch();
    this->setAutoFillBackground(false);
}

PEImageWindow::~PEImageWindow()
{
    delete this->renderer;
}

World *PEImageWindow::GetWorld()
{
    return this->world;
}

void PEImageWindow::SetWorld(World *w)
{
    this->world = w;
}

float PEImageWindow::GetFPS()
{
    return this->lastFPS;
}

RendererStats PEImageWindow::GetRendererStats() const
{
    if (!this->renderer)
        return RendererStats();
    return this->renderer->GetStats();
}

void PEImageWindow::setTitle(const QString &title)
{
    this->setWindowTitle(title);
}

void PEImageWindow::initializeRenderer()
{
    delete this->renderer;
    this->renderer = new QImageRenderer(std::max(1, this->width()), std::max(1, this->height()));
}

void PEImageWindow::paintEvent(QPaintEvent *event)
{
    (void)event;

    if (!this->world)
        return;

    if (!this->renderer)
        this->initializeRenderer();

    qint64 current_time = QDateTime::currentDateTime().toMSecsSinceEpoch();
    if (current_time - this->fps_start > this->fps_sampling_rate)
    {
        this->lastFPS = static_cast<float>(this->currentFPS) / (this->fps_sampling_rate / 1000);
        this->currentFPS = 0;
        this->fps_start = current_time;
    }
    this->currentFPS++;

    this->world->ForceRender(this->renderer);

    QPainter painter(this);
    painter.drawPixmap(0, 0, this->renderer->GetPixmap());
    this->renderer->HasUpdate = false;
}

void PEImageWindow::resizeEvent(QResizeEvent *event)
{
    (void)event;
    this->initializeRenderer();
}
