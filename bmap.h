/*
*Copyright (c) 2014, Louis P. Santillan <lpsantil@gmail.com>
*All rights reserved.
*
*Redistribution and use in source and binary forms, with or without
*modification, are permitted provided that the following conditions are met:
*
** Redistributions of source code must retain the above copyright notice, this
*  list of conditions and the following disclaimer.
*
** Redistributions in binary form must reproduce the above copyright notice,
*  this list of conditions and the following disclaimer in the documentation
*  and/or other materials provided with the distribution.
*
*THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
*AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
*IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
*DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
*FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
*DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
*SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
*CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
*OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
*OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef __BMAP__
#define __BMAP__

#ifndef __bool_true_false_are_defined
   #ifdef _Bool
      #define bool _Bool
   #else
      #define bool char
   #endif
   #define true 1
   #define false 0
   #define __bool_true_false_are_defined 1
#endif

#define BMAP_DEFAULT_ALIGNMENT 64

typedef struct
{
   unsigned int sizeBits;
   unsigned int sizeBytes;
   unsigned char* bits;
} BMAP;

typedef struct
{
   BMAP* ( *New )( unsigned int bits );
   void ( *free )( BMAP* bm );
   void ( *set )( BMAP* bm, unsigned int idx );
   void ( *clear )( BMAP* bm, unsigned int idx );
   bool ( *check )( BMAP* bm, unsigned int idx );
} BMAP_Class;

unsigned int BMAP_CEIL_SIZE( unsigned int bits );

BMAP* BMap_New( unsigned int bits );
void BMap_free( BMAP* bm );
void BMap_set( BMAP* bm, unsigned int idx );
void BMap_clear( BMAP* bm, unsigned int idx );
bool BMap_check( BMAP* bm, unsigned int idx );

extern BMAP_Class BMap;

#endif
