/*
 * File: code_profiling_utility_functions.h
 *
 * Code generated for instrumentation.
 *
 */

/* Functions with a C call interface */
#ifdef __cplusplus

extern "C"
{

#endif

#include "xcp_platform_custom.h"
#ifdef __cplusplus

}

#endif

#include "rtwtypes.h"

/* Upload code instrumentation data point */
void store_code_profiling_data_point(
  void* pData, uint32_T numMemUnits, uint32_T sectionId);

/* Atomic function to read timer and upload data point */
void code_profiling_atomic_read_store(uint32_T sectionId);

/* Uploads data */
void xilUploadProfilingData(uint32_T sectionId);

/* Pause/restart the timer while running code associated with storing and uploading the data. */
void xilProfilingTimerFreeze(void);
void xilProfilingTimerUnFreeze(void);

/* Code instrumentation method(s) for model MicroMouseTemplate */
void taskTimeStart_MicroMouseTemplate(uint32_T sectionId);
void taskTimeEnd_MicroMouseTemplate(uint32_T sectionId);

/* Code instrumentation method(s) for model MicroMouseTemplate */
