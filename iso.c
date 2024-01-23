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
                           {{1,1,1,1,1,1,1,1,1,1,1}, // 4>
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

u08 fillBlk(u08 face) {
   switch(face) {
   case 0: // 
      for (u08 y=0; y<SIZE; y++) {
         for (u08 x=0; x<SIZE; x++) {
            block[0][y][x]=faces[0][y][x]; // right face
            block[x][y][0]=faces[0][y][x]; // left  face
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

void iso() {
   clg(); // clear
   drawb(0,0,256,192);
   for (u08 f=0; f<7; f++) {
      fillBlk(f);
      for (u08 z=0; z<SIZE; z++) {
         for (u08 y=0; y<SIZE; y++) {
            for (u08 x=0; x<SIZE; x++) {
               if(z>0 && y>0 && x>0) break; // skip hidden lines
               u08 px=block[z][y][x];
               if(px==1) {
                  //printf("z:%u y:%u x:%u px:%u\n", z, y, x, px);
                  u08 x2 = SIZE -  z + x;          // isometric projection
                  u08 y2 = SIZE - (z + x) / 2 + y; // isometric projection
                  x2=x2+60+f*20;
                  y2=y2+90;
                  plot(x2, y2);
               }
            }
         }
      }
   }
   while(1);
} // void iso()
