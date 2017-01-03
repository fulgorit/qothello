#ifndef ELEMENT_H
#define ELEMENT_H

#endif // ELEMENT_H

typedef enum { rouge, vert , jaune, noir } couleurtd;

class element
{

    couleurtd couleur;
    int posx;
    int posy;

    public:
        element();
        void info(void);

};
