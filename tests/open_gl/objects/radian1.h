#ifndef RADIAN1_H
#define RADIAN1_H

#include <PixelEngine/actor.h>

class Radian1 : public PE::Actor
{
    public:
        Radian1(const PE::Vector &position);
        void Update(qint64 time = 0);
        void Render(PE::Renderer *r, PE::Camera *c) override;
        double radian = 0;
        double distance = 10;
        PE::Vector v1;
        PE::Vector v2;
};

#endif // RADIAN1_H
