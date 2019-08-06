//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QOpenGLWindow>
#include <QTimer>
#include <PixelEngine/Graphics/peglwindow.h>

class QImage;

namespace PE
{
    class PEGLWidget;
    class QGLRenderer;
    class World;
}

class Game;

class MainWindow : public PE::PEGLWindow
{
        Q_OBJECT

    public:
        static MainWindow *Main;
        MainWindow();
        ~MainWindow() override;
        void Render();
        int GetWidth();
        int GetHeight();
        void InstallWorld(PE::World *w);

    public slots:
        void OnRender();
        void keyPressEvent(QKeyEvent* e) override;
        void keyReleaseEvent(QKeyEvent* e) override;

    private slots:
        void on_actionNew_game_triggered();
        void on_actionFast_game_triggered();
        void on_actionShow_console_triggered();
        void on_actionFluid_terrain_triggered();
        void on_actionLow_FPS_triggered(bool checked);
        void on_actionTest_2_triggered();

    private:
        PE::PEGLWidget *viewPort;
        QTimer *renderTimer;
        Game *game;
};

#endif // MAINWINDOW_H
