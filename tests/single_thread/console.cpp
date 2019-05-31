//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#include "console.h"
#include "ui_console.h"
#include <QDateTime>

QList<QString> Console::Items;
// Last message
QString Console::LastMessage = "";
qint64 Console::LastMessageTime = 0;
Console *Console::ActiveConsole = nullptr;

void Console::Append(QString text, bool debug)
{
    LastMessageTime = QDateTime::currentDateTime().toMSecsSinceEpoch();
    Console::LastMessage = text;
    Console::Items.append(text);
    if (Console::ActiveConsole != nullptr)
    {
        Console::ActiveConsole->ui->plainTextEdit->appendPlainText(text + "\n");
    }
}

Console::Console(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Console)
{
    ui->setupUi(this);
    Console::ActiveConsole = this;
    foreach (QString line, Items)
        this->ui->plainTextEdit->appendPlainText(line + "\n");
}

Console::~Console()
{
    Console::ActiveConsole = nullptr;
    delete ui;
}
