/********************						***********************/

//

//	Player PRO 5.0 -- MAD Class for BeOS -

//

//	Library Version 5.0

//

//	To use with MAD Library for BeOS: CodeWarrior

//

//	Antoine ROSSET

//	16 Tranchees

//	1206 GENEVA

//	SWITZERLAND

//

//	Thank you for your interest in PlayerPRO !

//

//	Special Thanks to:

//

//	Dario Accornero <adario@cs.bu.edu>

//

//	For his BeOS support and help!

//

//	FAX:			(+41 22) 346 11 97

//	PHONE: 			(+41 79) 203 74 62

//	Internet: 		rossetantoine@bluewin.ch

//

/********************						***********************/



#include "RDriver.h"

#include "FileUtils.h"



void iFileCreate( Ptr AlienFileName, long)

{

}



FILE* iFileOpen( Ptr name)

{

	return fopen( name, "rb");

}



long iGetEOF( FILE* iFileRefI)

{

	long curEOF;

	

	fseek( iFileRefI, 0, SEEK_END);

	curEOF = ftell( iFileRefI);

	fseek( iFileRefI, 0, 0);

	

	return curEOF;

}



OSErr iRead( long size, Ptr dest, FILE* iFileRefI)

{

	fread( dest, size, 1, iFileRefI);

	

	return noErr;

}



OSErr iSeekCur( long size, FILE* iFileRefI)

{

	return fseek( iFileRefI , size, SEEK_CUR);

}



OSErr iWrite( long size, Ptr dest, FILE* iFileRefI)

{

	fwrite( dest, size, 1, iFileRefI);

	

	return noErr;

}



void iClose( FILE* iFileRefI)

{

	fclose( iFileRefI);

}



//	DA on 9/9/98



void MOT32( void *msg_buf)

{

	uint32	*theDataPtr = (uint32*)msg_buf;

	uint32	theData = *theDataPtr;

	theData = B_BENDIAN_TO_HOST_INT32( theData );

	*theDataPtr = theData;

}



//	DA on 9/9/98



void MOT16( void *msg_buf)

{

	uint16	*theDataPtr = (uint16*)msg_buf;

	uint16	theData = *theDataPtr;

	theData = B_BENDIAN_TO_HOST_INT16( theData );

	*theDataPtr = theData;

}



//	DA on 9/9/98



void INT32( void *msg_buf)

{

	uint32	*theDataPtr = (uint32*)msg_buf;

	uint32	theData = *theDataPtr;

	theData = B_LENDIAN_TO_HOST_INT32( theData );

	*theDataPtr = theData;

}



//	DA on 9/9/98



void INT16( void *msg_buf)

{

	uint16	*theDataPtr = (uint16*)msg_buf;

	uint16	theData = *theDataPtr;

	theData = B_LENDIAN_TO_HOST_INT16( theData );

	*theDataPtr = theData;

}



Ptr MADstrcpy( Ptr dst, const char* src)

{

	long i = 0;

	

	do

	{

		dst[ i] = src[ i];

	}while( src[ i++]);

	

	return dst;

}



int MADstrcmp( const char *dst, const char* src)

{

	long i = 0;

	

	do

	{

		if( dst[ i] != src[ i]) return -1;

	}while( src[ i++]);

	

	return 0;

}

