// ***************************************************************************************
// ***************************************************************************************
//
//      Name :      triangle.c
//      Authors :   Paul Robson (paul@robsons.org.uk)
//                  Bastian Molkenthin
//      Date :      29th December 2024
//      Reviewed :  No
//      Purpose :   Triangle Drawing functions
//
// ***************************************************************************************
// ***************************************************************************************

#include "common.h"
#include "include/atomic.h"

typedef struct _Vertice {
    float x,y;
} Vertice;

static Vertice vt1,vt2,vt3;
static int colour = 2;

// ***************************************************************************************
//
//                                      Sort the vertices
//
// ***************************************************************************************

static void sortVerticesAscendingByY(void)
{
    Vertice     vTmp;

    if (vt1.y > vt2.y) {
        vTmp = vt1;
        vt1 = vt2;
        vt2 = vTmp;
    }
    /* here v1.y <= v2.y */
    if (vt1.y > vt3.y) {
        vTmp = vt1;
        vt1 = vt3;
        vt3 = vTmp;
    }
    /* here v1.y <= v2.y and v1.y <= v3.y so test v2 vs. v3 */
    if (vt2.y > vt3.y) {
        vTmp = vt2;
        vt2 = vt3;
        vt3 = vTmp;
    }
}

// ***************************************************************************************
//
//                           Fill a triangle with a flat bottom
//
// ***************************************************************************************

static void fillBottomFlatTriangle(Vertice v1,Vertice v2,Vertice v3) {
    float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
    float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

    float curx1 = v1.x;
    float curx2 = v1.x;

    for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++) {
        GFXAHorizLine((int)curx1,(int)curx2,scanlineY,colour);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

// ***************************************************************************************
//
//                           Fill a triangle with a flat top
//
// ***************************************************************************************

static void fillTopFlatTriangle(Vertice v1,Vertice v2,Vertice v3)
{
    float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
    float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

    float curx1 = v3.x;
    float curx2 = v3.x;

    for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--) {
        GFXAHorizLine((int)curx1,(int)curx2,scanlineY,colour);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

// ***************************************************************************************
//
//                           Draw a filled triangle
//
// ***************************************************************************************

static void drawFilledTriangle() {
    /* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
    sortVerticesAscendingByY();

    /* here we know that v1.y <= v2.y <= v3.y */
    /* check for trivial case of bottom-flat triangle */
    if (vt2.y == vt3.y) {
        fillBottomFlatTriangle(vt1, vt2, vt3);
    }
    /* check for trivial case of top-flat triangle */
    else if (vt1.y == vt2.y) {
        fillTopFlatTriangle(vt1, vt2, vt3);
    } else {
        /* general case - split the triangle in a topflat and bottom-flat one */
        Vertice vt4;
        vt4.x =  (int)(vt1.x + ((float)(vt2.y - vt1.y) / (float)(vt3.y - vt1.y)) * (vt3.x - vt1.x));
        vt4.y = vt2.y;
        fillBottomFlatTriangle(vt1, vt2, vt4);
        fillTopFlatTriangle(vt2, vt4, vt3);
    }
}

// ***************************************************************************************
//
//                           Link in draw filled triangle function
//
// ***************************************************************************************

void GFXFillTriangle(GFXPort *vp,int x0,int y0,int x1,int y1,int x2,int y2,int col) {
    GFXASetPort(vp);
    vt1.x = x0;vt1.y = y0;
    vt2.x = x1;vt2.y = y1;
    vt3.x = x2;vt3.y = y2;
    colour = col;
    drawFilledTriangle();
}

// ***************************************************************************************
//
//                           Draw framed triangle (e.g. outline)
//
// ***************************************************************************************

void GFXFrameTriangle(GFXPort *vp,int x0,int y0,int x1,int y1,int x2,int y2,int col) {
    GFXASetPort(vp);
    GFXALine(x0,y0,x1,y1,col);
    GFXALine(x0,y0,x2,y2,col);
    GFXALine(x2,y2,x1,y1,col);
}        