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

/* Called before starting a profiled section of code */
void taskTimeStart(uint32_T);

/* Called on finishing a profiled section of code */
void taskTimeEnd(uint32_T);

/* Uploads data */
void xilUploadProfilingData(uint32_T sectionId);

/* Pause the timer while running code associated with storing and uploading the data. */
void xilProfilingTimerFreeze(void);

/* Restart the timer after a pause */
void xilProfilingTimerUnFreeze(void);

/* Code instrumentation method(s) for model MicroMouseTemplate */
void taskTimeStart_MicroMouseTemplate(uint32_T sectionId);
void taskTimeEnd_MicroMouseTemplate(uint32_T sectionId);

/* Code instrumentation method(s) for model MicroMouseTemplate */
