// PrefsFile.h
#ifdef __cplusplus
extern "C" {
#endif

FSSpec PrefsFileSpec( Str255 );
OSErr WritePrefsFile( FSSpec, Ptr, long );
OSErr ReadPrefsFile( FSSpec, Ptr, long );

#ifdef __cplusplus
}
#endif
