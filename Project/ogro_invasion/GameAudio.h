

#pragma once
#include <xact3.h>
#include <queue>
using namespace std;

class GameAudio
{
private:

	// THE ENGINE CONTAINS ALL DATA STRUCTURES RELEVANT TO PLAYING SOUND
	IXACT3Engine* pXACT3Engine;

	// THE WAVE BANK AND SOUND BANK ARE USED TO TELL THE ENGINE TO PLAY SOUND
	IXACT3WaveBank* pWaveBank;
	IXACT3WaveBank* pMusicBank;
	IXACT3SoundBank* pSoundBank;

	// FLAG THAT TELLS THE ENGINE IF MUSIC HAS BEEN STOPPED
	bool stopped;

	// SEQUENCE OF MUSIC TO BE PLAYED
	queue<char*>* musicSequence;

	// HOW MANY FILES IN THE SEQUENCE BEFORE IT STARTS REPEATING
	// -1 MEANS THAT THE SEQUENCE NEVER REPEATS
	// -2 MEANS THAT THE SEQUENCE SHOULDN'T BE PLAYED
	int repeat;

	// FUNCTIONS DEFINED IN GameAudio.cpp
	HRESULT stopCue(char* cue);
	HRESULT stopMusic();

public:
	// INLINED ACCESSOR FUNCTIONS
	~GameAudio()	{ shutdown();	}
	queue<char*>* getMusicSequence()	{ return musicSequence;	}

	// FUNCTIONS DEFINED IN GameAudio.cpp
	GameAudio();
	HRESULT initialize(wchar_t* waveBankFile, wchar_t* musicBankFile, wchar_t* soundBankFile);
	void doWork();
	HRESULT playSound(char* cue);
	HRESULT playMusic();
	HRESULT stopSound(char* cue);
	HRESULT manualStopMusic();
	void stopMusicSequence();
	HRESULT shutdown();
	void setStop(bool initStopped);
	bool isStopped();
	void setMusicSequence(queue<char*>* initSequence, int initrepeat);
	void setMusicSequence(queue<char*>* initSequence);
};
