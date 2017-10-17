#include <assert.h>
#include <stdlib.h>
#include "bmap.h"

uint64_t BMAP_CEIL_SIZE( uint64_t bits )
{
	uint64_t tmp = bits + BMAP_DEFAULT_ALIGNMENT;

	assert( tmp != 0 );

	return( BMAP_DEFAULT_ALIGNMENT *
			( tmp / BMAP_DEFAULT_ALIGNMENT ) );
}

/*
 *  * Contstructor for BMAP_Class Object
 *   */
BMAP_Class BMap =
{
	BMap_New,
	BMap_free,
	BMap_set,
	BMap_clear,
	BMap_check
};


BMAP* BMap_New( uint64_t bits )
{
	BMAP* ret = NULL;
	uint64_t adjBits = BMAP_CEIL_SIZE( bits ),
				 bytes = 0;

	assert( adjBits != 0 );
	assert( ( adjBits % BMAP_DEFAULT_ALIGNMENT ) == 0 );

	bytes = adjBits / (8 * sizeof( char ));

	ret = ( BMAP* )( calloc( 1, sizeof( BMAP ) ) );

	assert( ret != NULL );

	ret->sizeBits = adjBits;
	ret->sizeBytes = bytes;
	ret->bits = ( unsigned char* )( calloc( bytes, sizeof( unsigned char ) ) );

	assert( ret->bits != NULL );

	return( ret );
}

void BMap_set( BMAP* bm, uint64_t idx )
{
	uint64_t byte = 0,
				 bit = 0;
	unsigned char mask = 0;

	assert( bm != NULL );
	assert( bm->bits != NULL );

	byte = idx / ( 8 * sizeof( char ) );
	assert( byte < bm->sizeBytes );

	bit = idx % ( 8 * sizeof( char ) );
	assert( bit <= 7 );
	mask = ( 1 << bit );

	bm->bits[ byte ] |= mask;
}


void BMap_clear( BMAP* bm, uint64_t idx )
{
	uint64_t byte = 0,
				 bit = 0;
	unsigned char mask = 0;

	assert( bm != NULL );
	assert( bm->bits != NULL );

	byte = idx / ( 8 * sizeof( char ) );
	assert( byte < bm->sizeBytes );

	bit = idx % ( 8 * sizeof( char ) );
	assert( bit <= 7 );
	mask = ~( 1 << bit );

	bm->bits[ byte ] &= mask;
}

bool BMap_check( BMAP* bm, uint64_t idx )
{
	uint64_t byte = 0,
				 bit = 0;
	unsigned char mask = 0;

	assert( bm != NULL );
	assert( bm->bits != NULL );
	assert( idx < bm->sizeBits );

	byte = idx / ( 8 * sizeof( char ) );
	assert( byte < bm->sizeBytes );

	bit = idx % ( 8 * sizeof( char ) );
	assert( bit <= 7 );

	mask = ( 1 << bit );

	if( ( bm->bits[ byte ] & mask ) == 0 )
	{
		return( false );
	} 

	return( true );
}

void BMap_free( BMAP* bm )
{
	assert( bm != NULL );

	assert( bm->bits != NULL );

	free( bm->bits );

	bm->bits = NULL;

	free( bm );
}


