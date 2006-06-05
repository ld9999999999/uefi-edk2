#include "CompressDll.h"
#include "EfiCompress.h"

extern
EFI_STATUS
Compress (
  IN      UINT8   *SrcBuffer,
  IN      UINT32  SrcSize,
  IN      UINT8   *DstBuffer,
  IN OUT  UINT32  *DstSize
  );
  
JNIEXPORT jbyteArray JNICALL  Java_org_tianocore_framework_tasks_Compress_CallCompress 
(JNIEnv *env, jobject obj, jbyteArray SourceBuffer, jint SourceSize, jstring path)
{
  char*          DestBuffer;
  int            DestSize;   
  int            Result;
  char           *InputBuffer;
  jbyteArray     OutputBuffer;
  jbyte          *TempByte;
  
  DestSize   = 0;
  DestBuffer = NULL;
  
  TempByte = (*env)->GetByteArrayElements(env, SourceBuffer, 0);
  InputBuffer = (char*) TempByte;

  
   //
   //  First call compress function and get need buffer size
   //

   Result = Compress (
			  (char*) InputBuffer, 
			  SourceSize,  
			  DestBuffer,
			  &DestSize
			  );

   if (Result = EFI_BUFFER_TOO_SMALL) {
	   DestBuffer = malloc (DestSize);
   }

   //
   //  Second call compress and get the DestBuffer value
   //
   Result = Compress(
              (char*) InputBuffer, 
			  SourceSize,  
			  DestBuffer,
			  &DestSize  
	   );

   //
   // new a MV array to store the return compressed buffer
   //
   OutputBuffer = (*env)->NewByteArray(env, DestSize);
   (*env)->SetByteArrayRegion(env, OutputBuffer,0, DestSize, (jbyte*) DestBuffer);

   //
   // Free Ouputbuffer.
   //
   free (DestBuffer);
  

  if (Result != 0) {
    return NULL;
  } else {
    return OutputBuffer;
  }
}

#ifdef _MSC_VER
BOOLEAN 
__stdcall 
DllMainCRTStartup(
    unsigned int hDllHandle, 
    unsigned int nReason,    
    void*   Reserved    
)
{
	return TRUE;
}
#else
#ifdef __GNUC__
#endif
#endif

