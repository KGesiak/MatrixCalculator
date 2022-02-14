#include <memory>
#include <iostream>
using namespace std;

void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nSize );
void ComplMatrix( double** pTabD, double** pTab, int nSize );


int CreateMatrix( double*** pTab, int nSize )
{
	*pTab = (double**)malloc( sizeof( double* ) * nSize );
	if( !pTab ) return 0;
	memset( *pTab, 0, sizeof( double* ) * nSize );

	double** p = *pTab;
	for( int i = 0; i < nSize; i++ )
	{
		*p = (double*)malloc( sizeof( double ) * nSize );
		if( !*p ) return 0;
		memset( *p++, 0, sizeof( double ) * nSize );
	}
	return 1;
}

void Complement( double** pTabO, double** pTabI, int nRow, int nCol, int nSize )
{

	for( int i = 0; i < nSize; i++, pTabI++) 
	{
		if( i == nRow )
			continue;
		double* pO = *pTabO++;
		double* pT = *pTabI;
		for( int j = 0; j < nSize; j++, pT++ ) 
		{
			if( j == nCol )
				continue;
			*pO++ = *pT;		//Copying values from the original matrix to the one without given row and column
		}
	}
}
void DeleteMatrix( double*** pTab, int nSize )
{
	double** v = *pTab;
	for( int i = 0; i < nSize; i++ )
		free( *v++ );
	free( *pTab );
	*pTab = NULL;
}
double Det( double** pTab, int nSize )
{

	if( nSize == 1 ) return **pTab;
	if( nSize == 2 ) return **pTab * pTab[1][1] - pTab[1][0] * pTab[0][1];

	double** temp = NULL;	
	
	if( !CreateMatrix( &temp, nSize - 1 ) ) //Creating a temporary matrix in order to count its determinant later on
	{
		perror( "Allocation error in Det" );
		return 1;
	}

	double res = 0;	
	double* ptr = *pTab;
	int sign = 1;

	for( int i = 0; i < nSize; i++ )
	{
		Complement( temp, pTab, 0, i, nSize );
		res += sign * *ptr++ * Det( temp, nSize - 1 );
		sign= -sign;

	}

	DeleteMatrix( &temp, nSize - 1 ); // Deleting the temporary matrix

	return res;			
}
void ComplMatrix( double** pTabD, double** pTab, int nSize )
{
	double** temp = NULL;	
		
	if( !CreateMatrix( &temp, nSize - 1 ) ) // Creating a temporary matrix in order to count determinant of the smaller matrix
	{
		perror( "ERROR: Allocation error in ComplMatrix!" );
		return;
	}
	for( int i = 0; i < nSize; i++, pTabD++ )
	{
		int sign = ( i%2 ) ? -1: 1;
		double* p = *pTabD;
		for( int j = 0; j < nSize; j++ )
		{
			Complement( temp, pTab, i, j, nSize );
			*p++ = sign * Det( temp, nSize - 1 );
			sign = -sign;
		}
	}
	
}
void TransMatrix( double** pTab, int nSize )
{
	double** p = pTab;
	for( int i = 0; i < nSize - 1; i++, p++ )
	{
		double* v = *p + i + 1;
		for( int j = i + 1; j < nSize; j++ )
		{
			double temp = *v;
			*v++ = pTab[j][i];
			pTab[j][i] = temp;
		}
	}
}
void LayoutEqu( double** pInv, double* pB, double* pRes, int nSize )
{
	for( int i = 0; i < nSize; i++, pInv++, pRes++ )
	{
		double* p = *pInv;
		double* pbb = pB;
		for( int j = 0; j < nSize; j++ ) 
			*pRes += *p++ * *pbb++;	//Multiplying every element in matrix's row by every element in vector
	}
}
void PrintMatrix( double** pTab, int nSize )
{

	for( int i = 0; i < nSize; i++ )
	{
		double* p = *pTab++;
		for( int j = 0; j < nSize; j++ )
			printf( "%.3lf ", *p++ );
		printf( "\n" );
	}
}
void InverseMatrix( double** pInv, double** pTab, int nSize, double det )
{
	ComplMatrix( pInv, pTab, nSize );
	TransMatrix( pInv, nSize );


	for( int i = 0; i < nSize; i++, pInv++ )
	{
		double* pI = *pInv;
		for( int j = 0; j < nSize; j++ )
		{
			*pI++ /= det;
		}
	}
}