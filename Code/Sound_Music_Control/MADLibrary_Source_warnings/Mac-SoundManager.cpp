/********************						***********************/
//
//	Player PRO 5.8 - DRIVER SOURCE CODE -
//
//	Library Version 5.8
//
//	To use with MAD Library for Mac: Symantec, CodeWarrior and MPW
//
//	Antoine ROSSET
//	20 Micheli-Du-Crest
//	1205 GENEVA
//	SWITZERLAND
//
//	COPYRIGHT ANTOINE ROSSET 1996, 1997, 1998, 1999, 2000
//
//	Thank you for your interest in PlayerPRO !
//
//	FAX:			(+41 22) 346 11 97
//	PHONE: 			(+41 79) 203 74 62
//	Internet: 		RossetAntoine@bluewin.ch
//
/********************						***********************/
#include "zselector.h"		// this selects which build we will be doing
#include "zplatform.h"
#include "ppsound.h"

#include "RDriver.h"
#include "RDriverInt.h"

/*************************/

OSErr	MySndPlayDoubleBuffer (SndChannelPtr chan, SndDoubleBufferHeaderPtr theParams);

SndChannelPtr CreateSndChannel( long init)
{
	SndChannelPtr 	mySndChan;	// pointer to a sound channel
	OSErr 			myErr;

	mySndChan = 0L;
	myErr = SndNewChannel( &mySndChan, sampledSynth, init, nil);
	
	return mySndChan;			// return SndChannelPtr
}

OSErr	DBSndClose( MADDriverRec *inMADDriver)
{
OSErr		err;
short			i;

	err = SndDisposeChannel( inMADDriver->MusicChannelPP, true);
	inMADDriver->MusicChannelPP = 0L;

	#if CALL_NOT_IN_CARBON
	
	DisposeSndDoubleBackUPP( inMADDriver->TheHeader.dbhDoubleBack);
	
	#endif
	
	inMADDriver->TheHeader.dbhDoubleBack = 0L;

	for ( i = 0;   i <= 1; i++) { DisposePtr( (Ptr) inMADDriver->TheHeader.dbhBufferPtr[i]); inMADDriver->TheHeader.dbhBufferPtr[i] = 0L;}

	return noErr;
}

OSErr	InitDBSoundManager( MADDriverRec *inMADDriver, long init)
{
	OSErr			err = noErr;

	inMADDriver->MusicChannelPP = 0L;
	inMADDriver->MusicChannelPP = CreateSndChannel( init);
	if( inMADDriver->MusicChannelPP == 0L) return MADSoundManagerErr;
	
	err = DBSndPlay( inMADDriver, inMADDriver->MusicChannelPP);
	return err;
}

pascal void MyDoubleBackProc(SndChannelPtr chan, SndDoubleBufferPtr doubleBuffer);

OSErr DBSndPlay ( MADDriverRec *inMADDriver, SndChannelPtr chan)
{
	SndDoubleBufferPtr 			doubleBuffer;
	short 						i;
	OSErr 						err = noErr;
	
	inMADDriver->TheHeader.dbhCompressionID 		= 0;
	inMADDriver->TheHeader.dbhPacketSize 			= 0;
	
	#if CALL_NOT_IN_CARBON
	
	inMADDriver->TheHeader.dbhDoubleBack 			= NewSndDoubleBackProc( MyDoubleBackProc);
	
	#else
	
	inMADDriver->TheHeader.dbhDoubleBack 			= (SndDoubleBackUPP) MyDoubleBackProc;
	
	#endif
	
	inMADDriver->TheHeader.dbhSampleRate = inMADDriver->DriverSettings.outPutRate;
	inMADDriver->TheHeader.dbhSampleSize = inMADDriver->DriverSettings.outPutBits;
	
//	if( inMADDriver->DriverSettings.outPutMode == MonoOutPut) inMADDriver->TheHeader.dbhNumChannels = 1;
//	else
	
	inMADDriver->TheHeader.dbhNumChannels = 2;
	
	for (i = 0; i <= 1; i++)
	{
		doubleBuffer = (SndDoubleBufferPtr) MADNewPtrClear( sizeof (SndDoubleBuffer) + inMADDriver->BufSize + 20L, inMADDriver->lib);
		if( doubleBuffer == 0L) return MADNeedMemory;

		doubleBuffer->dbNumFrames 		= 0;
		doubleBuffer->dbFlags 			= 0;
		doubleBuffer->dbUserInfo[ 0]	= (long) inMADDriver;
		doubleBuffer->dbUserInfo[ 1]	= true;

		MyDoubleBackProc( chan, doubleBuffer);
		
		inMADDriver->TheHeader.dbhBufferPtr[i] = doubleBuffer;
	}
	#if CALL_NOT_IN_CARBON
	err = SndPlayDoubleBuffer( chan, &inMADDriver->TheHeader);
	#else
	err = MySndPlayDoubleBuffer( chan, &inMADDriver->TheHeader);
	#endif
	
	if( err != noErr) return MADSoundManagerErr;
	return err;
}

void StopChannel( MADDriverRec *inMADDriver)
{
SndCommand			cmd;

	cmd.cmd = quietCmd;
	cmd.param1 = 0;
	cmd.param2 = 0;
	SndDoImmediate(	inMADDriver->MusicChannelPP,	&cmd);
	
	cmd.cmd = flushCmd;
	cmd.param1 = 0;
	cmd.param2 = 0;
	SndDoImmediate(	inMADDriver->MusicChannelPP,	&cmd);
}

void PlayChannel( MADDriverRec *inMADDriver)
{
	OSErr					iErr;

	MyDoubleBackProc( inMADDriver->MusicChannelPP, inMADDriver->TheHeader.dbhBufferPtr[ 0]);
	MyDoubleBackProc( inMADDriver->MusicChannelPP, inMADDriver->TheHeader.dbhBufferPtr[ 1]);
	
	#if CALL_NOT_IN_CARBON
	iErr = SndPlayDoubleBuffer( inMADDriver->MusicChannelPP, &inMADDriver->TheHeader);
	#else
	iErr = MySndPlayDoubleBuffer( inMADDriver->MusicChannelPP, &inMADDriver->TheHeader);
	#endif
}