// BonRecTest.cpp : �R���\�[�� �A�v���P�[�V�����̃G���g�� �|�C���g���`���܂��B
//

#include "stdafx.h"
#include "BonRecTest.h"

bool isSignal = false;

unsigned int __stdcall WaitThread(void *pParam)
{
	getchar();

	isSignal = true;

	return 0;
}

BOOL WINAPI HandlerRoutine(DWORD dwCtrlType)
{
	isSignal = true;

	return TRUE;
}

void usage()
{
	std::cerr << "Usage: BonRecTest.exe --driver BonDriver.dll --output output.ts [--space space] [--channel channel]" << std::endl;
}

int _tmain(int argc, TCHAR* argv[])
{
	std::cerr << "* BonRecTest *" << std::endl;

	if (argc < 2) {
		usage();

		return 0;
	}

	CBonRecTest pBonRecTest;
	HANDLE hWaitThread;

	TCHAR *pEnd;
	bool used = false;

	for (int i = 1; i < argc - 1; i++) {
		if (used) {
			used = false;
			continue;
		}

		if (!_tcscmp(argv[i], TEXT("--driver"))) {
			pBonRecTest.bonDriverPath = argv[i + 1];
			used = true;
		}
		else if (!_tcscmp(argv[i], TEXT("--decoder"))) {
			pBonRecTest.decoderPath = argv[i + 1];
			used = true;
		}
		else if (!_tcscmp(argv[i], TEXT("--output"))) {
			pBonRecTest.outputPath = argv[i + 1];
			used = true;
		}
		else if (!_tcscmp(argv[i], TEXT("--space"))) {
			pBonRecTest.space = _tcstoul(argv[i + 1], &pEnd, 10);
			used = true;
		}
		else if (!_tcscmp(argv[i], TEXT("--channel"))) {
			DWORD channel = _tcstoul(argv[i + 1], &pEnd, 10);

			if (channel < 10000) {
				pBonRecTest.channel = channel;
			}
			else {
				pBonRecTest.space = channel / 10000UL;
				pBonRecTest.channel = channel % 10000UL;
			}
			used = true;
		}
		else if (!_tcscmp(argv[i], TEXT("--emm"))) {
			pBonRecTest.emm = true;
		}
		else if (!_tcscmp(argv[i], TEXT("--log"))) {
			pBonRecTest.log = true;
		}
	}

	if (!pBonRecTest.Start()) {
		return -1;
	}

	hWaitThread = (HANDLE)_beginthreadex(NULL, 0, WaitThread, NULL, 0, NULL);

	SetConsoleCtrlHandler(*HandlerRoutine, true);
	while (!isSignal) Sleep(100UL);

	TerminateThread(hWaitThread, -1);
	CloseHandle(hWaitThread);
	hWaitThread = NULL;

	std::cerr << "Abort!" << std::endl;

    return 0;
}

