//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef PEOPENGLWINDOW_H
#define PEOPENGLWINDOW_H

#include "renderer.h"
#include <QOpenGLWindow>

namespace PE
{
    class QOpenGLRenderer;
    class World;

    /*!
     * \brief QOpenGLWindow-backed viewport using the real PixelEngine OpenGL renderer.
     */
    class PEOpenGLWindow : public QOpenGLWindow
    {
        public:
            PEOpenGLWindow();
            ~PEOpenGLWindow() override;
            World *GetWorld();
            void SetWorld(World *w);
            float GetFPS();
            RendererStats GetRendererStats() const;
            QOpenGLRenderer *GetRenderer() { return this->renderer; }

        protected:
            void initializeRenderer();
            void initializeGL() override;
            void resizeGL(int width, int height) override;
            void paintGL() override;

            float fps_sampling_rate = 2000;
            qint64 fps_start = 0;
            int currentFPS = 0;
            float lastFPS = 0;
            QOpenGLRenderer *renderer = nullptr;
            World *world = nullptr;
    };
}

#endif // PEOPENGLWINDOW_H
