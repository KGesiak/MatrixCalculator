#define _DEBUG_
#include <iostream>
#include "matrix.h"
void PrintVector( double* pv, int nSize );
void ReadData( FILE* fin, double** pMatrix, double* b, int nSize );
void DeleteVector( double** pv, int nSize );

int main( int argc, char* argv[] )
{
	if( argc != 2 )
	{
		printf( "Usage: %s <plik_we>", argv[0] );
		return 1;
	}
	FILE* fin;
	fin = fopen( argv[1], "r" );
	if( !fin )
	{
		printf( "ERROR: File %s could not be opened!", argv[1] );
		return 2;
	}

	int nSize = 0; // Size of matrix	
	fscanf( fin, "%d", &nSize ); //Reading the size of matrix from the file

	double** pMatrix = NULL;

	double* pB = (double*)malloc( sizeof( double ) * nSize ); //Creating the vector

	if( !CreateMatrix( &pMatrix, nSize ) || !pB )
	{
		perror( "ERROR: Allocation error while creating pMatrix / pB" );
		return 3;
	}

	memset( pB, 0, sizeof( double ) * nSize );

	ReadData( fin, pMatrix, pB, nSize );
	fclose( fin );

#ifdef _DEBUG_
	printf( "Matrix: \n" );
	PrintMatrix( pMatrix, nSize );
	printf( "\nb: \n" );
	PrintVector( pB, nSize );

#endif
	

	double D = Det( pMatrix, nSize );
	if( fabs(D) < 1e-16 )
	{
		perror( "ERROR: Matrix cannot be inversed, Det = 0!" );
		return 4;
	}

	double** InvMatrix = NULL; 
	CreateMatrix( &InvMatrix, nSize ); //Creating the inversed matrix
	if( !InvMatrix )
	{
		perror( "ERROR: Allocation error while creating InvMatrix" );
		return 5;
	}
	InverseMatrix( InvMatrix, pMatrix, nSize, D );
#ifdef _DEBUG_

	printf( "\nDetriment: %.3lf \n", D );
	printf( "\nInversed Matrix: \n" );
	PrintMatrix( InvMatrix, nSize );
#endif

	double* pRes = (double*)malloc( sizeof( double ) * nSize ); //Creating the result vector
	
	if( !pRes )
	{
		perror( "ERROR: Allocation error while creating pRes" );
		return 6;
	}
	
	memset( pRes, 0, sizeof( double ) * nSize );

	LayoutEqu( InvMatrix, pB, pRes, nSize );
	printf( "\nx: \n" );
	PrintVector( pRes, nSize );

	DeleteMatrix( &pMatrix, nSize );
	DeleteMatrix( &InvMatrix, nSize );
	DeleteVector( &pRes,nSize );
	DeleteVector( &pB,nSize );

	return 0;

}
void PrintVector( double* pv, int nSize )
{ 
	for( int i = 0; i < nSize; i++ )
		printf( "%.3lf\n", *pv++ );
}
void ReadData( FILE* fin, double** pMatrix, double* b, int nSize )
{
	for( int i = 0; i < nSize; i++ ) 
	{
		double* p = *pMatrix++;
		for( int j = 0; j < nSize; j++ )
			fscanf( fin, "%lf", p++ );
		fscanf( fin, "%lf", b++ );
	}
}
void DeleteVector( double** pv, int nSize )
{
	free( *pv );
	*pv = NULL;
}