// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

// OS compatiblity
#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

// Windows specific
#include <windows.h>

// detect memory leaks in debug builds
#define _CRTDBG_MAP_ALLOC

// standard C/C++ includes
#include <stdlib.h>
#include <crtdbg.h>
#include <stdint.h>
#include <stdio.h>
#include <assert.h>
#include <vector>
#include <io.h>
#include <direct.h>
#include <dirent.h>

#define INTEL
#define MAX_FILENAMELEN (MAX_PATH*2)

// external definitions
#include "CommonTypes.h"

// Core components
#include "BaseFile.h"
#include "DiskFile.h"
#include "MemoryFile.h"
#include "CRC.h"
#include "BitBuffer.h"

// IPF library public definitions
#include "CapsLibAll.h"

// CODECs
#include "DiskEncoding.h"
#include "CapsDefinitions.h"
#include "CTRawCodec.h"

// file support
#include "CapsFile.h"
#include "DiskImage.h"
#include "CapsLoader.h"
#include "CapsImageStd.h"
#include "CapsImage.h"
#include "StreamImage.h"
#include "StreamCueImage.h"
#include "DiskImageFactory.h"

// Device access
#include "C2Comm.h"

// system
#include "CapsCore.h"
#include "CapsFDCEmulator.h"
#include "CapsFormatMFM.h"
