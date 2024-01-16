/* Unblock v0.00.00a 2024/01/16 Unblock cubic tiles for ZX Spectrum         */
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
#include <stdlib.h> // rand(), srand()
#include <conio.h> // textcolor(), remap with textColor[8]
#include <graphics.h> // 0,0 up-left, 255,191 down-right, 31,23 char/colors

#define OK 0
#define WARN 1
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

#define SIZEC 5
#define SIZER 4
#define SIZED 3
#define SIZEM (SIZED+SIZER+SIZEC)
#define WAIT 100 // ms. 100 is smooth
#define mSECOND 35

typedef unsigned char  bit;
typedef unsigned char  u08;
typedef   signed char  s08;
typedef unsigned short u16;

#include "keys.c" // this to avoid a Makefile

u08 sizeM; // contain the shape max side on 3 axis
u08 co=0; // column origin of current shape side
u08 ro=0; // row    origin of current shape side
u08 ax=0; // pixel absolute shape position x
u08 ay=0; // pixel absolute shape position y
u08 sw=0; // pixel shape width
u08 sh=0; // pixel shape height
u08 cy=0; // horizontal cursor position y
u08 cx=0; // vertical   cursor position x
u08 dtby=0; // when 1 Draw Tiles  on Background layers in Yellow
u08 dlby=0; // when 1 Draw Layers on Background layers in Yellow
u16 iblks=0; // initial number of shape blocks
u16 cblks=0; // number of current shape blocks

u08 tileFace=0; // current face of current tile
u08 tileDepth=0; // layer depth of current tile
u08 tileMov=0; // free movements of current tile

extern u08 regR; // R register value

#asm
._regR // R register value
defb 0
#endasm

static char textColor[8] = { 0, 1, 4, 5, 2, 3, 14, 7 };

// tile
// face:0       ^1       >2       v3       <4       x5       .6    
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

u08 shape0[SIZED][SIZER][SIZEC]={{{0,0,0,0,0}, // shape[d][r][c]
                                  {0,0,0,0,0},
                                  {0,0,0,0,0},
             /* [0][0][0] ==> */  {0,0,0,0,0}},
                                 {{2,4,0,0,0},
                                  {2,0,4,0,0},
                                  {2,0,0,4,0},
                                  {2,0,0,0,4}},
                                 {{5,5,5,5,5},
                                  {5,5,5,5,5},
                                  {5,5,5,5,5},
                                  {5,5,5,5,5}}};

u08 shape[SIZEM][SIZEM][SIZEM]; // shape[d][r][c]
u08 shaps[SIZEM][SIZEM][SIZEM]; // shaps[d][r][c]

// pause for X milli-second
void pause(u16 ms) {
   for (u16 t=0; t<ms; t++)
      for (u16 m=0; m<mSECOND; m++) { }
} // pause(u16 ms)

// read R register, used to have a good randomization
void readRegR() {
   __asm
      LD  A, R
      LD  (_regR), A
   __endasm
} // readRegR()

// fill shape0[] with random faces
void fillShape() {
   readRegR();
   srand(regR); 
   for (u08 d=0; d<SIZED; d++) {
      for (u08 r=0; r<SIZER; r++) {
         for (u08 c=0; c<SIZEC; c++) {
            u08 face=rand()/(RAND_MAX/7); // 6:0
            //printf(" face:%d\n", face);
            shape0[d][r][c]=face;
         }
      }
   }
} // fillShape()

// find the max coordinate of the shape
void sizeMax() { // ToDo: explore whole shape to get real max coordinate
   if (SIZEC>SIZER) {
      if (SIZED>SIZEC) sizeM=SIZED;
      else sizeM=SIZEC;
   } else {
      if (SIZER>SIZED) sizeM=SIZER;
      else sizeM=SIZED;
   }
   //printf("SIZED:%u SIZER:%u SIZEC:%u SIZEM:%u sizeM:%u\n", SIZED, SIZER, SIZEC, SIZEM, sizeM);
} // sizeMax()

// empty shape[] or shaps[]
void blankShape(bit e) {
   for (u08 d=0; d<SIZEM; d++) {
      for (u08 r=0; r<SIZEM; r++) {
         for (u08 c=0; c<SIZEM; c++) {
            if (e) shape[d][r][c]=0; // empty space
            else shaps[d][r][c]=0; // empty space
         }
      }
   }
} // blankShape(bit e)

// init variables and fill shape[] with shape0[]
void init() { // take care to set co,ro of working shape before call
   fillShape();
   sizeMax();
   blankShape(1);
   u08 face;
   for (u08 d=0; d<SIZED; d++) {
      for (u08 r=0; r<SIZER; r++) {
         for (u08 c=0; c<SIZEC; c++) {
            face=shape0[d][r][c];
            if (face>0) iblks++;
            shape[d][r][c]=face; // copy default shape
         }
      }
   }
   cblks=iblks;
   ax=co*8;
   ay=ro*8;
   sw=sizeM*8;
   sh=sizeM*8;
   cy=(ro-1)*8+2; // horizontal cursor position y
   cx=(co-1)*8+2; // vertical   cursor position x
} // init()

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

// draw at r+ro[23:0],c+co[31:0] one tile with 'face[6:0]''
u08 drawTile(u08 r, u08 c, u08 face) {
   u08 ca=co+c;
   u08 ra=ro+r;
   if (ca>31 || ra>23 || face>6) return ERR;
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
   //if (rot>0 && face>=1 && face<=4) {
   //   face=face+rot;
   //   if (face>4) face=face-4;
   //}
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
} // drawTile(u08 r, u08 c, u08 face)

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
         if (v || !hide) drawTile(r, c, shape[depth][r][c]);
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

// rotate 'shape[]' on 'mov' (use 'shaps[]' as tmp)
u08 rotShape(u08 mov) {
   if (mov>3) return ERR;
   //printf(" shapeSide:%u shapeRot:%u mov:%u\n", shapeSide, shapeRot, mov);
   blankShape(0); // target shaps[]
   u08 face;
   for (u08 d=0; d<sizeM; d++) {
      for (u08 r=0; r<sizeM; r++) {
         for (u08 c=0; c<sizeM; c++) {
            switch (mov) { // shape[d][r][c]
            case 0: // ok
               //face = shape[d][sizeM-1-r][c];
               //printf("face:%u calcTileFace():%u\n", face, calcTileFace(shape[d][sizeM-1-r][c], mov));
               face = calcTileFace(shape[d][sizeM-1-r][c], mov);
               shaps[r][d][c] = face;
               break;
            case 1: // ok
               face = calcTileFace(shape[d][r][c], mov);
               shaps[c][r][sizeM-1-d] = face;
               break;
            case 2: // ok
               face = calcTileFace(shape[d][r][c], mov);
               shaps[r][sizeM-1-d][c] = face;
               break;
            case 3: // 
               face = calcTileFace(shape[d][r][sizeM-1-c], mov);
               shaps[c][r][d] = face;
            } // switch (mov)
         } // inner for
      } // middle for
   } // outer for
   for (u16 b=0; b<sizeof(shape); b++) { // copy back to shape[]
      *(shape+b)=*(shaps+b);
   }
   return OK;
} // u08 rotShape(u08 mov)

// return 'face' of tile at r,c with origin ro,co
u08 getShapeFace(u08 r, u08 c) {
   if (co>31 || ro>23) return ERR;
   u08 face;
   u08 d;
   for (d=0; d<sizeM; d++) {
      face=shape[d][r][c];
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
      face=shape[d][r][c];
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
   u08 face;
   u08 d;
   switch (tileFace) {
   case 0:
      ret=WARN;
      tileMov=0;
      break;
   case 1:
      for (s=r-1; s>=-1; s--) {
         face=shape[tileDepth][s][c]; // up one tile
         if(s==-1) face=0; // below s=0
         //printf(" s:%d face:%u\n", s, face);
         if (face==0) { // free pos
            //printf(" move up\n");
            shape[tileDepth][s+1][c]=0;
            d=getShapeDepth(s+1, c); // current pos
            if(d>tileDepth) { // we are fore layers
               delTile(s+1, c);
               //printf(" restore back\n");
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(s+1, c, shape[d][s+1][c]);
               dtby=0;
            }
            if(s==-1) break; // break for, below s=0
            d=getShapeDepth(s, c); // target pos
            shape[tileDepth][s][c]=tileFace;
            if (d>tileDepth) { // only back layers
               delTile(s, c);
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(s, c, tileFace);
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
                  delTile(b, c);
                  if (dlby==1 && tileDepth>0) dtby=1;
                  drawTile(b, c, shape[d][b][c]);
                  dtby=0;
               }
               d=getShapeDepth(b+1, c); // target pos
               shape[tileDepth][b+1][c]=tileFace;
               if (d>tileDepth) { // only back layers
                  delTile(b+1, c);
                  //printf(" restore back\n");
                  if (dlby==1 && tileDepth>0) dtby=1;
                  drawTile(b+1, c, tileFace);
                  dtby=0;
               } // else printf(" we are back\n");
               pause(WAIT);
            }
            break;
         }
      } // for
      if (ret==OK) {
         //delTile(s+1, c); // remove tile out of shape
         //shape[tileDepth][s+1][c]=0; // remove tile from shape
      }
      break;
   case 2:
      for (s=c+1; s<=sizeM; s++) {
         face=shape[tileDepth][r][s]; // to the right
         //printf(" s:%d face:%u\n", s, face);
         if (face==0) { // free pos
            //printf(" move right\n");
            shape[tileDepth][r][s-1]=0;
            d=getShapeDepth(r, s-1); // current pos
            if(d>tileDepth) { // we are fore layers
               delTile(r, s-1);
               //printf(" restore back\n");
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(r, s-1, shape[d][r][s-1]);
               dtby=0;
            }
            if(s==sizeM) break; // break for, reached sizeM
            d=getShapeDepth(r, s); // target pos
            shape[tileDepth][r][s]=tileFace;
            if (d>tileDepth) { // only back layers
               delTile(r, s);
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(r, s, tileFace);
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
                  delTile(r, b);
                  if (dlby==1 && tileDepth>0) dtby=1;
                  drawTile(r, b, shape[d][r][b]);
                  dtby=0;
               }
               d=getShapeDepth(r, b-1); // target pos
               shape[tileDepth][r][b-1]=tileFace;
               if (d>tileDepth) { // only back layers
                  delTile(r, b-1);
                  //printf(" restore back\n");
                  if (dlby==1 && tileDepth>0) dtby=1;
                  drawTile(r, b-1, tileFace);
                  dtby=0;
               } // else printf(" we are back\n");
               pause(WAIT);
            }
            break;
         }
      } // for
      if (ret==OK) {
         //printf(" s:%u final del tile\n", s);
         delTile(r, s); // remove tile out of shape
         if (s<sizeM) shape[tileDepth][r][s]=0; // remove tile from shape
      }
      break;
   case 3:
      for (s=r+1; s<=sizeM; s++) {
         face=shape[tileDepth][s][c]; // down one tile
         //printf(" s:%d face:%u\n", s, face);
         if (face==0) { // free pos
            //printf(" move down\n");
            shape[tileDepth][s-1][c]=0;
            d=getShapeDepth(s-1, c); // current pos
            if(d>tileDepth) { // we are fore layers
               delTile(s-1, c);
               //printf(" restore back\n");
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(s-1, c, shape[d][s-1][c]);
               dtby=0;
            }
            if(s==sizeM) break; // break for, reached sizeM
            d=getShapeDepth(s, c); // target pos
            shape[tileDepth][s][c]=tileFace;
            if (d>tileDepth) { // only back layers
               delTile(s, c);
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(s, c, tileFace);
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
                  delTile(b, c);
                  if (dlby==1 && tileDepth>0) dtby=1;
                  drawTile(b, c, shape[d][b][c]);
                  dtby=0;
               }
               d=getShapeDepth(b-1, c); // target pos
               shape[tileDepth][b-1][c]=tileFace;
               if (d>tileDepth) { // only back layers
                  delTile(b-1, c);
                  //printf(" restore back\n");
                  if (dlby==1 && tileDepth>0) dtby=1;
                  drawTile(b-1, c, tileFace);
                  dtby=0;
               } // else printf(" we are back\n");
               pause(WAIT);
            }
            break;
         }
      } // for
      if (ret==OK) {
         delTile(s, c); // remove tile out of shape
         if (s<sizeM) shape[tileDepth][s][c]=0; // remove tile from shape
      }
      break;
   case 4:
      for (s=c-1; s>=-1; s--) {
         face=shape[tileDepth][r][s]; // to the left
         if(s==-1) face=0; // below s=0
         //printf(" s:%d face:%u\n", s, face);
         if (face==0) { // free pos
            //printf(" move left\n");
            shape[tileDepth][r][s+1]=0;
            d=getShapeDepth(r, s+1); // current pos
            if(d>tileDepth) { // we are fore layers
               delTile(r, s+1);
               //printf(" restore back\n");
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(r, s+1, shape[d][r][s+1]);
               dtby=0;
            }
            if(s==-1) break; // break for, below s=0
            d=getShapeDepth(r, s); // target pos
            shape[tileDepth][r][s]=tileFace;
            if (d>tileDepth) { // only back layers
               delTile(r, s);
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(r, s, tileFace);
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
                  delTile(r, b);
                  //printf(" restore back\n");
                  if (dlby==1 && tileDepth>0) dtby=1;
                  drawTile(r, b, shape[d][r][b]);
                  dtby=0;
               }
               d=getShapeDepth(r, b+1); // target pos
               shape[tileDepth][r][b+1]=tileFace;
               if (d>tileDepth) { // only back layers
                  delTile(r, b+1);
                  if (dlby==1 && tileDepth>0) dtby=1;
                  drawTile(r, b+1, tileFace);
                  dtby=0;
               } // else printf(" we are back\n");
               pause(WAIT);
            }
            break;
         }
      } // for
      if (ret==OK) {
         //delTile(r, s+1); // remove tile out of shape
         //shape[tileDepth][r][s+1]=0; // remove tile from shape
      }
      break;
   case 5:
      for (s=tileDepth+1; s<sizeM; s++) {
         shape[s-1][r][c]=0; // remove current tile
         face=shape[s][r][c]; // check below
         //printf(" s:%d face:%u\n", s, face);
         shape[s-1][r][c]=tileFace; // restore
         if (face==0) { // free pos
            //printf(" move below\n");
            shape[s-1][r][c]=0; // remove current tile
            shape[s][r][c]=tileFace;
            delTile(r, c);
            if (dlby==1 && s>0) dtby=1;
            drawTile(r, c, tileFace);
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
               if (dlby==1 && tileDepth>0) dtby=1;
               drawTile(r, c, tileFace);
               dtby=0;
               pause(WAIT);
            }
            break;
         }
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
            if (dlby==1 && s>0) dtby=1;
            drawTile(r, c, tileFace);
            dtby=0;
            tileMov=tileDepth-s;
            pause(WAIT);
         }
      } // for
      delTile(r, c); // remove tile out of shape
      face=getShapeFace(r, c); // tile below
      drawTile(r, c, face);
   } // switch (tileFace)
   return ret;
} // u08 unblockTile(u08 r, u08 c)

// print presentation and instructions
void menu() {
   clg(); // clear
   textcolor(textColor[BLK]);
   printf("\n Unblock cubic vector tiles for ZX Spectrum 48k\n");
   printf(" Copyright 2024 V. Messina\n");
   printf("\n");
   printf(" All blocks moves in arrow direction:\n");
   printf(" Up    block\n");
   co=0; ro=0;
   drawTile( 5, 10, 1);
   textcolor(textColor[BLK]);
   printf(" Right block\n");
   drawTile( 6, 10, 2);
   textcolor(textColor[BLK]);
   printf(" Down  block\n");
   drawTile( 7, 10, 3);
   textcolor(textColor[BLK]);
   printf(" Left  block\n");
   drawTile( 8, 10, 4);
   textcolor(textColor[BLK]);
   printf(" In    block\n");
   drawTile( 9, 10, 5);
   textcolor(textColor[BLK]);
   printf(" Out   block\n");
   drawTile(10, 10, 6);
   printf(" Blocks will be blocked by other blocks on arrow path\n\n");
   printf(" Use IJKL to select a block\n");
   printf(" Use WASD to rotate shape\n");
   printf(" Use SPACE to try unblock selected block\n");
   printf("\n SPACE to start\n");
   drawb(0,0,256,192);
   while (1) {
      readKeys();
      if (keySPdown()) break;
   }
} // menu()

void main() {
   co=26; ro=6; // working shape origin
   init(); // take care to set co,ro of working shape before call
   //printf(" sizeM:%u\n", sizeM);
   menu();
   clg(); // clear
   textcolor(textColor[BLK]);
   printf("\x1B\x59\x36\x20"); // ESC Y r c: cursor r,c (add 0x20=32)
   printf(" Unblock cubic vector tiles for ZX Spectrum 48k\n");
   printf("\x1B\x59\x20\x20"); // ESC Y r c: cursor r,c (add 0x20=32)
   printf("\n");
   printf(" Shape sized:%ux%ux%u\n", sizeM, sizeM, sizeM);
   printf(" blks:%u/%u\n", cblks, iblks);
   //printf("                            Layer 0     Layer 1     Layer 2\n");
   drawb(0,0,256,192);

   ro=1;
   co=14; drawShapeLayer(0, 0);
   co=20; drawShapeLayer(1, 0);
   co=26; drawShapeLayer(2, 0);
   ro=6; co=20;
   drawShapeSide();
   //co=26; ro=1;
   //drawShapeSide();
   //drawShapeLayer(0, 0);
   co=26; ro=6; // working shape origin
   drawShapeSide();
   //drawShapeLayer(0, 0);

   u08 c=0, r=0;
   textcolor(textColor[BLK]);
   drawb((co+c)*8+2, cy, 4, 4); // horizontal cursor
   drawb(cx, (ro+r)*8+2, 4, 4); // vertical   cursor

   while (1) {
      readKeys();
      u08 ret;
      u08 mov;
      if (keyWdown()) {
         //printf(" w up\n");
         clga((co+c)*8+2, cy, 4, 4); // horizontal cursor
         clga(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
         mov=0;
         rotShape(mov);
         clga(ax, ay, sw, sh); // delete shape at ro[23:0],co[31:0]
         drawShapeSide();
         textcolor(textColor[BLK]);
         drawb((co+c)*8+2, cy, 4, 4); // horizontal cursor
         drawb(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
      }
      if (keyDdown()) {
         //printf(" d right\n");
         clga((co+c)*8+2, cy, 4, 4); // horizontal cursor
         clga(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
         mov=1;
         rotShape(mov);
         clga(ax, ay, sw, sh); // delete shape at ro[23:0],co[31:0]
         drawShapeSide();
         textcolor(textColor[BLK]);
         drawb((co+c)*8+2, cy, 4, 4); // horizontal cursor
         drawb(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
      }
      if (keySdown()) {
         //printf(" s down\n");
         clga((co+c)*8+2, cy, 4, 4); // horizontal cursor
         clga(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
         mov=2;
         rotShape(mov);
         clga(ax, ay, sw, sh); // delete shape at ro[23:0],co[31:0]
         drawShapeSide();
         textcolor(textColor[BLK]);
         drawb((co+c)*8+2, cy, 4, 4); // horizontal cursor
         drawb(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
      }
      if (keyAdown()) {
         //printf(" a left\n");
         clga((co+c)*8+2, cy, 4, 4); // horizontal cursor
         clga(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
         mov=3;
         rotShape(mov);
         clga(ax, ay, sw, sh); // delete shape at ro[23:0],co[31:0]
         drawShapeSide();
         textcolor(textColor[BLK]);
         drawb((co+c)*8+2, cy, 4, 4); // horizontal cursor
         drawb(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
      }
      // block selection
      if (keyIdown()) {
         //printf("i");
         if(r>0) {
            clga(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
            r--;
            textcolor(textColor[BLK]);
            drawb(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
         }
      }
      if (keyJdown()) {
         //printf("j");
         if(c>0) {
            clga((co+c)*8+2, cy, 4, 4); // horizontal cursor
            c--;
            textcolor(textColor[BLK]);
            drawb((co+c)*8+2, cy, 4, 4); // horizontal cursor
         }
      }
      if (keyKdown()) {
         //printf("k");
         if(r<sizeM) {
            clga(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
            r++;
            textcolor(textColor[BLK]);
            drawb(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
         }
      }
      if (keyLdown()) {
         //printf("l");
         if(c<sizeM) {
            clga((co+c)*8+2, cy, 4, 4); // horizontal cursor
            c++;
            textcolor(textColor[BLK]);
            drawb((co+c)*8+2, cy, 4, 4); // horizontal cursor
         }
      }
      if (keySPdown()) {
         //printf("SP ");
         //printf("r:%u c:%u\n", r, c);
         clga((co+c)*8+2, cy, 4, 4); // horizontal cursor
         clga(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
         ret=unblockTile(r, c);
         if (ret==0) cblks--;
         textcolor(textColor[BLK]);
         drawb((co+c)*8+2, cy, 4, 4); // horizontal cursor
         drawb(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
         clga( 0, 16, 64, 8);
         printf("\x1B\x59\x22\x20"); // ESC Y r c: cursor r,c (add 0x20=32)
         printf(" blks:%u/%u\n", cblks, iblks);
      }
      if (keyENdown()) {
         //printf("EN ");
         //printf("r:%u c:%u\n", r, c);
         clga((co+c)*8+2, cy, 4, 4); // horizontal cursor
         clga(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
         ret=getShapeDepth(r, c);
         shape[ret][r][c]=0;
         delTile(r, c);
         if (ret!=0) cblks--;
         ret=getShapeDepth(r, c);// restore back
         if(ret<sizeM) drawTile(r, c, shape[ret][r][c]);
         textcolor(textColor[BLK]);
         drawb((co+c)*8+2, cy, 4, 4); // horizontal cursor
         drawb(cx, (ro+r)*8+2, 4, 4); // vertical   cursor
         clga( 0, 16, 64, 8);
         printf("\x1B\x59\x22\x20"); // ESC Y r c: cursor r,c (add 0x20=32)
         printf(" blks:%u/%u\n", cblks, iblks);
      }
      pause(132);
      if (cblks==0) {
         printf(" you win!\n");
         while(1);
      } 
   } // while(1)
} // main()
