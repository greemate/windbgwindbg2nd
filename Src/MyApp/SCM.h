#ifndef _SCM_H_
#define _SCM_H_


DWORD RegisterLegacyDriver(LPSTR lpPathName, LPSTR lpDriver, DWORD dwStartType, DWORD dwErrorControl);
DWORD DeregisterLegacyDriver(LPSTR lpDriver);
DWORD StartLegacyDriver(LPSTR lpDriver);
DWORD StopLegacyDriver(LPSTR lpDriver);


#endif // _SCM_H_
