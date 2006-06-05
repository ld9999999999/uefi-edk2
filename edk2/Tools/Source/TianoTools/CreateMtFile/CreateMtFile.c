/*++

Copyright (c)  1999 - 2002 Intel Corporation. All rights reserved
This software and associated documentation (if any) is furnished
under a license and may only be used or copied in accordance
with the terms of the license. Except as permitted by such
license, no part of this software or documentation may be
reproduced, stored in a retrieval system, or transmitted in any
form or by any means without the express written consent of
Intel Corporation.


Module Name:

  CreateMtFile.c

Abstract:

  Simple utility to create a pad file containing fixed data.
  
--*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "UefiBaseTypes.h"
#include "Base.h"

#define PROGRAM_NAME  "CreateMtFile"

typedef struct {
  INT8    *OutFileName;
  INT8    ByteValue;
  UINT32  FileSize;
} OPTIONS;

static
EFI_STATUS
ProcessArgs (
  IN INT32          Argc,
  IN INT8           *Argv[],
  IN OUT OPTIONS    *Options
  );

static
void
Usage (
  VOID
  );

int
main (
  IN INT32  Argc,
  IN INT8   *Argv[]
  )
/*++

Routine Description:
  
  Main entry point for this utility.

Arguments:

  Standard C entry point args Argc and Argv

Returns:

  EFI_SUCCESS if good to go

--*/
// GC_TODO:    ] - add argument and description to function comment
// GC_TODO:    EFI_INVALID_PARAMETER - add return value to function comment
// GC_TODO:    EFI_DEVICE_ERROR - add return value to function comment
// GC_TODO:    EFI_DEVICE_ERROR - add return value to function comment
{
  FILE    *OutFptr;
  OPTIONS Options;

  //
  // Process the command-line arguments.
  //
  if (ProcessArgs (Argc, Argv, &Options) != EFI_SUCCESS) {
    return EFI_INVALID_PARAMETER;
  }
  //
  // Open the output file
  //
  if ((OutFptr = fopen (Options.OutFileName, "wb")) == NULL) {
    fprintf (
      stdout,
      PROGRAM_NAME " ERROR: Could not open output file '%s' for writing\n",
      Options.OutFileName
      );
    return EFI_DEVICE_ERROR;
  }
  //
  // Write the pad bytes. Do it the slow way (one at a time) for now.
  //
  while (Options.FileSize > 0) {
    if (fwrite (&Options.ByteValue, 1, 1, OutFptr) != 1) {
      fclose (OutFptr);
      fprintf (stdout, PROGRAM_NAME " ERROR: Failed to write to output file\n");
      return EFI_DEVICE_ERROR;
    }

    Options.FileSize--;
  }
  //
  // Close the file
  //
  fclose (OutFptr);
  return EFI_SUCCESS;
}

static
EFI_STATUS
ProcessArgs (
  IN INT32          Argc,
  IN INT8           *Argv[],
  IN OUT OPTIONS    *Options
  )
/*++

Routine Description:
  
  Process the command line arguments.

Arguments:

  Argc    - argument count as passed in to the entry point function
  Argv    - array of arguments as passed in to the entry point function
  Options - stucture of where to put the values of the parsed arguments

Returns:

  EFI_SUCCESS if everything looks good
  EFI_INVALID_PARAMETER otherwise

--*/
// GC_TODO:    ] - add argument and description to function comment
{
  UINT32  Multiplier;

  //
  // Clear the options
  //
  memset ((char *) Options, 0, sizeof (OPTIONS));

  //
  // Skip program name
  //
  Argv++;
  Argc--;
  if (Argc < 2) {
    Usage ();
    return EFI_INVALID_PARAMETER;
  }
  //
  // If first arg is dash-option, then print usage.
  //
  if (Argv[0][0] == '-') {
    Usage ();
    return EFI_INVALID_PARAMETER;
  }
  //
  // First arg is file name
  //
  Options->OutFileName = Argv[0];
  Argc--;
  Argv++;

  //
  // Second arg is file size. Allow 0x1000, 0x100K, 1024, 1K
  //
  Multiplier = 1;
  if ((Argv[0][strlen (Argv[0]) - 1] == 'k') || (Argv[0][strlen (Argv[0]) - 1] == 'K')) {
    Multiplier = 1024;
  }
  //
  // Look for 0x prefix on file size
  //
  if ((Argv[0][0] == '0') && ((Argv[0][1] == 'x') || (Argv[0][1] == 'X'))) {
    if (sscanf (Argv[0], "%x", &Options->FileSize) != 1) {
      fprintf (stdout, PROGRAM_NAME " ERROR: Invalid file size '%s'\n", Argv[0]);
      Usage ();
      return EFI_INVALID_PARAMETER;
    }
    //
    // Otherwise must be a decimal number
    //
  } else {
    if (sscanf (Argv[0], "%d", &Options->FileSize) != 1) {
      fprintf (stdout, PROGRAM_NAME " ERROR: Invalid file size '%s'\n", Argv[0]);
      Usage ();
      return EFI_INVALID_PARAMETER;
    }
  }

  Options->FileSize *= Multiplier;
  //
  // Assume byte value of 0xff
  //
  Options->ByteValue = (INT8) (UINT8) 0xFF;
  return EFI_SUCCESS;
}
//
// Print utility usage info
//
static
void
Usage (
  VOID
  )
/*++

Routine Description:

  GC_TODO: Add function description

Arguments:

  None

Returns:

  GC_TODO: add return values

--*/
{
  UINT32            Index;
  static const INT8 *Text[] = {
    " ",
    "Usage:  "PROGRAM_NAME " OutFileName FileSize",
    "  where:",
    "    OutFileName is the name of the output file to generate",
    "    FileSize is the size of the file to create",
    "  Examples:",
    "    "PROGRAM_NAME " OutFile.bin 32K",
    "    "PROGRAM_NAME " OutFile.bin 0x1000",
    " ",
    NULL
  };

  for (Index = 0; Text[Index] != NULL; Index++) {
    fprintf (stdout, "%s\n", Text[Index]);
  }
}
