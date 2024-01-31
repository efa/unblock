/* Unblock v0.00.00a 2024/01/23 Unblock cubic tiles for ZX Spectrum         */
/* Copyright 2023-2024 Valerio Messina http://users.iol.it/efa              */
/* iso.c is part of Unblock
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

#define SIZE 11

u08 faces[7][SIZE][SIZE]= {{{1,1,1,1,1,1,1,1,1,1,1}, // 0
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,1,1,1,1,1,1,1,1,1,1}},
                           {{1,1,1,1,1,1,1,1,1,1,1}, // 1^
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,1,0,0,0,0,1},
                            {1,0,0,0,1,1,1,0,0,0,1},
                            {1,0,0,1,0,1,0,1,0,0,1},
                            {1,0,1,0,0,1,0,0,1,0,1},
                            {1,1,0,0,0,1,0,0,0,1,1},
                            {1,0,0,0,0,1,0,0,0,0,1},
                            {1,0,0,0,0,1,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,1,1,1,1,1,1,1,1,1,1}},
                           {{1,1,1,1,1,1,1,1,1,1,1}, // 2>
                            {1,0,0,0,1,0,0,0,0,0,1},
                            {1,0,0,0,0,1,0,0,0,0,1},
                            {1,0,0,0,0,0,1,0,0,0,1},
                            {1,0,0,0,0,0,0,1,0,0,1},
                            {1,0,1,1,1,1,1,1,1,0,1},
                            {1,0,0,0,0,0,0,1,0,0,1},
                            {1,0,0,0,0,0,1,0,0,0,1},
                            {1,0,0,0,0,1,0,0,0,0,1},
                            {1,0,0,0,1,0,0,0,0,0,1},
                            {1,1,1,1,1,1,1,1,1,1,1}},
                           {{1,1,1,1,1,1,1,1,1,1,1}, // 3v
                            {1,0,0,0,0,0,0,0,0,0,1}, 
                            {1,0,0,0,0,1,0,0,0,0,1},
                            {1,0,0,0,0,1,0,0,0,0,1},
                            {1,1,0,0,0,1,0,0,0,1,1},
                            {1,0,1,0,0,1,0,0,1,0,1},
                            {1,0,0,1,0,1,0,1,0,0,1},
                            {1,0,0,0,1,1,1,0,0,0,1},
                            {1,0,0,0,0,1,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,1,1,1,1,1,1,1,1,1,1}},
                           {{1,1,1,1,1,1,1,1,1,1,1}, // 4<
                            {1,0,0,0,0,0,1,0,0,0,1},
                            {1,0,0,0,0,1,0,0,0,0,1},
                            {1,0,0,0,1,0,0,0,0,0,1},
                            {1,0,0,1,0,0,0,0,0,0,1},
                            {1,0,1,1,1,1,1,1,1,0,1},
                            {1,0,0,1,0,0,0,0,0,0,1},
                            {1,0,0,0,1,0,0,0,0,0,1},
                            {1,0,0,0,0,1,0,0,0,0,1},
                            {1,0,0,0,0,0,1,0,0,0,1},
                            {1,1,1,1,1,1,1,1,1,1,1}},
                           {{1,1,1,1,1,1,1,1,1,1,1}, // 5x
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,1,0,0,0,0,0,1,0,1},
                            {1,0,0,1,0,0,0,1,0,0,1},
                            {1,0,0,0,1,0,1,0,0,0,1},
                            {1,0,0,0,0,1,0,0,0,0,1},
                            {1,0,0,0,1,0,1,0,0,0,1},
                            {1,0,0,1,0,0,0,1,0,0,1},
                            {1,0,1,0,0,0,0,0,1,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,1,1,1,1,1,1,1,1,1,1}},
                           {{1,1,1,1,1,1,1,1,1,1,1}, // 6.
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,1,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,0,0,0,0,0,0,0,0,0,1},
                            {1,1,1,1,1,1,1,1,1,1,1}}};

u08 block[SIZE][SIZE][SIZE]; // block[z][y][x]

// fill block[] with (right) face
u08 fillBlk(u08 face) {
   switch(face) {
   case 0: // 
      for (u08 y=0; y<SIZE; y++) {
         for (u08 x=0; x<SIZE; x++) {
            block[0][y][x]=faces[0][y][x]; // right face
            block[y][x][0]=faces[0][y][x]; // left  face
            block[y][0][x]=faces[0][y][x]; // top   face
         }
      }
      break;
   case 1: // ok
      for (u08 y=0; y<SIZE; y++) {
         for (u08 x=0; x<SIZE; x++) {
            block[0][y][x]=faces[1][y][x]; // right face
            block[x][y][0]=faces[1][y][x]; // left  face
            block[y][0][x]=faces[6][y][x]; // top   face
         }
      }
      break;
   case 2: // ok
      for (u08 y=0; y<SIZE; y++) {
         for (u08 x=0; x<SIZE; x++) {
            block[0][y][x]=faces[2][y][x]; // right face
            block[y][x][0]=faces[5][y][x]; // left  face
            block[y][0][x]=faces[2][y][x]; // top   face
         }
      }
      break;
   case 3: // ok
      for (u08 y=0; y<SIZE; y++) {
         for (u08 x=0; x<SIZE; x++) {
            block[0][y][x]=faces[3][y][x]; // right face
            block[x][y][0]=faces[3][y][x]; // left  face
            block[y][0][x]=faces[5][y][x]; // top   face
         }
      }
      break;
   case 4: // ok
      for (u08 y=0; y<SIZE; y++) {
         for (u08 x=0; x<SIZE; x++) {
            block[0][y][x]=faces[4][y][x]; // right face
            block[y][x][0]=faces[6][y][x]; // left  face
            block[y][0][x]=faces[4][y][x]; // top   face
         }
      }
      break;
   case 5: // ok
      for (u08 y=0; y<SIZE; y++) {
         for (u08 x=0; x<SIZE; x++) {
            block[0][y][x]=faces[5][y][x]; // right face
            block[y][x][0]=faces[3][y][x]; // left  face
            block[y][0][x]=faces[3][y][x]; // top   face
         }
      }
      break;
   case 6: // ok
      for (u08 y=0; y<SIZE; y++) {
         for (u08 x=0; x<SIZE; x++) {
            block[0][y][x]=faces[6][y][x]; // right face
            block[y][x][0]=faces[1][y][x]; // left  face
            block[y][0][x]=faces[1][y][x]; // top   face
         }
      }
   } // switch(face)
} // u08 fillBlk(u08 face)

// draw at ax,ay a block half face in isometric projection
void drawHalfFace(u08 face, u08 hFace) {
   if (face > 6 || hFace > 5) return;
   fillBlk(face);
   for (u08 z=0; z<SIZE; z++) {
      for (u08 y=0; y<SIZE; y++) {
         for (u08 x=0; x<SIZE; x++) {
            if(z>0 && y>0 && x>0) break; // skip hidden lines
            u08 px=block[z][y][x];
            if(px==1) {
               //printf("z:%u y:%u x:%u px:%u\n", z, y, x, px);
               switch (hFace) {
               case 0:
                  if (z>0 || x>y) goto skip; // skip to next pixel
                  break;
               case 1:
                  if (z>0 || y>x) goto skip; // skip to next pixel
                  break;
               case 2:
                  if (y>0 || z>x) goto skip; // skip to next pixel
                  break;
               case 3:
                  if (y>0 || x>z) goto skip; // skip to next pixel
                  break;
               case 4:
                  if (x>0 || y>z) goto skip; // skip to next pixel
                  break;
               case 5:
                  if (x>0 || z>y) goto skip; // skip to next pixel
               }
               u08 x2 = SIZE -  z + x;          // isometric projection
               u08 y2 = SIZE - (z + x) / 2 + y; // isometric projection
               x2=x2+ax;
               y2=y2+ay;
               plot(x2, y2);
               skip:
            }
         }
      }
   }
} // void drawHalfFace(u08 face, u08 hFace)

/* hFace
   /|\   
  /3|2\  
 |\ | /| 
 |4\|/1| 
 | /|\ | 
 |/5|0\| 
  \ | /  
   \|/   */

// draw at ax,ay a block half face in isometric projection
void drawHalfFace2(u08 face, u08 hFace) {
   if (face > 6 || hFace > 5) return;
   switch (face) {
   case 0:
      switch (hFace) {
      case 0:
         draw(ax+20, ay+15, ax+10, ay+20);
         drawr(0, -10);
         break;
      case 1:
         draw(ax+20, ay+15, ax+20, ay+5);
         drawr(-10, 5);
         break;
      case 2:
         draw(ax+10, ay, ax+20, ay+5);
         drawr(-10, 5);
         break;
      case 3:
         draw(ax+10, ay, ax, ay+5);
         drawr(10, 5);
         break;
      case 4:
         draw(ax, ay+15, ax, ay+5);
         drawr(10, 5);
         break;
      case 5:
         draw(ax, ay+15, ax+10, ay+20);
         drawr(0, -10);
      } // switch (hFace)
      break;
   case 1:
      switch (hFace) {
      case 0:
         draw(ax+20, ay+15, ax+10, ay+20);
         drawr(0, -10);
         draw(ax+11, ay+16, ax+13, ay+12);
         draw(ax+15, ay+13, ax+15, ay+16);
         break;
      case 1:
         draw(ax+20, ay+15, ax+20, ay+5);
         drawr(-10, 5);
         plot(ax+14,ay+11);
         draw(ax+15, ay+13, ax+15, ay+10);
         draw(ax+15, ay+10, ax+19, ay+12);
         break;
      case 2:
         draw(ax+10, ay, ax+20, ay+5);
         drawr(-10, 5);
         plot(ax+10,ay+5);
         break;
      case 3:
         draw(ax+10, ay, ax, ay+5);
         drawr(10, 5);
         plot(ax+10,ay+5);
         break;
      case 4:
         draw(ax, ay+15, ax, ay+5);
         drawr(10, 5);
         plot(ax+6,ay+11);
         draw(ax+1, ay+12, ax+4, ay+10);
         draw(ax+5, ay+10, ax+5, ay+13);
         break;
      case 5:
         draw(ax, ay+15, ax+10, ay+20);
         drawr(0, -10);
         draw(ax+5, ay+13, ax+5, ay+16);
         draw(ax+7, ay+12, ax+9, ay+16);
      } // switch (hFace)
      break;
   case 2:
      switch (hFace) {
      case 0:
         draw(ax+20, ay+15, ax+10, ay+20);
         drawr(0, -10);
         draw(ax+12, ay+14, ax+15, ay+12);
         draw(ax+14, ay+17, ax+16, ay+14);
         break;
      case 1:
         draw(ax+20, ay+15, ax+20, ay+5);
         drawr(-10, 5);
         draw(ax+14, ay+9, ax+18, ay+11);
         draw(ax+15, ay+13, ax+17, ay+11);
         draw(ax+17, ay+12, ax+17, ay+13);
         break;
      case 2:
         draw(ax+10, ay, ax+20, ay+5);
         drawr(-10, 5);
         plot(ax+11,ay+4);
         draw(ax+10, ay+5, ax+13, ay+4);
         draw(ax+13, ay+5, ax+13, ay+8);
         break;
      case 3:
         draw(ax+10, ay, ax, ay+5);
         drawr(10, 5);
         draw(ax+5, ay+4, ax+10, ay+4);
         draw(ax+7, ay+7, ax+10, ay+6);
         break;
      case 4:
         draw(ax, ay+15, ax, ay+5);
         drawr(10, 5);
         draw(ax+2, ay+14, ax+8, ay+11);
         draw(ax+2, ay+8, ax+5, ay+13);
         break;
      case 5:
         draw(ax, ay+15, ax+10, ay+20);
         drawr(0, -10);
         draw(ax+2, ay+14, ax+8, ay+11);
         draw(ax+8, ay+17, ax+5, ay+13);
      } // switch (hFace)
      break;
   case 3:
      switch (hFace) {
      case 0:
         draw(ax+20, ay+15, ax+10, ay+20);
         drawr(0, -10);
         plot(ax+16,ay+14);
         draw(ax+11, ay+14, ax+15, ay+16);
         draw(ax+15, ay+13, ax+15, ay+15);
         break;
      case 1:
         draw(ax+20, ay+15, ax+20, ay+5);
         drawr(-10, 5);
         draw(ax+15, ay+10, ax+15, ay+13);
         draw(ax+17, ay+13, ax+19, ay+10);
         break;
      case 2:
         draw(ax+10, ay, ax+20, ay+5);
         drawr(-10, 5);
         draw(ax+10, ay+2, ax+10, ay+8);
         draw(ax+11, ay+5, ax+16, ay+5);
         break;
      case 3:
         draw(ax+10, ay, ax, ay+5);
         drawr(10, 5);
         draw(ax+10, ay+2, ax+10, ay+8);
         draw(ax+4, ay+5, ax+9, ay+5);
         break;
      case 4:
         draw(ax, ay+15, ax, ay+5);
         drawr(10, 5);
         draw(ax+5, ay+10, ax+5, ay+13);
         draw(ax+1, ay+10, ax+3, ay+13);
        break;
      case 5:
         draw(ax, ay+15, ax+10, ay+20);
         drawr(0, -10);
         plot(ax+4,ay+14);
         draw(ax+5, ay+13, ax+5, ay+16);
         draw(ax+6, ay+15, ax+9, ay+14);
      } // switch (hFace)
      break;
   case 4:
      switch (hFace) {
      case 0:
         draw(ax+20, ay+15, ax+10, ay+20);
         drawr(0, -10);
         draw(ax+16, ay+16, ax+12, ay+14);
         draw(ax+13, ay+13, ax+15, ay+13);
         break;
      case 1:
         draw(ax+20, ay+15, ax+20, ay+5);
         drawr(-10, 5);
         draw(ax+15, ay+13, ax+18, ay+11);
         draw(ax+13, ay+12, ax+16, ay+9);
         break;
      case 2:
         draw(ax+10, ay, ax+20, ay+5);
         drawr(-10, 5);
         draw(ax+10, ay+5, ax+13, ay+4);
         draw(ax+10, ay+7, ax+13, ay+7);
         break;
      case 3:
         draw(ax+10, ay, ax, ay+5);
         drawr(10, 5);
         plot(ax+9,ay+7);
         draw(ax+7, ay+3, ax+7, ay+7);
         draw(ax+8, ay+7, ax+10, ay+5);
         break;
      case 4:
         draw(ax, ay+15, ax, ay+5);
         drawr(10, 5);
         plot(ax+5,ay+13);
         break;
      case 5:
         draw(ax, ay+15, ax+10, ay+20);
         drawr(0, -10);
         plot(ax+5,ay+13);
      } // switch (hFace)
      break;
   case 5:
      switch (hFace) {
      case 0:
         draw(ax+20, ay+15, ax+10, ay+20);
         drawr(0, -10);
         draw(ax+12, ay+17, ax+15, ay+13);
         draw(ax+16, ay+13, ax+18, ay+8);
         break;
      case 1:
         draw(ax+20, ay+15, ax+20, ay+5);
         drawr(-10, 5);
         draw(ax+12, ay+11, ax+18, ay+14);
         draw(ax+12, ay+11, ax+18, ay+14);
         break;
      case 2:
         draw(ax+10, ay, ax+20, ay+5);
         drawr(-10, 5);
         draw(ax+10, ay+6, ax+13, ay+7);
         draw(ax+10, ay+4, ax+15, ay+4);
         break;
      case 3:
         draw(ax+10, ay, ax, ay+5);
         drawr(10, 5);
         draw(ax+7, ay+4, ax+7, ay+8);
         draw(ax+7, ay+4, ax+10, ay+4);
         draw(ax+7, ay+4, ax+9, ay+5);
         break;
      case 4:
         draw(ax, ay+15, ax, ay+5);
         drawr(10, 5);
         draw(ax+3, ay+11, ax+5, ay+13);
         draw(ax+3, ay+11, ax+6, ay+9);
         draw(ax+2, ay+11, ax+3, ay+13);
         break;
      case 5:
         draw(ax, ay+15, ax+10, ay+20);
         drawr(0, -10);
         draw(ax+5, ay+13, ax+8, ay+14);
         draw(ax+4, ay+14, ax+6, ay+17);
      } // switch (hFace)
      break;
   case 6:
      switch (hFace) {
      case 0:
         draw(ax+20, ay+15, ax+10, ay+20);
         drawr(0, -10);
         plot(ax+15,ay+13);
         break;
      case 1:
         draw(ax+20, ay+15, ax+20, ay+5);
         drawr(-10, 5);
         plot(ax+15,ay+13);
         break;
      case 2:
         draw(ax+10, ay, ax+20, ay+5);
         drawr(-10, 5);
         draw(ax+13, ay+7, ax+13, ay+3);
         draw(ax+13, ay+7, ax+10, ay+5);
         draw(ax+13, ay+7, ax+10, ay+7);
         break;
      case 3:
         draw(ax+10, ay, ax, ay+5);
         drawr(10, 5);
         draw(ax+10, ay+7, ax+5, ay+7);
         draw(ax+7, ay+4, ax+10, ay+5);
         break;
      case 4:
         draw(ax, ay+15, ax, ay+5);
         drawr(10, 5);
         draw(ax+2, ay+11, ax+5, ay+13);
         draw(ax+4, ay+9, ax+7, ay+12);
         break;
      case 5:
         draw(ax, ay+15, ax+10, ay+20);
         drawr(0, -10);
         plot(ax+7,ay+13);
         draw(ax+4, ay+16, ax+8, ay+14);
         draw(ax+5, ay+13, ax+8, ay+14);
      } // switch (hFace)
   } // switch (face)
} // void drawHalfFace(u08 face, u08 hFace)

/* hFace
   /|\   
  /3|2\  
 |\ | /| 
 |4\|/1| 
 | /|\ | 
 |/5|0\| 
  \ | /  
   \|/   */

// draw 7 blocks with all faces
void iso() {
   clg(); // clear
   drawb(0,0,256,192);
//#if 0
   ax=20; // pixel absolute shape position x
   ay=54; // pixel absolute shape position y
   for (u08 f=0; f<7; f++) {
      ax=ax+24;
      for (u08 hf=0; hf<6; hf++) {
         drawHalfFace(f, hf);
      }
   } // for face
   ax=20; // pixel absolute shape position x
   ay=78; // pixel absolute shape position y
   for (u08 f=0; f<7; f++) {
      ax=ax+24;
      for (u08 hf=0; hf<6; hf++) {
         if (hf==0) { ax=ax+2; ay=ay+6; }
         if (hf==1) {          ay=ay-2; }
         if (hf==2) {          ay=ay-2; }
         if (hf==3) { ax=ax-2;          }
         if (hf==4) {          ay=ay+2; }
         if (hf==5) {          ay=ay+2; }
         drawHalfFace(f, hf);
      }
      ay=ay-6;
   } // for face
   ax=21; // pixel absolute shape position x
   ay=110; // pixel absolute shape position y
   for (u08 f=0; f<7; f++) {
      ax=ax+24;
      for (u08 hf=0; hf<6; hf++) {
         if (hf==0) { ax=ax+2; ay=ay+6; }
         if (hf==1) {          ay=ay-2; }
         if (hf==2) {          ay=ay-2; }
         if (hf==3) { ax=ax-2;          }
         if (hf==4) {          ay=ay+2; }
         if (hf==5) {          ay=ay+2; }
         drawHalfFace2(f, hf);
      }
      ay=ay-6;
   } // for face
//#endif
   ax=21; // pixel absolute shape position x
   ay=140; // pixel absolute shape position y
   for (u08 f=0; f<7; f++) {
      ax=ax+24;
      for (u08 hf=0; hf<6; hf++) {
         drawHalfFace2(f, hf);
      }
   } // for face
   while(1);
} // void iso()
