//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019

#ifndef CONSOLE_H
#define CONSOLE_H

#include <QDialog>
#include <QList>

namespace Ui {
    class Console;
}

class Console : public QDialog
{
        Q_OBJECT

    public:
        static void Append(QString text, bool debug = false);
        static qint64 LastMessageTime;
        static QString LastMessage;
        static QList<QString> Items;
        static Console *ActiveConsole;
        explicit Console(QWidget *parent = nullptr);
        ~Console();

    private:
        Ui::Console *ui;
};

#endif // CONSOLE_H
