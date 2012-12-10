

#pragma
#include "GameAudio.h"


GameAudio::GameAudio()
{
	pXACT3Engine = NULL;
	pWaveBank = NULL;
	pMusicBank = NULL;
	pSoundBank = NULL;
	stopped = false;
	repeat = -1;
}

void WINAPI XACTNotificationCallback(const XACT_NOTIFICATION* pNotification);

HRESULT GameAudio::initialize(wchar_t* waveBankFile, wchar_t* musicBankFile, wchar_t* soundBankFile)
{
	//Setup variables

	HRESULT hr;
	HANDLE hFile;
	HANDLE musicFile;
	HANDLE hMapFile;
	DWORD dwFileSize, dwBytesRead;
	void* pbWaveBank, *pbMusicBank, *pbSoundBank;

	//Initialise the engine
	hr = XACT3CreateEngine(0, &pXACT3Engine);
	if(FAILED(hr) || pXACT3Engine == NULL)
		return E_FAIL;
	XACT_RUNTIME_PARAMETERS engineParameters = {0};
	engineParameters.fnNotificationCallback = XACTNotificationCallback;
	hr = pXACT3Engine->Initialize(&engineParameters);
	if(FAILED(hr))
		return hr;

	//Start the wave bank
	hr = E_FAIL;
	hFile = CreateFile(waveBankFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		dwFileSize = GetFileSize(hFile, NULL);
		if(dwFileSize != -1)
		{
			hMapFile = CreateFileMapping(hFile, NULL, PAGE_READONLY, 0, dwFileSize, NULL);
			if(hMapFile)
			{
				pbWaveBank = MapViewOfFile(hMapFile, FILE_MAP_READ, 0, 0, 0);
				if(pbWaveBank)
					hr = pXACT3Engine->CreateInMemoryWaveBank(pbWaveBank, dwFileSize, 0, 0, &pWaveBank);
				CloseHandle(hMapFile);
			}
		}
		CloseHandle(hFile);
	}
	if(FAILED(hr))
		return E_FAIL;

	//Start the music wave bank
	/*hr = E_FAIL;
	musicFile = CreateFile(musicBankFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED | FILE_FLAG_NO_BUFFERING, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		XACT_STREAMING_PARAMETERS* streamingParameters = new XACT_STREAMING_PARAMETERS;
		streamingParameters->file = hFile;
		streamingParameters->packetSize = 32;
		streamingParameters->offset = 0;
		streamingParameters->flags = 0;
		hr = pXACT3Engine->CreateStreamingWaveBank(streamingParameters, &pMusicBank);
	}
	if(FAILED(hr))
		return E_FAIL;*/

	//Start the sound bank
	hr = E_FAIL;
	hFile = CreateFile(soundBankFile, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if(hFile != INVALID_HANDLE_VALUE)
	{
		dwFileSize = GetFileSize(hFile, NULL);
		if(dwFileSize != -1)
		{
			pbSoundBank = new BYTE[dwFileSize];
			if(pbSoundBank)
				if(ReadFile(hFile, pbSoundBank, dwFileSize, &dwBytesRead, NULL) != 0)
					hr = pXACT3Engine->CreateSoundBank(pbSoundBank, dwFileSize, 0, 0, &pSoundBank);
		}
		CloseHandle(hFile);
	}
	if(FAILED(hr))
		return E_FAIL;

	return S_OK;
}

void GameAudio::doWork()
{
	if(stopped)
		stopMusic();
	pXACT3Engine->DoWork();
}

HRESULT GameAudio::playMusic()
{
	char* cue = musicSequence->front();
	XACTINDEX cuedex = pSoundBank->GetCueIndex(cue);
	HRESULT hr = pSoundBank->Play(cuedex, 0, 0, NULL);
	if(FAILED(hr))
		return E_FAIL;
	XACT_NOTIFICATION_DESCRIPTION* desc = new XACT_NOTIFICATION_DESCRIPTION();
	desc->type = XACTNOTIFICATIONTYPE_CUESTOP;
	desc->pSoundBank = pSoundBank;
	desc->cueIndex = cuedex;
	desc->pvContext = this;
	return pXACT3Engine->RegisterNotification(desc);
}

HRESULT GameAudio::playSound(char* cue)
{
	XACTINDEX cuedex = pSoundBank->GetCueIndex(cue);
	return pSoundBank->Play(cuedex, 0, 0, NULL);
}

HRESULT GameAudio::stopCue(char* cue)
{
	XACTINDEX cuedex = pSoundBank->GetCueIndex(cue);
	return pSoundBank->Stop(cuedex, 0);
}

HRESULT GameAudio::stopSound(char* cue)
{
	return stopCue(cue);
}

HRESULT GameAudio::stopMusic()
{
	stopped = false;
	if(repeat == 0)
		musicSequence->push(musicSequence->front());
	else if(repeat > 0)
		repeat--;
	else if(repeat == -2)
		return S_OK;
	musicSequence->pop();
	if(musicSequence->size() > 0)
		return playMusic();
	return S_OK;
}

HRESULT GameAudio::manualStopMusic()
{
	return stopCue(musicSequence->front());
}

HRESULT GameAudio::shutdown()
{
	HRESULT hr = pXACT3Engine->ShutDown();
	if(FAILED(hr))
		return E_FAIL;
	return S_OK;
}

void GameAudio::setStop(bool initAutoStopped)
{
	stopped = initAutoStopped;
}

bool GameAudio::isStopped()
{
	return stopped;
}

void GameAudio::setMusicSequence(queue<char*>* initSequence, int initrepeat)
{
	musicSequence = initSequence;
	repeat = initrepeat;
}

void GameAudio::setMusicSequence(queue<char*>* initSequence)
{
	string repeatString = initSequence->front();
	int repeat = atoi(repeatString.c_str());
	initSequence->pop();
	setMusicSequence(initSequence, repeat);
}

void GameAudio::stopMusicSequence()
{
	repeat = -2;
	stopCue(musicSequence->front());
}


//SO I CAN'T DO JACK SHIT WITH THIS FUNCTION BECAUSE IT'S GODDAMN STATIC AND HAS TO BE GODDAMN STATIC. WHY THE HELL
//DOES IT EVEN EXIST!? WHY DID MICROSOFT ASSUME THAT WE WOULD ALWAYS BE CALLING EXACT FROM A STATIC CONTEXT!?
void WINAPI XACTNotificationCallback(const XACT_NOTIFICATION* pNotification)
{
	if( pNotification->type == XACTNOTIFICATIONTYPE_CUESTOP )
	{
		GameAudio* audio = (GameAudio*)pNotification->pvContext;
		audio->setStop(true);
	}

	// ....
}
