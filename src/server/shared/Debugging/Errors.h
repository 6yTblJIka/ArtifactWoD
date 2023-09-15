////////////////////////////////////////////////////////////////////////////////
//
// ArtifactCore based on TC 6.2.X
// Copyright (C) 2012-2023 TrinityCore
// All Rights Reserved.  Web site TrinityCore https://trinitycore.org/
//
////////////////////////////////////////////////////////////////////////////////

#ifndef ERRORS_H
#define ERRORS_H

#include "Common.h"
#include "Log.h"
#include <ace/Stack_Trace.h>
#include <ace/OS_NS_unistd.h>

#define WPAssert(assertion) { if (!(assertion)) { sLog->outAshran("\n%s:%i in %s ASSERTION FAILED:\n  %s\n%", __FILE__, __LINE__, __FUNCTION__, #assertion); } }
#define WPError(assertion, errmsg) { if (!(assertion)) { sLog->outError(LOG_FILTER_GENERAL, "%\n%s:%i in %s ERROR:\n  %s\n", __FILE__, __LINE__, __FUNCTION__, (char *)errmsg); } }
#define WPWarning(assertion, errmsg) { if (!(assertion)) { sLog->outError(LOG_FILTER_GENERAL, "\n%s:%i in %s WARNING:\n  %s\n", __FILE__, __LINE__, __FUNCTION__, (char *)errmsg); } }
#define WPFatal(assertion, errmsg) { if (!(assertion)) { sLog->outError(LOG_FILTER_GENERAL, "\n%s:%i in %s FATAL ERROR:\n  %s\n", __FILE__, __LINE__, __FUNCTION__, (char *)errmsg); ACE_OS::sleep(10); } }

#define ASSERT WPAssert
#endif
