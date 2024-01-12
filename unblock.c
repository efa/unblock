/* Unblock v0.00.00a 2024/01/12 Unblock cubic tiles for ZX Spectrum         */
/* Copyright 2023-2024 Valerio Messina http://users.iol.it/efa              */
/* unblock.c is part of Unblock
   Unblock is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Unblock is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU Lesser General Public License
   along with Unblock.  If not, see <http://www.gnu.org/licenses/>. */

// Unblock cubic tiles for ZX Spectrum
// Generate with: $ zcc +zx -vn -lndos -lm -create-app unblock.c -o unblock.ZX8
// using the x88dk cross-compiler https://github.com/z88dk/z88dk

#include <stdio.h> // printf(): 0,0 up-left, 63,23 down-right
//#include <stdlib.h> // malloc() use -DAMALLOC, rand()
#include <conio.h> // textcolor(), remap with textColor[8]
#include <graphics.h> // 0,0 up-left, 255,191 down-right, 31,23 char/colors

#define OK 0
#define ERR 255

#define BLK 0x00 //  0 for textcolor()
#define BLU 0x01 //  1 for textcolor()
#define RED 0x02 //  4 for textcolor()
#define MAG 0x03 //  5 for textcolor()
#define GRE 0x04 //  2 for textcolor()
#define CYA 0x05 //  3 for textcolor()
#define YEL 0x06 // 14 for textcolor()
#define WHI 0x07 //  7 for textcolor()
#define BRI 0x40
#define FLA 0x80
#define COLOR(PAPER,INK) (((PAPER&7)<<3)|(INK&7))

#define SIZEX 5
#define SIZEY 4
#define SIZEZ 3
#define SIZEM (SIZEX+SIZEY+SIZEZ)
#define WAIT 1000 // ms. 100 is smooth
#define mSECOND 35

typedef unsigned char  bit;
typedef unsigned char  u08;
typedef   signed char  s08;
typedef unsigned short u16;

u08 co=0; // column origin of current shape side
u08 ro=0; // row    origin of current shape side
u08 dtby=0; // when 1 Draw Tiles  on Background in Yellow
u08 dlby=1; // when 1 Draw Layers on Background in Yellow
//u08 shapeSide=0; // keep current shape side
u08 shapeRot=0;  // keep current shape rotation

u08 tileFace=0; // current face of current tile
u08 tileDepth=0; // layer depth of current tile
u08 tileMov=0; // free movements of current tile

static char textColor[8] = { 0, 1, 4, 5, 2, 3, 14, 7 };

u08 shape0[SIZEZ][SIZEY][SIZEX]={{{0,5,0,3,4}, // shape[d][r][c]
                                  {2,3,0,0,6},
                                  {6,5,0,0,3},
              /* [0][0][0] ==> */ {0,3,0,6,0}},
                                 {{6,0,3,0,2},
                                  {6,5,0,0,2},
                                  {6,0,0,6,2},
                                  {6,5,0,0,0}},
                                 {{3,2,6,0,1},
                                  {0,0,4,0,2},
                                  {3,0,6,5,0},
                                  {6,5,2,0,0}}};

u08 shape1[SIZEZ][SIZEY][SIZEX]={{{1,2,3,4,5}, // shape[d][r][c]
                                  {6,1,2,3,4},
                                  {5,6,1,2,3},
              /* [0][0][0] ==> */ {4,5,6,1,2}},
                                 {{0,0,0,0,0},
                                  {0,0,0,0,0},
                                  {0,0,0,0,0},
                                  {0,0,0,0,0}},
                                 {{0,0,0,0,0},
                                  {0,0,0,0,0},
                                  {0,0,0,0,0},
                                  {0,0,0,0,0}}};

u08 shape[SIZEM][SIZEM][SIZEM]; // shape[z][y][x]
u08 shaps[SIZEM][SIZEM][SIZEM]; // shaps[z][y][x]
//u08* shapePtr; // will point to shape or shaps
//long heap;

u08 sizeM; // contain the shape max side on 3 axis

// pause for X milli-second
void pause(u16 ms) {
   for (u16 t=0; t<ms; t++)
      for (u16 m=0; m<mSECOND; m++) { }
} // pause(u16 ms)

// empty shape[]
void blankShape() {
   for (u08 z=0; z<SIZEM; z++) {
      for (u08 y=0; y<SIZEM; y++) {
         for (u08 x=0; x<SIZEM; x++) {
            shape[z][y][x]=0; // empty space
            shaps[z][y][x]=0; // empty space
         }
      }
   }
} // blankShape()

// fill shape[] with shape0[]
void shapeInit() {
   blankShape();
   for (u08 z=0; z<SIZEZ; z++) {
      for (u08 y=0; y<SIZEY; y++) {
         for (u08 x=0; x<SIZEX; x++) {
            shape[z][y][x]=shape0[z][y][x]; // copy default shape
         }
      }
   }
} // shapeInit()

// find the max coordinate of the shape
void sizeMax() { // ToDO: explore whole shape to get real max coordinate
   if (SIZEX>SIZEY) {
      if (SIZEZ>SIZEX) sizeM=SIZEZ;
      else sizeM=SIZEX;
   } else {
      if (SIZEY>SIZEZ) sizeM=SIZEY;
      else sizeM=SIZEZ;
   }
   //printf("SIZEZ:%u SIZEY:%u SIZEX:%u SIZEM:%u sizeM:%u\n", SIZEZ, SIZEY, SIZEX, SIZEM, sizeM);
} // sizeMax()

// delete one tile at r+ro[23:0],c+co[31:0]
u08 delTile(u08 r, u08 c) {
   u08 ca=co+c;
   u08 ra=ro+r;
   if (ca>31 || ra>23) return ERR;
   u08 x = ca*8;
   u08 y = ra*8;
   //textcolor(textColor[WHI]);
   clga(x, y, 8, 8);
   return OK;
} // delTile(u08 r, u08 c)

// tile
// face:0       ^1       >2       !3       <4       x5       o6    
// 0123456780123456780123456780123456780123456780123456780123456780
// 1        1+------+1+------+1+------+1+------+1+------+1+------+1
// 2        2|      |2|  O   |2|      |2|   O  |2|      |2|      |2
// 3        3|  OO  |3|   O  |3|  OO  |3|  O   |3| O  O |3|      |3
// 4        4| OOOO |4| OOOO |4|O OO O|4| OOOO |4|  OO  |4|  OO  |4
// 5        5|O OO O|5| OOOO |5| OOOO |5| OOOO |5|  OO  |5|  OO  |5
// 6        6|  OO  |6|   O  |6|  OO  |6|  O   |6| O  O |6|      |6
// 7        7|      |7|  O   |7|      |7|   O  |7|      |7|      |7
// 8        8+------+8+------+8+------+8+------+8+------+8+------+8
// 0123456780123456780123456780123456780123456780123456780123456780
// draw at r+ro[23:0],c+co[31:0] one tile with 'face[6:0]' rotated 'rot[3:0]'
u08 drawTile(u08 r, u08 c, u08 face, u08 rot) {
   u08 ca=co+c;
   u08 ra=ro+r;
   if (ca>31 || ra>23 || face>6 || rot>3) return ERR;
   u08 x = ca*8;
   u08 y = ra*8;
   if (face>0) drawb(x, y, 8, 8);
   switch (face) {
   case 1:
      textcolor(textColor[BLU]);
      break;
   case 2:
      textcolor(textColor[MAG]);
      break;
   case 3:
      textcolor(textColor[RED]);
      break;
   case 4:
      textcolor(textColor[GRE]);
      break;
   case 5:
      textcolor(textColor[CYA]);
      break;
   case 6:
      textcolor(textColor[BLK]);
   }
   if (dtby==1) textcolor(textColor[YEL]);
   if (rot>0 && face>=1 && face<=4) {
      face=face+rot;
      if (face>4) face=face-4;
   }
   //printf("face:%hu\n", face);
   switch (face) {
   case 1:
      draw(x+1,y+4,x+3,y+2);
      draw(x+3,y+3,x+3,y+5);
      draw(x+4,y+5,x+4,y+3);
      draw(x+4,y+2,x+6,y+4);
      break;
   case 2:
      draw(x+3,y+1,x+5,y+3);
      draw(x+4,y+3,x+2,y+3);
      draw(x+2,y+4,x+4,y+4);
      draw(x+5,y+4,x+3,y+6);
      break;
   case 3:
      draw(x+1,y+3,x+3,y+5);
      draw(x+4,y+4,x+4,y+2);
      draw(x+3,y+2,x+3,y+4);
      draw(x+4,y+5,x+6,y+3);
      break;
   case 4:
      draw(x+4,y+1,x+2,y+3);
      draw(x+3,y+4,x+5,y+4);
      draw(x+5,y+3,x+3,y+3);
      draw(x+2,y+4,x+4,y+6);
      break;
   case 5:
      draw(x+2,y+2,x+5,y+5);
      draw(x+2,y+5,x+5,y+2);
      break;
   case 6:
      draw(x+3,y+3,x+4,y+3);
      draw(x+3,y+4,x+4,y+4);
   }
   return OK;
} // drawTile(u08 r, u08 c, u08 face, u08 rot)

#if 0
// draw at ro,co one layer with depth 'z' view looking down
u08 drawShapeSingleLayer(u08 z) {
   if (co>31 || ro>23 || z>=SIZEZ) return ERR;
   for (u08 r=0; r<SIZEY; r++) {
      for (u08 c=0; c<SIZEX; c++) {
         //printf("c:%hu=%hu\n", c, face[r][c]);
         drawTile(r, c, shape[z][r][c], 0);
      }
   }
   return OK;
} // drawShapeSingleLayer(u08 z)
#endif

// calc new face/tile orientation on 'mov'
u08 calcTileFace(u08 face, u08 mov) {
   if (face>6 || mov>3) return ERR;
   u08 ret;
   switch (face) {
   case 0: // ok
      ret=0;
      break;
   case 1: // ok
      if (mov==0) ret=5;
      if (mov==1) ret=face;
      if (mov==2) ret=6;
      if (mov==3) ret=face;
      break;
   case 2: // ok
      if (mov==0) ret=face;
      if (mov==1) ret=5;
      if (mov==2) ret=face;
      if (mov==3) ret=6;
      break;
   case 3: // ok
      if (mov==0) ret=6;
      if (mov==1) ret=face;
      if (mov==2) ret=5;
      if (mov==3) ret=face;
      break;
   case 4: // ok
      if (mov==0) ret=face;
      if (mov==1) ret=6;
      if (mov==2) ret=face;
      if (mov==3) ret=5;
      break;
   case 5: // ok
      if (mov==0) ret=3;
      if (mov==1) ret=4;
      if (mov==2) ret=1;
      if (mov==3) ret=2;
      break;
   case 6: // ok
      if (mov==0) ret=1;
      if (mov==1) ret=2;
      if (mov==2) ret=3;
      if (mov==3) ret=4;
   } // switch (face)
   return ret;
} // u08 calcTileFace(u08 face, u08 mov)

// rotate 'shape[]' on 'mov' (use 'shape2[]' as tmp)
u08 rotShape(u08 mov) {
   if (mov>3) return ERR;
   //printf(" shapeSide:%u shapeRot:%u mov:%u\n", shapeSide, shapeRot, mov);
   //blankShape(); // target shape2[]
   u08 face;
   // mov0,2: x for z,y
   // mov1,3: y for z,x
   for (u08 z=0; z<sizeM; z++) {
      for (u08 y=0; y<sizeM; y++) {
         for (u08 x=0; x<sizeM; x++) {
            switch (mov) { // shape[z][y][x]
            case 0: // ok
               //face = shape[z][sizeM-1-y][x];
               //printf("face:%u calcTileFace():%u\n", face, calcTileFace(shape[z][sizeM-1-y][x], mov));
               face = calcTileFace(shape[z][sizeM-1-y][x], mov);
               shaps[y][z][x] = face;
               break;
            case 1: // ok
               face = calcTileFace(shape[z][y][x], mov);
               shaps[x][y][sizeM-1-z] = face;
               break;
            case 2: // ok
               face = calcTileFace(shape[z][y][x], mov);
               shaps[y][sizeM-1-z][x] = face;
               break;
            case 3: // 
               face = calcTileFace(shape[z][y][sizeM-1-x], mov);
               shaps[x][y][z] = face;
            } // switch (mov)
         } // inner for
      } // middle for
   } // outer for
   //shapeSide=side;
   //shapeRot=rot;
   for (u16 b=0; b<sizeof(shape); b++) { // copy back to shape[]
      *(shape+b)=*(shaps+b);
   }
   return OK;
} // u08 rotShape(u08 side, u08 rot)

// draw at ro,co one layer 'depth', hide=1 only uncovered, seen from 'shapeSide' rotated 'shapeRot'
u08 drawShapeLayer(u08 depth, bit hide) {
   if (co>31 || ro>23) return ERR;
   if (depth>=sizeM) return ERR;
   if (dlby==1 && depth>0) dtby=1;
   for (u08 r=0; r<sizeM; r++) {
      for (u08 c=0; c<sizeM; c++) {
         bit v;
         v=1;
         if (hide) {
            for (s08 d=depth-1; d>=0; d--) {
               if (shape[d][r][c]!=0) v=0; // hide when covered
            }
         }
         if (v || !hide) drawTile(r, c, shape[depth][r][c], shapeRot);
      } // inner for
   } // outer for
   dtby=0;
   return OK;
} // drawShapeLayer(u08 depth, bit hide)

// draw at ro,co the shape seen from 'shapeSide' rotated 'shapeRot'
u08 drawShapeSide() {
   if (co>31 || ro>23) return ERR;
   for (s08 d=sizeM-1; d>=0; d--) {
      //printf(" d:%hu\n", d);
      drawShapeLayer(d, 1);
   }
   return OK;
} // drawShapeSide()

// extract the tile 'face' at r,c on layer 'd' from shape[] 'shapeSide' rotated 'shapeRot'
// face is returned as viewed from top (side=0), with no rotation (rot=0)
u08 getLayerFace(u08 r, u08 c, u08 d) { // optimize: shape[d][r][c]
   if (c>31 || r>23) return ERR;
   if (d>sizeM) return ERR;
   u08 face = shape[d][r][c];
   //printf(" face:%hu\n", face);
   return face;
} // getLayerFace(u08 r, u08 c, u08 d)

// return 'face' of tile at r,c with origin ro,co
u08 getShapeFace(u08 r, u08 c) {
   if (co>31 || ro>23) return ERR;
   u08 face;
   u08 d;
   for (d=0; d<sizeM; d++) {
      face=getLayerFace(r, c, d); // optimize: shape[d][r][c]
      if (face>0) break;
   }
   //printf(" depth:%hu\n", d);
   //printf(" face:%hu\n", face);
   return face;
} // u08 getShapeFace(u08 r, u08 c)

// return 'depth' of tile at r,c with origin ro,co
u08 getShapeDepth(u08 r, u08 c) {
   if (co>31 || ro>23) return ERR;
   u08 face;
   u08 d;
   for (d=0; d<sizeM; d++) {
      face=getLayerFace(r, c, d); // optimize: shape[d][r][c]
      if (face>0) break;
   }
   //printf(" depth:%hu\n", d);
   //printf(" face:%hu\n", face);
   return d;
} // u08 getShapeDepth(u08 r, u08 c)

// try to unblock tile at r,c with origin ro,co
u08 unblockTile(u08 r, u08 c) {
   //printf(" r:%u c:%u\n", r, c);
   tileFace=getShapeFace(r, c);
   tileDepth=getShapeDepth(r, c);
   //printf(" tileFace:%u tileDepth:%u\n", tileFace, tileDepth);
   int s; // s08 won't work with z88dk v2.3 --fsigned-char too
   u08 ret=OK;
   switch (tileFace) {
   case 0:
      tileMov=0;
      break;
   case 1:
      for (s=r-1; s>=-1; s--) {
         u08 face=getLayerFace(s, c, tileDepth);
         if(s==-1) face=0; // below s=0
         //printf(" s:%d face:%u\n", s, face);
         u08 d=getShapeDepth(s, c);
         //printf(" s:%d d:%u\n", s, d);
         if (face==0) { // free pos
            //printf(" move up\n");
            shape[tileDepth][s+1][c]=0;
            d=getShapeDepth(s+1, c); // current pos
            if(d>tileDepth) { // we are fore layers
               //printf(" del tile\n");
               delTile(s+1, c);
               //printf(" restore back\n");
               for (s08 d=sizeM; d>tileDepth; d--) { // redraw back layers
                  //printf(" faceM:%u\n", shape[d][s+1][c]);
                  drawShapeLayer(d, 1);
               }
            }
            if(s==-1) break; // break for, below s=0
            d=getShapeDepth(s, c); // target pos
            shape[tileDepth][s][c]=tileFace;
            if (d>tileDepth) { // only back layers
               delTile(s, c);
               //printf(" draw new tile\n");
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(s, c, tileFace, shapeRot);
               dtby=0;
            } // else printf(" we are back\n");
            tileMov=r-s;
            pause(WAIT);
         } else { // busy pos
            tileMov=r-s-1;
            ret=ERR; // blocked
            for (u08 b=s+1; b<r; b++) {
               //printf(" b:%u back down\n", b);
               shape[tileDepth][b][c]=0;
               d=getShapeDepth(b, c); // current pos
               if(d>tileDepth) { // we are fore layers
                  //printf(" del tile d:%u\n", d);
                  delTile(b, c);
                  //printf(" restore back d:%u\n", d);
                  for (s08 d=sizeM; d>tileDepth; d--) { // redraw back layers
                     //printf(" faceB:%u b:%u d:%u \n", shape[d][b][c], b, d);
                     drawShapeLayer(d, 1);
                  }
               }
               d=getShapeDepth(b+1, c); // target pos
               shape[tileDepth][b+1][c]=tileFace;
               if (d>tileDepth) { // only back layers
                  delTile(b+1, c);
                  //printf(" draw new tile\n");
                  if (dlby==1 && tileDepth>0) dtby=1;
                  drawTile(b+1, c, tileFace, shapeRot);
                  dtby=0;
               } // else printf(" we are back\n");
               pause(WAIT);
               //printf(" ---\n");
            }
            break;
         }
         //printf(" ---\n");
      } // for
      if (ret==OK) {
         //printf(" ---\n");
         //delTile(s+1, c); // remove tile out of shape
         //shape[tileDepth][s+1][c]=0; // remove tile from shape
      }
      break;
   case 2:
      for (s=c+1; s<sizeM; s++) {
         u08 face=getLayerFace(r, s, tileDepth);
         //printf(" s:%d face:%u\n", s, face);
         u08 d=getShapeDepth(r, s);
         //printf(" s:%d d:%u\n", s, d);
         if (face==0) { // free pos
            //printf(" move right\n");
            shape[tileDepth][r][s-1]=0;
            d=getShapeDepth(r, s-1); // current pos
            if(d>tileDepth) { // we are fore layers
               //printf(" del tile\n");
               delTile(r, s-1);
               //printf(" restore back\n");
               for (s08 d=sizeM; d>tileDepth; d--) { // redraw back layers
                  //printf(" faceM:%u\n", shape[d][r][s-1]);
                  drawShapeLayer(d, 1);
               }
            }
            d=getShapeDepth(r, s); // target pos
            shape[tileDepth][r][s]=tileFace;
            if (d>tileDepth) { // only back layers
               delTile(r, s);
               //printf(" draw new tile\n");
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(r, s, tileFace, shapeRot);
               dtby=0;
            } // else printf(" we are back\n");
            tileMov=s-c;
            pause(WAIT);
         } else { // busy pos
            tileMov=s-c-1;
            ret=ERR; // blocked
            for (u08 b=s-1; b>=c+1; b--) {
               //printf(" b:%u back left\n", b);
               shape[tileDepth][r][b]=0;
               d=getShapeDepth(r, b); // current pos
               if(d>tileDepth) { // we are fore layers
                  //printf(" del tile d:%u\n", d);
                  delTile(r, b);
                  //printf(" restore back d:%u\n", d);
                  for (s08 d=sizeM; d>tileDepth; d--) { // redraw back layers
                     //printf(" faceB:%u b:%u d:%u \n", shape[d][r][b], b, d);
                     drawShapeLayer(d, 1);
                  }
               }
               d=getShapeDepth(r, b-1); // target pos
               shape[tileDepth][r][b-1]=tileFace;
               if (d>tileDepth) { // only back layers
                  delTile(r, b-1);
                  //printf(" draw new tile\n");
                  if (dlby==1 && tileDepth>0) dtby=1;
                  drawTile(r, b-1, tileFace, shapeRot);
                  dtby=0;
               } // else printf(" we are back\n");
               pause(WAIT);
               //printf(" ---\n");
            }
            break;
         }
         //printf(" ---\n");
      } // for
      if (ret==OK) {
         delTile(r, s-1); // remove tile out of shape
         shape[tileDepth][r][s-1]=0; // remove tile from shape
      }
      break;
   case 3:
      for (s=r+1; s<sizeM; s++) {
         u08 face=getLayerFace(s, c, tileDepth);
         //printf(" s:%d face:%u\n", s, face);
         u08 d=getShapeDepth(s, c);
         //printf(" s:%d d:%u\n", s, d);
         if (face==0) { // free pos
            //printf(" move down\n");
            shape[tileDepth][s-1][c]=0;
            d=getShapeDepth(s-1, c); // current pos
            if(d>tileDepth) { // we are fore layers
               //printf(" del tile\n");
               delTile(s-1, c);
               //printf(" restore back\n");
               for (s08 d=sizeM; d>tileDepth; d--) { // redraw back layers
                  //printf(" faceM:%u\n", shape[d][s-1][c]);
                  drawShapeLayer(d, 1);
               }
            }
            d=getShapeDepth(s, c); // target pos
            shape[tileDepth][s][c]=tileFace;
            if (d>tileDepth) { // only back layers
               delTile(s, c);
               //printf(" draw new tile\n");
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(s, c, tileFace, shapeRot);
               dtby=0;
            } // else printf(" we are back\n");
            tileMov=s-r;
            pause(WAIT);
         } else { // busy pos
            tileMov=s-r-1;
            ret=ERR; // blocked
            for (u08 b=s-1; b>=r+1; b--) {
               //printf(" b:%u back up\n", b);
               shape[tileDepth][b][c]=0;
               d=getShapeDepth(b, c); // current pos
               if(d>tileDepth) { // we are fore layers
                  //printf(" del tile d:%u\n", d);
                  delTile(b, c);
                  //printf(" restore back d:%u\n", d);
                  for (s08 d=sizeM; d>tileDepth; d--) { // redraw back layers
                     //printf(" faceB:%u b:%u d:%u \n", shape[d][b][c], b, d);
                     drawShapeLayer(d, 1);
                  }
               }
               d=getShapeDepth(b-1, c); // target pos
               shape[tileDepth][b-1][c]=tileFace;
               if (d>tileDepth) { // only back layers
                  delTile(b-1, c);
                  //printf(" draw new tile\n");
                  if (dlby==1 && tileDepth>0) dtby=1;
                  drawTile(b-1, c, tileFace, shapeRot);
                  dtby=0;
               } // else printf(" we are back\n");
               pause(WAIT);
               //printf(" ---\n");
            }
            break;
         }
         //printf(" ---\n");
      } // for
      if (ret==OK) {
         delTile(s-1, c); // remove tile out of shape
         shape[tileDepth][s-1][c]=0; // remove tile from shape
      }
      break;
   case 4:
      for (s=c-1; s>=-1; s--) {
         u08 face=getLayerFace(r, s, tileDepth);
         if(s==-1) face=0; // below s=0
         //printf(" s:%d face:%u\n", s, face);
         u08 d=getShapeDepth(r, s);
         //printf(" s:%d d:%u\n", s, d);
         if (face==0) { // free pos
            //printf(" move left\n");
            shape[tileDepth][r][s+1]=0;
            d=getShapeDepth(r, s+1); // current pos
            if(d>tileDepth) { // we are fore layers
               //printf(" del tile\n");
               delTile(r, s+1);
               //printf(" restore back\n");
               for (s08 d=sizeM; d>tileDepth; d--) { // redraw back layers
                  //printf(" faceM:%u\n", shape[d][r][s+1]);
                  drawShapeLayer(d, 1);
               }
            }
            if(s==-1) break; // break for, below s=0
            d=getShapeDepth(r, s); // target pos
            shape[tileDepth][r][s]=tileFace;
            if (d>tileDepth) { // only back layers
               delTile(r, s);
               //printf(" draw new tile\n");
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(r, s, tileFace, shapeRot);
               dtby=0;
            } // else printf(" we are back\n");
            tileMov=r-s;
            pause(WAIT);
         } else { // busy pos
            tileMov=c-s-1;
            ret=ERR; // blocked
            for (u08 b=s+1; b<c; b++) {
               //printf(" b:%u back right\n", b);
               shape[tileDepth][r][b]=0;
               d=getShapeDepth(r, b); // current pos
               if(d>tileDepth) { // we are fore layers
                  //printf(" del tile d:%u\n", d);
                  delTile(r, b);
                  //printf(" restore back d:%u\n", d);
                  for (s08 d=sizeM; d>tileDepth; d--) { // redraw back layers
                     //printf(" faceB:%u b:%u d:%u \n", shape[d][r][b], b, d);
                     drawShapeLayer(d, 1);
                  }
               }
               d=getShapeDepth(r, b+1); // target pos
               shape[tileDepth][r][b+1]=tileFace;
               if (d>tileDepth) { // only back layers
                  delTile(r, b+1);
                  //printf(" draw new tile\n");
                  if (dlby==1 && tileDepth>0) dtby=1;
                  drawTile(r, b+1, tileFace, shapeRot);
                  dtby=0;
               } // else printf(" we are back\n");
               pause(WAIT);
               //printf(" ---\n");
            }
            break;
         }
         //printf(" ---\n");
      } // for
      if (ret==OK) {
         //printf(" ---\n");
         //delTile(r, s+1); // remove tile out of shape
         //shape[tileDepth][r][s+1]=0; // remove tile from shape
      }
      break;
   case 5:
      for (s=tileDepth+1; s<sizeM; s++) {
         shape[s-1][r][c]=0; // remove current tile
         u08 face=getLayerFace(r, c, s); // check below
         //printf(" s:%d face:%u\n", s, face);
         u08 d=getShapeDepth(r, c); // check below
         //printf(" s:%d depth:%u\n", s, d);
         shape[s-1][r][c]=tileFace; // restore
         if (face==0) { // free pos
            //printf(" move below\n");
            shape[s-1][r][c]=0; // remove current tile
            shape[s][r][c]=tileFace;
            delTile(r, c);
            //printf(" draw new tile\n");
            if (dlby==1 && s>0) dtby=1;
            drawTile(r, c, tileFace, shapeRot);
            dtby=0;
            tileMov=s-tileDepth-1;
            pause(WAIT);
         } else { // busy pos
            tileMov=s-tileDepth-1;
            ret=ERR; // blocked
            for (u08 b=s-1; b>=tileDepth+1; b--) {
               //printf(" b:%u back over\n", b);
               shape[b][r][c]=0;
               shape[b-1][r][c]=tileFace;
               delTile(r, c);
               //printf(" draw new tile\n");
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(r, c, tileFace, shapeRot);
               dtby=0;
               pause(WAIT);
               //printf(" ---\n");
            }
            break;
         }
         //printf(" ---\n");
      } // for
      if (ret==OK) {
         delTile(r, c); // remove tile out of shape
         shape[s-1][r][c]=0; // remove tile from shape
      }
      break;
   case 6:
      for (s=tileDepth-1; s>=-1; s--) {
         //printf(" s:%d\n", s);
         shape[tileDepth][r][c]=0; // remove current tile
         if(s>=0) {
            //printf(" move over\n");
            delTile(r, c);
            //printf(" draw new tile\n");
            if (dlby==1 && s>0) dtby=1;
            drawTile(r, c, tileFace, shapeRot);
            dtby=0;
            tileMov=tileDepth-s;
            pause(WAIT);
         }
         //printf(" ---\n");
      } // for
      delTile(r, c); // remove tile out of shape
      break;
   } // switch (tileFace)
   return ret;
} // u08 unblockTile(u08 r, u08 c)

#include "keys.c"

void main() {
   clg(); // clear
   textcolor(textColor[BLK]);
   drawb(0,0,256,192);
   //printf("\n Unblock by V.Messina\n");
   printf("\n");
   //printf("\n\n\n\n\n");
   //printf("                            Layer 0     Layer 1     Layer 2\n");

   //mallinit();              // heap cleared to empty
   //sbrk(65535-SIZEM*SIZEM*SIZEM,SIZEM*SIZEM*SIZEM); // initialize heap size
   //u08 size=sizeof(shape0);
   //printf("sizeof(shape):%u sizeof(shape0):%u\n", sizeof(shape), size);
   //shapePtr=malloc(size);
   //printf("shapePtr:%p\n", shapePtr);

   shapeInit();
   sizeMax();

   //ro=1;
   //co=14; drawShapeSingleLayer(0);
   //co=20; drawShapeSingleLayer(1);
   //co=26; drawShapeSingleLayer(2);
   //textcolor(textColor[BLK]);

   //ro=9;
   //co= 2; drawShapeLayer(0, 0);
   //co= 8; drawShapeLayer(1, 0);
   //co=14; drawShapeLayer(2, 0);
   //co=20; drawShapeLayer(0, 1);
   //co=20; drawShapeLayer(1, 1);
   //co=20; drawShapeLayer(2, 1);

   //u08 face=shape[0][0][0]; // shape[d][r][c]
   //face=1;
   //printf(" face:%hu\n", face);
   //ro=9; co=8;
   // drawTileSide(u08 r, u08 c, u08 face, u08 side, u08 rot)
   //drawTileSide(0,0, face, 0, 0);
   //drawTileSide(0,1, face, 1, 0);
   //drawTileSide(0,2, face, 2, 0);
   //drawTileSide(0,3, face, 3, 0);
   //drawTileSide(0,4, face, 4, 0);
   //drawTileSide(0,5, face, 5, 0);

   //textcolor(textColor[BLK]);
   //printf("    Rotate on perpendicular axis\n");
   //u08 s=0;
   //printf("    Side %hu, Rotate on Y|R axis. Hide=0\n", s);
   //u08 rot=1;
   //ro=9;
   //co= 2; drawShapeLayer(s, 0); // drawShapeLayer(depth, hide)
   //co= 8; drawShapeLayer(s, 1);
   //co=14; drawShapeLayer(s, 2);
   //co=20; drawShapeLayer(s, 3);

   //ro=9;
   //co=14; drawShapeLayer(2, 0);
   //co= 2; drawShapeSide0(3, 0); // drawShapeSide0(side, rot)
   //co= 8; drawShapeSide0(3, 1);
   //co=14; drawShapeSide0(3, 2);
   //co=20; drawShapeSide0(3, 3);
   //co= 8; drawShapeSide0(s, 0);
   //co=14; drawShapeSide0(s, 0);
   //co=20; drawShapeSide0(s, 0);
   //textcolor(textColor[BLK]);
   //printf("\n\n\n\n\n    Rot 0       Rot 1       Rot 2       Rot 3\n");
   //printf("\n\n\n\n    Depth 0     Depth 1     Depth 2     Depth 3\n");
   //printf("    Side %hu, Rotate on Y|R axis. Hide=1\n", s);
   //ro=15;
   //co= 2; drawShapeLayer(0, 1); // drawShapeLayer(depth, hide)
   //co= 8; drawShapeLayer(1, 1);
   //co=14; drawShapeLayer(2, 1);
   //co= 2; drawShapeSide0(s, 0);

   //printf("    Rotate on horizontal axis\n");
   //ro=14;
   //co= 2; drawShapeSide0(0, 0);
   //co=14; drawShapeSide0(5, 0);
   //textcolor(textColor[BLK]);
   //printf("\n\n\n\n    Side 0      Side 5\n");

   //co=14; ro=14;
   //drawTile(0, 0, 1, 0);
   //drawTile(0, 1, 1, 1);
   //drawTile(0, 2, 1, 2);
   //drawTile(0, 3, 1, 3);

   //drawTile(0, 5, 2, 0);
   //drawTile(0, 6, 2, 1);
   //drawTile(0, 7, 2, 2);
   //drawTile(0, 8, 2, 3);

   //drawTile(0,10, 3, 0);
   //drawTile(0,11, 3, 1);
   //drawTile(0,12, 3, 2);
   //drawTile(0,13, 3, 3);

   //ro=15;
   //drawTile(0, 0, 4, 0);
   //drawTile(0, 1, 4, 1);
   //drawTile(0, 2, 4, 2);
   //drawTile(0, 3, 4, 3);

   //drawTile(0, 5, 5, 0);
   //drawTile(0, 6, 5, 1);
   //drawTile(0, 7, 5, 2);
   //drawTile(0, 8, 5, 3);

   //drawTile(0,10, 6, 0);
   //drawTile(0,11, 6, 1);
   //drawTile(0,12, 6, 2);
   //drawTile(0,13, 6, 3);

   //printf("    Rotate on vertical Y|R axis\n");
   //dlfy=1;
   //ro=9;
   //co= 8; drawShapeSide0(1, 2); // drawShapeSide0(side, rot)
   //ro=13;
   //co= 2; drawShapeSide0(2, 3);
   //co= 8; drawShapeSide0(0, 0);
   //co=14; drawShapeSide0(4, 1);
   //co=20; drawShapeSide0(5, 2);
   //ro=18;
   //co= 8; drawShapeSide0(3, 2);

   //u08 side=3;
   //u08 rot=0;
   //u08 depth=0;
   //co=2; ro=13;
   //drawShapeLayer(depth, 0); // drawShapeLayer(depth, hide)
   //u08 face0=getLayerFace(0,0, side, rot, depth); // getLayerFace(r, c, side, rot, depth)
   //u08 face1=getLayerFace(0,1, side, rot, depth);
   //u08 face2=getLayerFace(0,2, side, rot, depth);
   //u08 face3=getLayerFace(0,3, side, rot, depth);
   //u08 face4=getLayerFace(0,4, side, rot, depth);
   //textcolor(textColor[BLK]);
   //printf("\n face0:%hu face1:%hu face2:%hu face3:%hu face4:%hu\n", face0, face1, face2, face3, face4);
   //printf("\n face0:%hu\n", face0);
   //ro=20;
   //co= 2; drawTile(0,0, face0, 0);
   //co= 3; drawTile(0,0, face1, 0);
   //co= 4; drawTile(0,0, face2, 0);
   //co= 5; drawTile(0,0, face3, 0);
   //co= 6; drawTile(0,0, face4, 0);

   //u08 side=3;
   //u08 rot=0;
   //co=8; ro=8;
   //drawShapeSide(side, rot); // drawShapeSide(side, rot)
   //u08 face0=getShapeFace(3,0); // getShapeFace(r, c)
   //u08 face1=getShapeFace(3,1);
   //u08 face2=getShapeFace(3,2);
   //u08 face3=getShapeFace(3,3);
   //u08 face4=getShapeFace(3,4);
   //textcolor(textColor[BLK]);
   //printf("\n\n\n\n\n\n\n\n\n face0:%hu face1:%hu face2:%hu face3:%hu face4:%hu\n", face0, face1, face2, face3, face4);
   //co=2; ro=8;
   //drawTile(0,0, face0, 0);
   //drawTile(0,1, face1, 0);
   //drawTile(0,2, face2, 0);
   //drawTile(0,3, face3, 0);
   //drawTile(0,4, face4, 0);
   //co=8; ro=13;
   //u08 face5=getShapeFace(3,1);
   //printf(" face5:%hu\n", face5);
   //co=2; ro=8;
   //drawTile( 0,6, face5, 0);

   //u08 side=0;
   //u08 rot=0;
   //co=8; ro=8;
   //drawShapeSide0(side, rot);
   //drawShapeLayer0(side, rot, 0, 0); // drawShapeLayer0(side, rot, depth, hide)

   //u08 rot=0;
   //ro=6; co=14;
   //drawShapeLayer(0, 0);
   //ro=6; co=20;
   //for (u08 r=0; r<sizeM; r++) {
   //   for (u08 c=0; c<sizeM; c++) {
   //      drawTile(r, c, shape[0][r][c], rot);
   //   }
   //}

   //printf(" rotate\n");
   //u08 side=3;
   //rotShape(side,rot);
   //ro=12; co=14;
   //drawShapeLayer(0, 0);
   //ro=12; co=20;
   //for (u08 r=0; r<sizeM; r++) {
   //   for (u08 c=0; c<sizeM; c++) {
   //      drawTile(r, c, shape[0][r][c], rot);
   //   }
   //}

   //textcolor(textColor[BLK]);
   //u08 side=0;
   //u08 rot=0;
   //dlby=0;
   //rotShape(side, rot);
   //co=20; ro=6;
   //drawShapeSide();
   //drawShapeLayer(0, 0);

   //side=3;
   //rot=0;
   //rotShape(side, rot);
   //co=26; ro=6;
   //drawShapeSide();
   //drawShapeLayer(0, 0);
   
   //pause(2000);
   //u08 ret;
   //textcolor(textColor[BLK]);
   //ret=unblockTile(3,1);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(2,4);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(0,3);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(2,4);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(0,2);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(1,2);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(3,2);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(1,2);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(3,2);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(0,4);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(1,2);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(2,1);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(0,1);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(2,0);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);
   //ret=unblockTile(0,0);
   //textcolor(textColor[BLK]);
   //printf(" ret:%hu tileDepth:%hu tileMov:%hu\n", ret, tileDepth, tileMov);

   //u08 side=0;
   //printf(" sizeM:%u\n", sizeM);
   u08 rot=0;
   dlby=0;
   co=26; ro=1;
   drawShapeSide();
   //drawShapeLayer(0, 0);
   //rotShape(rot);
   co=26; ro=6;
   drawShapeSide();
   //drawShapeLayer(0, 0);

//while(1);

   u08 c=0, r=0;
   textcolor(textColor[BLK]);
   drawb((co+c)*8+2, (ro-1)*8+2, 4, 4); // horizontal cursor
   drawb((co-1)*8+2, (ro+r)*8+2, 4, 4); // vertical   cursor

   //printf("\n");
   //printf(" ");
   //printf(" shapeSide:%u shapeRot:%u\n", shapeSide, shapeRot);
   u08 new;
   while (1) {
      readKeys();
      u08 ret;
      u08 mov;
      if (keyWdown()) {
         //printf("w");
         printf(" up:%u\n", shapeRot);
         mov=0;
         rotShape(mov);
         clga(co*8, ro*8, 40, 40);
         drawShapeSide();
      }
      if (keyDdown()) {
         //printf("d");
         printf(" right\n");
         mov=1;
         rotShape(mov);
         clga(co*8, ro*8, 40, 40);
         drawShapeSide();
      }
      if (keySdown()) {
         //printf("s");
         printf(" down\n");
         mov=2;
         rotShape(mov);
         clga(co*8, ro*8, 40, 40);
         drawShapeSide();
      }
      if (keyAdown()) {
         //printf("a");
         printf(" left\n");
         mov=3;
         rotShape(mov);
         clga(co*8, ro*8, 40, 40);
         drawShapeSide();
      }
      if (keyIdown()) {
         //printf("i");
         if(r>0) {
            clga((co-1)*8+2, (ro+r)*8+2, 4, 4);
            r--;
            textcolor(textColor[BLK]);
            drawb((co-1)*8+2, (ro+r)*8+2, 4, 4); // vertical   cursor
         }
      }
      if (keyJdown()) {
         //printf("j");
         if(c>0) {
            clga((co+c)*8+2, (ro-1)*8+2, 4, 4);
            c--;
            textcolor(textColor[BLK]);
            drawb((co+c)*8+2, (ro-1)*8+2, 4, 4); // horizontal cursor
         }
      }
      if (keyKdown()) {
         //printf("k");
         if(r<sizeM) {
            clga((co-1)*8+2, (ro+r)*8+2, 4, 4);
            r++;
            textcolor(textColor[BLK]);
            drawb((co-1)*8+2, (ro+r)*8+2, 4, 4); // vertical   cursor
         }
      }
      if (keyLdown()) {
         //printf("l");
         if(c<sizeM) {
            clga((co+c)*8+2, (ro-1)*8+2, 4, 4);
            c++;
            textcolor(textColor[BLK]);
            drawb((co+c)*8+2, (ro-1)*8+2, 4, 4); // horizontal cursor
         }
      }
      if (keySPdown()) {
         //printf("SP ");
         //printf("r:%u c:%u\n", r, c);
         clga((co+c)*8+2, (ro-1)*8+2, 4, 4);
         clga((co-1)*8+2, (ro+r)*8+2, 4, 4);
         ret=unblockTile(r, c);
         textcolor(textColor[BLK]);
         drawb((co+c)*8+2, (ro-1)*8+2, 4, 4); // horizontal cursor
         drawb((co-1)*8+2, (ro+r)*8+2, 4, 4); // vertical   cursor
         printf(" r:%u c:%u ret=%u\n", r, c, ret);
      }
      pause(132);
   }

   textcolor(textColor[BLK]);
   draw(0,0,0,191);
   while (1);
} // main()
