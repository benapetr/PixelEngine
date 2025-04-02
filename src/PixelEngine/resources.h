//! \file resources.h
//! \brief Header file for the Resources class, managing game assets like pixmaps and text.

//This program is free software: you can redistribute it and/or modify
//it under the terms of the GNU Lesser General Public License as published by
//the Free Software Foundation, either version 3 of the License, or
//(at your option) any later version.

//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU Lesser General Public License for more details.

// Copyright (c) Petr Bena 2019 - 2025

#ifndef RESOURCES_H
#define RESOURCES_H

#include <QPixmap>
#include <QHash>

namespace PE
{
    //! \class Resources
    //! \brief Manages game assets such as pixmaps and text resources.
    class Resources
    {
        public:
            //! \brief Retrieve a pixmap by its name.
            //! \param name The name of the pixmap.
            //! \return A constant reference to the QPixmap.
            static const QPixmap &GetPixmap(const QString &name);

            //! \brief Retrieve a text resource by its name.
            //! \param name The name of the text resource.
            //! \return A QString containing the text resource.
            static const QString GetText(const QString &name);

            //! \brief Clear the resource cache.
            static void ClearCache();

            //! \brief Get the total size of all resources in the cache.
            //! \return The size of the resources in bytes.
            static qint64 GetSize();

        private:
            static QHash<QString, QPixmap> pixmaps; //!< Cache of pixmaps.
            static qint64 resourcesSize; //!< Total size of cached resources.
    };
}

#endif // RESOURCES_H
