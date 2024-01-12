/* Unblock v0.00.00a 2024/01/12 Unblock cubic tiles for ZX Spectrum         */
/* Copyright 2023-2024 Valerio Messina http://users.iol.it/efa              */
/* keys.c is part of Unblock
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

extern u08 keyVCXZc;
extern u08 keyGFDSA;
extern u08 keyTREWQ;
extern u08 key54321;
extern u08 key67890;
extern u08 keyYUIOP;
extern u08 keyHJKLe;
extern u08 keyBNMys;

#asm
._keyVCXZc
defb 0
._keyGFDSA
defb 0
._keyTREWQ
defb 0
._key54321
defb 0
._key67890
defb 0
._keyYUIOP
defb 0
._keyHJKLe
defb 0
._keyBNMys
defb 0
#endasm

void readKeys() {
   __asm
      PUSH BC

      LD   C, $FE
      LD   B, $FE ; VCXZc
      IN   A, (C)
      NEG
      DEC  A
      AND  A, $1F
      LD  (_keyVCXZc), A

      LD   C, $FE
      LD   B, $FD ; GFDSA
      IN   A, (C)
      NEG
      DEC  A
      AND  A, $1F
      LD  (_keyGFDSA), A

      LD   C, $FE
      LD   B, $FB ; TREWQ
      IN   A, (C)
      NEG
      DEC  A
      AND  A, $1F
      LD  (_keyTREWQ), A

      LD   C, $FE
      LD   B, $F7 ; 54321
      IN   A, (C)
      NEG
      DEC  A
      AND  A, $1F
      LD  (_key54321), A

      LD   C, $FE
      LD   B, $EF ; 67890
      IN   A, (C)
      NEG
      DEC  A
      AND  A, $1F
      LD  (_key67890), A

      LD   C, $FE
      LD   B, $DF ; YUIOP
      IN   A, (C)
      NEG
      DEC  A
      AND  A, $1F
      LD  (_keyYUIOP), A

      LD   C, $FE
      LD   B, $BF ; HJKLe
      IN   A, (C)
      NEG
      DEC  A
      AND  A, $1F
      LD  (_keyHJKLe), A

      LD   C, $FE
      LD   B, $7F ; BNMys
      IN   A, (C)
      NEG
      DEC  A
      AND  A, $1F
      LD  (_keyBNMys), A

      POP BC
   __endasm
} // readKeys()

u08 keyVdown()  { return keyVCXZc & 0x10 ? 1 : 0; }
u08 keyCdown()  { return keyVCXZc & 0x08 ? 1 : 0; }
u08 keyXdown()  { return keyVCXZc & 0x04 ? 1 : 0; }
u08 keyZdown()  { return keyVCXZc & 0x02 ? 1 : 0; }
u08 keyCSdown() { return keyVCXZc & 0x01 ? 1 : 0; }

u08 keyGdown()  { return keyGFDSA & 0x10 ? 1 : 0; }
u08 keyFdown()  { return keyGFDSA & 0x08 ? 1 : 0; }
u08 keyDdown()  { return keyGFDSA & 0x04 ? 1 : 0; }
u08 keySdown()  { return keyGFDSA & 0x02 ? 1 : 0; }
u08 keyAdown()  { return keyGFDSA & 0x01 ? 1 : 0; }

u08 keyTdown()  { return keyTREWQ & 0x10 ? 1 : 0; }
u08 keyRdown()  { return keyTREWQ & 0x08 ? 1 : 0; }
u08 keyEdown()  { return keyTREWQ & 0x04 ? 1 : 0; }
u08 keyWdown()  { return keyTREWQ & 0x02 ? 1 : 0; }
u08 keyQdown()  { return keyTREWQ & 0x01 ? 1 : 0; }

u08 key5down()  { return key54321 & 0x10 ? 1 : 0; }
u08 key4down()  { return key54321 & 0x08 ? 1 : 0; }
u08 key3down()  { return key54321 & 0x04 ? 1 : 0; }
u08 key2down()  { return key54321 & 0x02 ? 1 : 0; }
u08 key1down()  { return key54321 & 0x01 ? 1 : 0; }

u08 key6down()  { return key67890 & 0x10 ? 1 : 0; }
u08 key7down()  { return key67890 & 0x08 ? 1 : 0; }
u08 key8down()  { return key67890 & 0x04 ? 1 : 0; }
u08 key9down()  { return key67890 & 0x02 ? 1 : 0; }
u08 key0down()  { return key67890 & 0x01 ? 1 : 0; }

u08 keyYdown()  { return keyYUIOP & 0x10 ? 1 : 0; }
u08 keyUdown()  { return keyYUIOP & 0x08 ? 1 : 0; }
u08 keyIdown()  { return keyYUIOP & 0x04 ? 1 : 0; }
u08 keyOdown()  { return keyYUIOP & 0x02 ? 1 : 0; }
u08 keyPdown()  { return keyYUIOP & 0x01 ? 1 : 0; }

u08 keyHdown()  { return keyHJKLe & 0x10 ? 1 : 0; }
u08 keyJdown()  { return keyHJKLe & 0x08 ? 1 : 0; }
u08 keyKdown()  { return keyHJKLe & 0x04 ? 1 : 0; }
u08 keyLdown()  { return keyHJKLe & 0x02 ? 1 : 0; }
u08 keyENdown() { return keyHJKLe & 0x01 ? 1 : 0; }

u08 keyBdown()  { return keyBNMys & 0x10 ? 1 : 0; }
u08 keyNdown()  { return keyBNMys & 0x08 ? 1 : 0; }
u08 keyMdown()  { return keyBNMys & 0x04 ? 1 : 0; }
u08 keySSdown() { return keyBNMys & 0x02 ? 1 : 0; }
u08 keySPdown() { return keyBNMys & 0x01 ? 1 : 0; }
