//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2026

#ifndef PEOPENGLWIDGET_H
#define PEOPENGLWIDGET_H

#include <QOpenGLWidget>

namespace PE
{
    class QOpenGLRenderer;
    class World;

    class PEOpenGLWidget : public QOpenGLWidget
    {
        public:
            PEOpenGLWidget(QWidget *parent, World *world);
            ~PEOpenGLWidget() override;
            World *GetWorld() const;
            void SetWorld(World *world);
            float GetFPS() const;

        protected:
            void initializeGL() override;
            void paintGL() override;
            void resizeGL(int width, int height) override;

        private:
            void recreateRenderer(int width, int height);

            float fpsSamplingRate = 2000;
            qint64 fpsStart = 0;
            int currentFPS = 0;
            float lastFPS = 0;
            QOpenGLRenderer *renderer = nullptr;
            World *world = nullptr;
    };
}

#endif // PEOPENGLWIDGET_H
