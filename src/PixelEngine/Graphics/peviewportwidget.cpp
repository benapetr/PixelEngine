//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#include "peviewportwidget.h"
#include "peglwidget.h"
#include "peopenglwidget.h"
#include "qimagerenderer.h"
#include "../world.h"
#include <QDateTime>
#include <QOpenGLContext>
#include <QPainter>
#include <QPaintEvent>
#include <QResizeEvent>
#include <QSizePolicy>
#include <QVBoxLayout>
#include <algorithm>

using namespace PE;

namespace
{
    class PEImageViewportWidget : public QWidget
    {
        public:
            PEImageViewportWidget(QWidget *parent, World *world) : QWidget(parent)
            {
                this->fpsStart = QDateTime::currentDateTime().toMSecsSinceEpoch();
                this->world = world;
                this->setAutoFillBackground(false);
                this->recreateRenderer();
            }

            ~PEImageViewportWidget() override
            {
                delete this->renderer;
            }

            void SetWorld(World *world)
            {
                this->world = world;
            }

            float GetFPS() const
            {
                return this->lastFPS;
            }

            RendererStats GetRendererStats() const
            {
                if (!this->renderer)
                    return RendererStats();
                return this->renderer->GetStats();
            }

        protected:
            void paintEvent(QPaintEvent *event) override
            {
                (void)event;

                if (!this->renderer || !this->world)
                    return;

                qint64 currentTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
                if (currentTime - this->fpsStart > this->fpsSamplingRate)
                {
                    this->lastFPS = static_cast<float>(this->currentFPS) / (this->fpsSamplingRate / 1000);
                    this->currentFPS = 0;
                    this->fpsStart = currentTime;
                }
                this->currentFPS++;

                this->world->Render(this->renderer);

                QPainter painter(this);
                painter.drawPixmap(0, 0, this->renderer->GetPixmap());
            }

            void resizeEvent(QResizeEvent *event) override
            {
                (void)event;
                this->recreateRenderer();
            }

        private:
            void recreateRenderer()
            {
                delete this->renderer;
                this->renderer = new QImageRenderer(std::max(1, this->width()), std::max(1, this->height()));
            }

            QImageRenderer *renderer = nullptr;
            World *world = nullptr;
            float fpsSamplingRate = 2000;
            qint64 fpsStart = 0;
            int currentFPS = 0;
            float lastFPS = 0;
    };
}

PEViewportWidget::PEViewportWidget(RendererBackend backend, QWidget *parent, World *world) : QWidget(parent)
{
    this->world = world;
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(0);
    this->SetBackend(backend);
}

PEViewportWidget::~PEViewportWidget()
{

}

RendererBackend PEViewportWidget::GetBackend() const
{
    return this->activeBackend;
}

RendererBackend PEViewportWidget::GetRequestedBackend() const
{
    return this->requestedBackend;
}

bool PEViewportWidget::SetBackend(RendererBackend backend)
{
    this->requestedBackend = backend;
    RendererBackend nextBackend = backend;

    if (!PEViewportWidget::IsBackendAvailable(nextBackend))
        nextBackend = RendererBackend_QImage;

    if (this->viewport && this->activeBackend == nextBackend)
        return this->requestedBackend == this->activeBackend;

    this->activeBackend = nextBackend;
    this->recreateViewport();
    return this->requestedBackend == this->activeBackend;
}

World *PEViewportWidget::GetWorld() const
{
    return this->world;
}

void PEViewportWidget::SetWorld(World *world)
{
    this->world = world;

    if (PEImageViewportWidget *imageViewport = dynamic_cast<PEImageViewportWidget*>(this->viewport))
        imageViewport->SetWorld(world);
    else if (PEGLWidget *glViewport = dynamic_cast<PEGLWidget*>(this->viewport))
        glViewport->SetWorld(world);
    else if (PEOpenGLWidget *openGLViewport = dynamic_cast<PEOpenGLWidget*>(this->viewport))
        openGLViewport->SetWorld(world);
}

void PEViewportWidget::Advance()
{
    if (this->world)
        this->world->Update();
    this->RequestRender();
}

void PEViewportWidget::RequestRender()
{
    if (this->viewport)
        this->viewport->update();
}

float PEViewportWidget::GetFPS() const
{
    if (PEImageViewportWidget *imageViewport = dynamic_cast<PEImageViewportWidget*>(this->viewport))
        return imageViewport->GetFPS();
    if (PEGLWidget *glViewport = dynamic_cast<PEGLWidget*>(this->viewport))
        return glViewport->GetFPS();
    if (PEOpenGLWidget *openGLViewport = dynamic_cast<PEOpenGLWidget*>(this->viewport))
        return openGLViewport->GetFPS();
    return 0;
}

RendererStats PEViewportWidget::GetRendererStats() const
{
    if (PEImageViewportWidget *imageViewport = dynamic_cast<PEImageViewportWidget*>(this->viewport))
        return imageViewport->GetRendererStats();
    if (PEOpenGLWidget *openGLViewport = dynamic_cast<PEOpenGLWidget*>(this->viewport))
        return openGLViewport->GetRendererStats();
    return RendererStats();
}

bool PEViewportWidget::IsBackendAvailable(RendererBackend backend)
{
    if (backend == RendererBackend_QImage)
        return true;

    QOpenGLContext context;
    return context.create();
}

void PEViewportWidget::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);

    if (this->viewport && (this->activeBackend == RendererBackend_QPainterOpenGL || this->activeBackend == RendererBackend_OpenGL))
        this->recreateViewport();
}

void PEViewportWidget::recreateViewport()
{
    delete this->viewport;
    this->viewport = nullptr;

    if (this->activeBackend == RendererBackend_OpenGL)
        this->viewport = new PEOpenGLWidget(this, this->world);
    else if (this->activeBackend == RendererBackend_QPainterOpenGL)
        this->viewport = new PEGLWidget(this, this->world);
    else
        this->viewport = new PEImageViewportWidget(this, this->world);

    this->viewport->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    this->layout()->addWidget(this->viewport);
}
