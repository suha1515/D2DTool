#pragma once
class CSoundMgr
{
	DECLARE_SINGLETON(CSoundMgr)

public:
	enum CHANNEL_ID { BGM, PLAYER, MONSTER, EFFECT, MAX_CHANNEL };

private:
	CSoundMgr();
	~CSoundMgr();

public:
	void Initialize();
	void UpdateSound();
	void PlaySound(const wstring& wstrSoundKey, CHANNEL_ID eID);
	void PlayBGM(const wstring& wstrSoundKey);
	void StopSound(CHANNEL_ID eID);
	void StopAll();
	void SetVolume(CHANNEL_ID eID, float fVol);
	void Release();

private:
	void LoadSoundFile();

private:
	// FMOD_SOUND: 사운드 파일에 대한 정보를 갖는 구조체.
	map<wstring, FMOD_SOUND*>	m_MapSound;

	// FMOD_CHANNEL: 사운드를 재생하고 관리하는 역할. ex) 볼륨 조절 등.
	FMOD_CHANNEL*	m_pChannelArr[MAX_CHANNEL];

	// FMOD_SYSTEM: FMOD_SOUND과 FMOD_CHANNEL을 총괄 관리하는 객체.
	FMOD_SYSTEM*	m_pSystem;
};	