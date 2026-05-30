//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef PEVIEWPORTWIDGET_H
#define PEVIEWPORTWIDGET_H

#include "renderer.h"
#include <QWidget>

class QResizeEvent;

namespace PE
{
    class World;

    /*!
     * \brief Renderer-agnostic PixelEngine viewport container.
     *
     * This widget owns the presentation widget for the selected renderer backend,
     * but it does not own the World pointer passed to it.
     */
    class PEViewportWidget : public QWidget
    {
        public:
            PEViewportWidget(RendererBackend backend, QWidget *parent = nullptr, World *world = nullptr);
            ~PEViewportWidget() override;
            RendererBackend GetBackend() const;
            RendererBackend GetRequestedBackend() const;
            bool SetBackend(RendererBackend backend);
            World *GetWorld() const;
            void SetWorld(World *world);
            void Advance();
            void RequestRender();
            float GetFPS() const;
            RendererStats GetRendererStats() const;
            static bool IsBackendAvailable(RendererBackend backend);

        protected:
            void resizeEvent(QResizeEvent *event) override;

        private:
            void recreateViewport();

            RendererBackend requestedBackend = RendererBackend_QImage;
            RendererBackend activeBackend = RendererBackend_QImage;
            World *world = nullptr;
            QWidget *viewport = nullptr;
    };
}

#endif // PEVIEWPORTWIDGET_H
