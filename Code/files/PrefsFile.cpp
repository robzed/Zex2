/*
 * PrefsFile.cpp
 *
 * $Log: PrefsFile.cpp,v $
 * Revision 1.2  2003/09/20 12:57:00  robp
 * Removed nested comments
 *
 * Revision 1.1.1.1  2003/09/05 22:35:24  stu_c
 * First Imported.
 *
 * Revision 1.5  2002/09/08 02:02:57  stu_c
 * Precompiled header adjustments
 *
 * Revision 1.4  2002/01/02 19:22:54  rob
 * added key setup code
 *
 * Revision 1.3  2001/12/29 01:09:41  stu
 * Removed flat carbon from project. All files update to observe zex anvil and pb builds and includethe right files as appropriate.
 *
 * Revision 1.2  2001/11/02 19:31:14  rob
 * Build variants for interface lib and carbon.
Coffee fixes after changes for X.
 *
 * Revision 1.1  2001/10/21 01:02:40  stu
 * Initial porting work
 *
 * Revision 1.0.0.1  2001/10/17 20:46:04  rob
 * First Imported.
 *
 * Revision 1.1.1.1  2001/01/01 21:13:35  rob
 * First Imported.
 *
 * Revision 1.0.0.1  2000/08/21 22:01:39  stu
 * First Imported.
 *
 */
#ifndef __ZEX_PCH__
  #include "zselector.h"		// this selects which build we will be doing
  #include "zplatform.h"

#endif //pch


#include "PrefsFile.h"

#ifdef __cplusplus
extern "C" {
#endif
OSType gPrefsCreator = 'LsZx';
#ifdef __cplusplus
}
#endif

FSSpec PrefsFileSpec( Str255 fileName )
{
	short foundVRefNum;
	long foundDirID;
	FSSpec out;
	OSErr err;
	
	// find the Preferences folder
	err = FindFolder(	kOnSystemDisk,
						kPreferencesFolderType,
						kCreateFolder,
						&foundVRefNum,&foundDirID);
	if (!err)
		// make it into a FSSpec (given the file name)
		FSMakeFSSpec(foundVRefNum, foundDirID, fileName, &out);
	return out;
}

OSErr WritePrefsFile( FSSpec fileSpec, Ptr buffer, long bufferLen ) {
	short fileRefNum;
	OSErr err;
	
	// create our prefs file
	err = FSpCreate( &fileSpec, gPrefsCreator, 'zprf', smSystemScript );
	if (err && err != -48) return err;

	// open it for writing
	err = FSpOpenDF( &fileSpec, fsWrPerm, &fileRefNum);
	if (err) return err;

	err = SetEOF( fileRefNum, 0);
	if (err) return err;

	// write the data
	err = FSWrite( fileRefNum, &bufferLen, buffer );
	if (err) return err;
	
	FSClose( fileRefNum );
	return err;
}

OSErr ReadPrefsFile( FSSpec fileSpec, Ptr buffer, long bufferLen ) {
	short fileRefNum;
	OSErr err;
	
	// open prefs file for reading
	err = FSpOpenDF( &fileSpec, fsCurPerm, &fileRefNum);
	if (err) return err;

	// read the data
	err = FSRead( fileRefNum, &bufferLen, buffer );
	FSClose( fileRefNum );

	return err;
}

