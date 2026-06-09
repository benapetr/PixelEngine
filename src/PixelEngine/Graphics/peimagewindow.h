//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef PEIMAGEWINDOW_H
#define PEIMAGEWINDOW_H

#include "renderer.h"
#include <QWidget>

class QPaintEvent;
class QResizeEvent;

namespace PE
{
    class QImageRenderer;
    class World;

    /*!
     * \brief QWidget-backed PixelEngine viewport using QImageRenderer.
     *
     * This is the basic/native renderer companion to PEGLWindow and
     * PEOpenGLWindow. It intentionally exposes the same common world/FPS
     * accessors so examples can switch renderer windows easily.
     */
    class PEImageWindow : public QWidget
    {
        public:
            explicit PEImageWindow(QWidget *parent = nullptr);
            ~PEImageWindow() override;
            World *GetWorld();
            void SetWorld(World *w);
            float GetFPS();
            RendererStats GetRendererStats() const;
            QImageRenderer *GetRenderer() { return this->renderer; }
            void setTitle(const QString &title);

        protected:
            void initializeRenderer();
            void paintEvent(QPaintEvent *event) override;
            void resizeEvent(QResizeEvent *event) override;

            float fps_sampling_rate = 2000;
            qint64 fps_start = 0;
            int currentFPS = 0;
            float lastFPS = 0;
            QImageRenderer *renderer = nullptr;
            World *world = nullptr;
    };
}

#endif // PEIMAGEWINDOW_H
