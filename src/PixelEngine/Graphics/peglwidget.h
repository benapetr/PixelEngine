//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef PEGLWIDGET_H
#define PEGLWIDGET_H

#include <QOpenGLWidget>

namespace PE
{
    class QGLRenderer;
    class World;
    class PEGLWidget : public QOpenGLWidget
    {
        public:
            PEGLWidget(QWidget *parent, World *w);
            World *GetWorld();
            void SetWorld(World *w);
            int GetFPS();

        protected:
            void paintEvent(QPaintEvent *event) override;
            int fps_sampling_rate = 2000;
            qint64 fps_start = 0;
            int currentFPS = 0;
            int lastFPS = 0;
            QGLRenderer *renderer;
            World *world;
    };
}

#endif // PEGLWIDGET_H
