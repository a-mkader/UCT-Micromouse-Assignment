/*
 * File: code_profiling_utility_functions.c
 *
 * Code generated for instrumentation.
 *
 */

#include "code_profiling_utility_functions.h"

/* Code instrumentation offset(s) for model MicroMouseTemplate */
#define taskTimeStart_MicroMouseTemplate_offset 0
#define taskTimeEnd_MicroMouseTemplate_offset 0

/* Code instrumentation offset(s) for model MicroMouseTemplate */

/* A function parameter may be intentionally unused */
#ifndef UNUSED_PARAMETER
# if defined(__LCC__)
#   define UNUSED_PARAMETER(x)
# else
#   define UNUSED_PARAMETER(x)         (void) (x)
# endif
#endif

void xilUploadProfilingData(uint32_T sectionId)
{
  code_profiling_atomic_read_store(sectionId);
}

/* For real-time, multitasking case this function is stubbed out. */
void xilProfilingTimerFreezeInternal(void)
{
}

void xilProfilingTimerFreeze(void)
{
}

/* For real-time, multitasking case this function is stubbed out. */
void xilProfilingTimerUnFreezeInternal(void)
{
}

void xilProfilingTimerUnFreeze(void)
{
}

void taskTimeStart(uint32_T sectionId)
{
  /* Send execution profiling data to host */
  xilUploadProfilingData(sectionId);
  xilProfilingTimerUnFreezeInternal();
}

void taskTimeEnd(uint32_T sectionId)
{
  uint32_T sectionIdNeg = ~sectionId;
  xilProfilingTimerFreezeInternal();

  /* Send execution profiling data to host */
  xilUploadProfilingData(sectionIdNeg);
}

/* Code instrumentation method(s) for model MicroMouseTemplate */
void taskTimeStart_MicroMouseTemplate(uint32_T sectionId)
{
  taskTimeStart(taskTimeStart_MicroMouseTemplate_offset + sectionId);
}

void taskTimeEnd_MicroMouseTemplate(uint32_T sectionId)
{
  taskTimeEnd(taskTimeEnd_MicroMouseTemplate_offset + sectionId);
}

/* Code instrumentation method(s) for model MicroMouseTemplate */
