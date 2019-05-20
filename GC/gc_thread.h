// PixelEngine
// --------------------------------------------------------------------------
// Copyright (C) Petr Bena - All Rights Reserved
// Unauthorized copying of this file, via any medium is strictly prohibited
// Proprietary and confidential

// Written by Petr Bena 2019

#ifndef GC_THREAD_H
#define GC_THREAD_H

#include <QThread>

namespace PE
{
    class GC_thread : public QThread
    {
            Q_OBJECT
        public:
            GC_thread();
            void Stop();
            bool IsStopped() const;
            bool IsRunning() const;
        protected:
            void run() override;
        private:
            bool running = true;
            bool stopped = false;
    };
}

#endif // GC_THREAD_H
