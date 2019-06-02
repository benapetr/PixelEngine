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

#include <QMainWindow>
#include <QTimer>

namespace Ui
{
    class MainWindow;
}

class QImage;

namespace PE
{
    class QImageRenderer;
}

class Game;

class MainWindow : public QMainWindow
{
        Q_OBJECT

    public:
        explicit MainWindow(QWidget *parent = nullptr);
        ~MainWindow() override;
        void Render();
        int GetWidth();
        int GetHeight();

    public slots:
        void OnRender();
        void keyPressEvent(QKeyEvent* e) override;
        void keyReleaseEvent(QKeyEvent* e) override;

    private slots:
        void on_actionRendering_triggered();
        void on_actionNew_game_triggered();
        void on_actionFast_game_triggered();
        void on_actionShow_console_triggered();
        void on_actionFluid_terrain_triggered();
        void on_actionLow_FPS_triggered(bool checked);

        void on_actionTest_2_triggered();

    private:
        QTimer *renderTimer;
        Game *game;
        QImage *qimage;
        PE::QImageRenderer *se_renderer;
        Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
