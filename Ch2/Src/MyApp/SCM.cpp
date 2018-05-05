#include "stdafx.h"
#include "winsvc.h"

DWORD RegisterLegacyDriver(LPSTR lpPathName, LPSTR lpDriver, DWORD dwStartType, DWORD dwErrorControl)
{
	DWORD dwError = ERROR_SUCCESS;
	SC_HANDLE hSCMan = NULL;
	SC_HANDLE hService = NULL;

	// connect to local service control manager
	if ((hSCMan = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE )) == NULL) 
	{
		dwError = GetLastError();
		return dwError;
	}

	// add to service control manager's databasedatabase
	if ((hService = CreateService(hSCMan, 
								  lpDriver, 
   								  lpDriver, 
   								  GENERIC_READ, SERVICE_KERNEL_DRIVER,
   								  dwStartType, dwErrorControl,
   								  lpPathName, 
   								  NULL, NULL, NULL, NULL, NULL)) == NULL)
   	{
         dwError = GetLastError();
    }
  	else
  	{ 
  		CloseServiceHandle(hService);
  	}

	CloseServiceHandle(hSCMan);
  	
	return dwError;
}

DWORD DeregisterLegacyDriver(LPSTR lpDriver)
{
	DWORD dwError = ERROR_SUCCESS;
	SC_HANDLE hSCMan = NULL;
	SC_HANDLE hService = NULL;
	
	// connect to local service control manager
	if ((hSCMan = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT)) == NULL) 
	{
		dwError = GetLastError();
		return dwError;
	}

	// get a handle to the service
	if ((hService = OpenService(hSCMan, lpDriver, DELETE)) != NULL) 
	{
		// remove the driver
		if (!DeleteService(hService))
			dwError = GetLastError();
		 
		CloseServiceHandle(hService);
	}
	else 
	{
		dwError = GetLastError();
	}

	CloseServiceHandle(hSCMan);
  	
	return dwError;
}

DWORD StartLegacyDriver(LPSTR lpDriver)
{
	DWORD dwError = ERROR_SUCCESS;
	SC_HANDLE hSCMan = NULL;
	SC_HANDLE hService = NULL;
	
	// connect to local service control manager
	if ((hSCMan = OpenSCManager(NULL, NULL, SC_MANAGER_CREATE_SERVICE | SC_MANAGER_CONNECT)) == NULL) 
	{
		dwError = GetLastError();
		return dwError;
	}

	// get a handle to the service
	if ((hService = OpenService(hSCMan, lpDriver, SERVICE_START)) != NULL) 
	{
		// start the driver
		if (!StartService(hService, 0, NULL))
			dwError = GetLastError();

  		CloseServiceHandle(hService);
	} 
	else 
	{
		dwError = GetLastError();
	}
	
	CloseServiceHandle(hSCMan);
  	
	return dwError;
}

DWORD StopLegacyDriver(LPSTR lpDriver)
{
	DWORD dwError = ERROR_SUCCESS;
	SC_HANDLE hSCMan = NULL;
	SC_HANDLE hService = NULL;
	SERVICE_STATUS serviceStatus;
	
	// connect to local service control manager
	if ((hSCMan = OpenSCManager(NULL, NULL, SC_MANAGER_CONNECT)) == NULL) 
	{
		dwError = GetLastError();
		return dwError;
	}

	// get a handle to the service
	if ((hService = OpenService(hSCMan, lpDriver, SERVICE_STOP)) != NULL) 
	{
		// stop the driver
		if (!ControlService(hService, SERVICE_CONTROL_STOP, &serviceStatus))
		    dwError = GetLastError();

		CloseServiceHandle(hService);
	} 
	else 
	{
		dwError = GetLastError();
	}
	
	CloseServiceHandle(hSCMan);
  	
	return dwError;
}

