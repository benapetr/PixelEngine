//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "game.h"
#include "console.h"
#include <PixelEngine/Graphics/peglwidget.h>
#include <QKeyEvent>
#include <QImage>
#include <QDesktopWidget>
#include <PixelEngine/engine.h>
#include <PixelEngine/world.h>
#include <PixelEngine/ringlog.h>
#include <PixelEngine/ringlog_item.h>
#include <PixelEngine/Graphics/qglrenderer.h>

MainWindow *MainWindow::Main = nullptr;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow)
{
    Main = this;
    PE::Engine::Initialize(false);
    this->ui->setupUi(this);
    this->showMaximized();
    this->viewPort = new PE::PEGLWidget(this, nullptr);
    this->layout()->addWidget(this->viewPort);
    this->se_renderer = new PE::QGLRenderer(this->GetWidth(), this->GetHeight(), this->viewPort);
    this->game = new Game(this->GetWidth(), this->GetHeight(), this->se_renderer);
    this->viewPort->SetWorld(this->game->GetWorld());
    this->renderTimer = new QTimer(this);
    connect(this->renderTimer, SIGNAL(timeout()), this, SLOT(OnRender()));
    // this timer speed defines FPS, smaller means higher FPS, but also more CPU usage
#ifdef __EMSCRIPTEN__
    // WASM is significantly worse than native, so let's decrease FPS to put less strain on CPU
    this->renderTimer->start(40);
#else
    this->renderTimer->start(10);
#endif

    // Init console
    foreach (PE::RingLog_Item item, PE::Engine::GetEngine()->RL->GetItems())
    {
        Console::Append(item.GetText());
    }

    this->ui->actionFast_game->setChecked(Game::SuperFast);

    new Console(this);
#ifdef __EMSCRIPTEN__
    setWindowFlags(Qt::FramelessWindowHint| Qt::WindowSystemMenuHint);
#endif
}

MainWindow::~MainWindow()
{
    delete this->game;
    delete this->se_renderer;
    delete ui;
}

void MainWindow::Render()
{
    this->se_renderer->Begin();
    this->game->GetWorld()->Render(this->se_renderer);
    this->se_renderer->End();
    this->se_renderer->HasUpdate = false;
}

int MainWindow::GetWidth()
{
    return this->viewPort->width();
}

int MainWindow::GetHeight()
{
    return this->viewPort->height();
}

void MainWindow::InstallWorld(PE::World *w)
{
    this->viewPort->SetWorld(w);
}

void MainWindow::OnRender()
{
    if (!this->ui->actionRendering->isChecked())
        return;
    this->viewPort->update();
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    this->game->GetWorld()->ProcessKeyPress(e->key());
}

void MainWindow::keyReleaseEvent(QKeyEvent *e)
{
    this->game->GetWorld()->ProcessKeyRelease(e->key());
}

void MainWindow::on_actionRendering_triggered()
{
    this->se_renderer->Enabled = !this->se_renderer->Enabled;
}

void MainWindow::on_actionNew_game_triggered()
{
    Game::CurrentGame->startGame();
}

void MainWindow::on_actionFast_game_triggered()
{
    Game::SuperFast = !Game::SuperFast;
}

void MainWindow::on_actionShow_console_triggered()
{
    Console::ActiveConsole->show();
}

void MainWindow::on_actionFluid_terrain_triggered()
{
    Game::FastTerrainUpdates = !Game::FastTerrainUpdates;
}

void MainWindow::on_actionLow_FPS_triggered(bool checked)
{
    this->renderTimer->stop();
    if (!checked)
        this->renderTimer->start(20);
    else
        this->renderTimer->start(40);
}

void MainWindow::on_actionTest_2_triggered()
{
    Game::CurrentGame->test2();
}
