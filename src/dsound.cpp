#include <Windows.h>
#include <stdio.h>
#include <direct.h>
#include <io.h>

#include "MemoryMgr.h"

struct dsound_dll
{
	HMODULE dll;
	FARPROC DirectSoundCreate;
	FARPROC DirectSoundEnumerateA;
	FARPROC DirectSoundEnumerateW;
	FARPROC DllCanUnloadNow;
	FARPROC DllGetClassObject;
	FARPROC DirectSoundCaptureCreate;
	FARPROC DirectSoundCaptureEnumerateA;
	FARPROC DirectSoundCaptureEnumerateW;
	FARPROC GetDeviceID;
	FARPROC DirectSoundFullDuplexCreate;
	FARPROC DirectSoundCreate8;
	FARPROC DirectSoundCaptureCreate8;
} dsound;

WRAPPER void _DirectSoundCreate() { VARJMP(dsound.DirectSoundCreate); }
WRAPPER void _DirectSoundEnumerateA() { VARJMP(dsound.DirectSoundEnumerateA); }
WRAPPER void _DirectSoundEnumerateW() { VARJMP(dsound.DirectSoundEnumerateW); }
WRAPPER void _DllCanUnloadNow() { VARJMP(dsound.DllCanUnloadNow); }
WRAPPER void _DllGetClassObject() { VARJMP(dsound.DllGetClassObject); }
WRAPPER void _DirectSoundCaptureCreate() { VARJMP(dsound.DirectSoundCaptureCreate); }
WRAPPER void _DirectSoundCaptureEnumerateA() { VARJMP(dsound.DirectSoundCaptureEnumerateA); }
WRAPPER void _DirectSoundCaptureEnumerateW() { VARJMP(dsound.DirectSoundCaptureEnumerateW); }
WRAPPER void _GetDeviceID() { VARJMP(dsound.GetDeviceID); }
WRAPPER void _DirectSoundFullDuplexCreate() { VARJMP(dsound.DirectSoundFullDuplexCreate); }
WRAPPER void _DirectSoundCreate8() { VARJMP(dsound.DirectSoundCreate8); }
WRAPPER void _DirectSoundCaptureCreate8() { VARJMP(dsound.DirectSoundCaptureCreate8); }

void init_dsound(HINSTANCE hInst)
{
	TCHAR dsound_dll_path[MAX_PATH];

	GetEnvironmentVariable("windir", dsound_dll_path, MAX_PATH);
	strcat_s(dsound_dll_path, "\\System32\\dsound.dll");

	dsound.dll = LoadLibrary(dsound_dll_path);

	dsound.DirectSoundCreate = GetProcAddress(dsound.dll, "DirectSoundCreate");
	dsound.DirectSoundEnumerateA = GetProcAddress(dsound.dll, "DirectSoundEnumerateA");
	dsound.DirectSoundEnumerateW = GetProcAddress(dsound.dll, "DirectSoundEnumerateW");
	dsound.DllCanUnloadNow = GetProcAddress(dsound.dll, "DllCanUnloadNow");
	dsound.DllGetClassObject = GetProcAddress(dsound.dll, "DllGetClassObject");
	dsound.DirectSoundCaptureCreate = GetProcAddress(dsound.dll, "DirectSoundCaptureCreate");
	dsound.DirectSoundCaptureEnumerateA = GetProcAddress(dsound.dll, "DirectSoundCaptureEnumerateA");
	dsound.DirectSoundCaptureEnumerateW = GetProcAddress(dsound.dll, "DirectSoundCaptureEnumerateW");
	dsound.GetDeviceID = GetProcAddress(dsound.dll, "GetDeviceID");
	dsound.DirectSoundFullDuplexCreate = GetProcAddress(dsound.dll, "DirectSoundFullDuplexCreate");
	dsound.DirectSoundCreate8 = GetProcAddress(dsound.dll, "DirectSoundCreate8");
	dsound.DirectSoundCaptureCreate8 = GetProcAddress(dsound.dll, "DirectSoundCaptureCreate8");
}

char *stristr(const char *str1, const char *str2)
{
	char *cp = (char *)str1;
	char *s1;
	char *s2;

	if (!*str2) return (char *)str1;

	while (*cp)
	{
		s1 = cp;
		s2 = (char *)str2;

		while (*s1 && *s2 && !(tolower(*s1) - tolower(*s2)))
		{
			s1++;
			s2++;
		}

		if (!*s2) return cp;

		cp++;
	}

	return NULL;
}

intptr_t _findonce(char *filename, _finddata_t *finddata)
{
	intptr_t handle = _findfirst(filename, finddata);

	if (handle != -1)
	{
		_findclose(handle);
	}

	return handle;
}

void init(void)
{
	FILE *cfg = fopen("dsound.cfg", "r");

	_finddata_t fd;
	char cwd[MAX_PATH];
	_getcwd(cwd, sizeof(cwd));

	char save_cwd[MAX_PATH];
	strcpy(save_cwd, cwd);

	if (_findonce("language_x1.dll", &fd) == -1)
	{
		size_t cwd_len = strlen(cwd);
		size_t folder_len = strlen("\\age2_x1");

		if (cwd_len > folder_len + 2)
		{
			char *p = &cwd[cwd_len - folder_len];

			if (cwd[cwd_len - 1] == '\\') p--;

			if (!strnicmp(p, "\\age2_x1", sizeof("\\age2_x1")))
			{
				*p = '\0';
				_chdir(cwd);
			}

			GetModuleFileName(NULL, cwd, sizeof(cwd));

			p = stristr(cwd, "\\age2_x1");

			if (p)
			{
				*p = '\0';
				_chdir(cwd);
			}
		}
	}

	if (cfg)
	{
		char line[MAX_PATH];

		while (fgets(line, sizeof(line), cfg))
		{
			if (*line == ';' || *line == '#') continue;

			char *p = strrchr(line, '\r');
			if (!p) p = strrchr(line, '\n');
			if (p) *p = '\0';

			LoadLibrary(line);
		}

		fclose(cfg);
	}

	_chdir(save_cwd);
}

BOOL WINAPI DllMain(HINSTANCE hInst, DWORD reason, LPVOID)
{
	if (reason == DLL_PROCESS_ATTACH)
	{
		init_dsound(hInst);

		init();
	}

	if (reason == DLL_PROCESS_DETACH)
	{
		FreeLibrary(dsound.dll);
	}

	return TRUE;
}
