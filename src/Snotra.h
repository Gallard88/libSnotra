// ==============================================
// ==============================================

#ifndef __SNOTRA__H__
#define __SNOTRA__H__

#ifdef __cplusplus
extern "C" {
#endif


int Snotra_Connect(void);
int Snotra_Send(const char *module, const char *parameter, float value);
void Snotra_Close(void);

// ==============================================
// ==============================================
#ifdef __cplusplus
}
#endif
#endif
