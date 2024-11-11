//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef PEGLWINDOW_H
#define PEGLWINDOW_H

#include <QOpenGLWindow>

namespace PE
{
    class QGLRenderer;
    class World;
    /*!
     * \brief The PEGLWindow class is a helper class for QGLRenderer - it automatically instantiates renderer for you
     *
     * You can use this widget anywhere in your Qt application to create a window that is PixelEngine view port
     */
    class PEGLWindow : public QOpenGLWindow
    {
        public:
            PEGLWindow();
            ~PEGLWindow();
            World *GetWorld();
            void SetWorld(World *w);
            float GetFPS();

        protected:
            void initializeRenderer();
            void paintGL() override;
            float fps_sampling_rate = 2000;
            qint64 fps_start;
            int currentFPS = 0;
            float lastFPS = 0;
            QGLRenderer *renderer;
            World *world;
    };
}

#endif // PEGLWINDOW_H
