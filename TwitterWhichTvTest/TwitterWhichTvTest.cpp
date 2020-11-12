
#include"stdafx.h"
#include "resource.h"
#include <time.h>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")

class TwitterWhichTvTestPlugin : public TVTest::CTVTestPlugin
{
	static LRESULT CALLBACK EventCallback(UINT Event, LPARAM lParam1, LPARAM lParam2, void* pClientData);
	static INT_PTR CALLBACK SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam, void* pClientData);
	bool ShowDialog(HWND hwndOwner, int id);

public :
	bool GetPluginInfo(TVTest::PluginInfo* pInfo) override;
	bool Initialize() override;
	bool Finalize() override;

private:
	void RegisterCommand(int ID, LPCWSTR pszText, LPCWSTR pszName, LPCWSTR pszDescription, int IconID = 0);
	bool OnPluginCommand(int Command);
	bool InitSettings();
	TCHAR m_szIniFileName[MAX_PATH];
	bool conf_mode = false;
	bool conf_isFinalized = false;
	static const int PANEL_ID = 1;
	enum {
		OPEN_TWEET_WINDOW = 1,
		TWEET_NOW_IMAGE_ONLY,
		TWEET_NOW
	};
};

bool TwitterWhichTvTestPlugin::GetPluginInfo(TVTest::PluginInfo* pInfo)
{
	pInfo->Type = TVTest::PLUGIN_TYPE_NORMAL;
	pInfo->Flags = TVTest::PLUGIN_FLAG_HASSETTINGS;
	pInfo->pszPluginName = L"Twitter Which TvTest";
	pInfo->pszCopyright = L"(c) 2020 noriokun4649";
	pInfo->pszDescription = L"TvTest���璼��TvTest�̃X�N�V���摜��Twitter�֓��e�\�ł��B�A�J�E���g�Ƃ̘A�g�͐ݒ肩�炨�肢���܂��B";
	//TwitterGetRequestToken();
	return true;
}

bool TwitterWhichTvTestPlugin::Initialize() {
	//TwitterWhichTvTest�A�C�R���o�^����
	m_pApp->RegisterPluginIconFromResource(g_hinstDLL, MAKEINTRESOURCE(IDB_ICON));

	//�R�}���h�o�^����
	RegisterCommand(OPEN_TWEET_WINDOW, L"OpenTweetWindow", L"�c�C�[�g��ʊJ��", L"�c�C�[�g�E�B���h�E���J���܂��B", IDB_ICON);
	RegisterCommand(TWEET_NOW_IMAGE_ONLY, L"TweetNowImageOnly", L"�������c�C�[�g(�摜�̂�)", L"��ʂ��L���v�`�����c�C�[�g���܂��B�摜�݂̂̃c�C�[�g���s���܂��B", IDB_ICON);
	RegisterCommand(TWEET_NOW, L"TweetNow", L"�������c�C�[�g", L"��ʂ��L���v�`�����c�C�[�g���܂��B");


	// �p�l�����ڂ�o�^����
	TVTest::PanelItemInfo PanelInfo;

	PanelInfo.Size = sizeof(TVTest::PanelItemInfo);
	PanelInfo.Flags = 0;
	PanelInfo.Style = TVTest::PANEL_ITEM_STYLE_NEEDFOCUS;
	PanelInfo.ID = PANEL_ID;
	PanelInfo.pszIDText = L"Twitter";
	PanelInfo.pszTitle = L"Twitter�p�l��";
	PanelInfo.hbmIcon = (HBITMAP)::LoadImage(g_hinstDLL, MAKEINTRESOURCE(IDB_ICON),
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	bool fResult = m_pApp->RegisterPanelItem(&PanelInfo);
	::DeleteObject(PanelInfo.hbmIcon);
	if (!fResult) {
		m_pApp->AddLog(L"�p�l�����ڂ�o�^�ł��܂���B", TVTest::LOG_TYPE_ERROR);
		return false;
	}

	//�C�x���g�R�[���o�b�N�o�^����
	m_pApp->SetEventCallback(EventCallback, this);

	return true;
}

bool TwitterWhichTvTestPlugin::Finalize() {
	return true;
}

bool TwitterWhichTvTestPlugin::InitSettings() {
	::GetModuleFileName(g_hinstDLL, m_szIniFileName, MAX_PATH);
	::PathRenameExtension(m_szIniFileName, TEXT(".ini"));
	conf_mode = ::GetPrivateProfileInt(TEXT("Settings"), TEXT("Mode"), conf_mode, m_szIniFileName) != 0;
	m_pApp->AddLog(m_szIniFileName);
	conf_isFinalized = true;
	return true;
}


// �v���O�C���̃R�}���h��o�^����
void TwitterWhichTvTestPlugin::RegisterCommand(
	int ID, LPCWSTR pszText, LPCWSTR pszName, LPCWSTR pszDescription, int IconID)
{
	TVTest::PluginCommandInfo CommandInfo;

	CommandInfo.Size = sizeof(CommandInfo);
	CommandInfo.Flags = 0;
	CommandInfo.State = 0;
	CommandInfo.ID = ID;
	CommandInfo.pszText = pszText;
	CommandInfo.pszName = pszName;
	CommandInfo.pszDescription = pszDescription;
	if (IconID != 0) {
		CommandInfo.Flags |= TVTest::PLUGIN_COMMAND_FLAG_ICONIZE;
		CommandInfo.hbmIcon =
			static_cast<HBITMAP>(
				::LoadImage(g_hinstDLL, MAKEINTRESOURCE(IconID),
					IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION));
	}
	else {
		CommandInfo.hbmIcon = nullptr;
	}

	m_pApp->RegisterPluginCommand(&CommandInfo);

	::DeleteObject(CommandInfo.hbmIcon);
}


// �v���O�C���̃R�}���h�����s����
bool TwitterWhichTvTestPlugin::OnPluginCommand(int Command)
{
	auto WindowHand = CTVTestPlugin::m_pPluginParam->hwndApp;
	switch (Command) {
	case OPEN_TWEET_WINDOW:
		return ShowDialog(WindowHand, IDD_TWEET);
	}
	return false;
}


bool TwitterWhichTvTestPlugin::ShowDialog(HWND hwndOwner, int id) {

	TVTest::ShowDialogInfo Info;
	Info.Flags = 0;
	Info.hinst = g_hinstDLL;
	Info.pszTemplate = MAKEINTRESOURCE(id);
	Info.pMessageFunc = SettingsDlgProc;
	Info.pClientData = this;
	Info.hwndOwner = hwndOwner;

	return m_pApp->ShowDialog(&Info) == IDOK;
}

INT_PTR CALLBACK TwitterWhichTvTestPlugin::SettingsDlgProc(HWND hDlg, UINT uMsg, WPARAM wParam, LPARAM lParam, void* pClientData) {
	TwitterWhichTvTestPlugin* pThis = static_cast<TwitterWhichTvTestPlugin*>(pClientData);
	switch (uMsg)
	{
	case WM_INITDIALOG:
		::CheckDlgButton(hDlg, IDC_CHECK1, pThis->conf_mode ? BST_CHECKED : BST_UNCHECKED);
		return TRUE;
	case WM_COMMAND:
		switch (LOWORD(wParam)) {
		case IDOK:
			pThis->conf_mode = ::IsDlgButtonChecked(hDlg, IDC_CHECK1) == BST_CHECKED;
			if (pThis->conf_isFinalized) {
				struct IntString {
					IntString(int Value) { ::wsprintf(m_szBuffer, TEXT("%d"), Value); }
					operator LPCTSTR() const { return m_szBuffer; }
					TCHAR m_szBuffer[16];
				};

				::WritePrivateProfileString(TEXT("Settings"), TEXT("Mode"), IntString(pThis->conf_mode), pThis->m_szIniFileName);
			}
		case IDCANCEL:
			::EndDialog(hDlg, LOWORD(wParam));
			return TRUE;
		}
	default:
		break;
		return FALSE;
	}
	return FALSE;
}


TVTest::CTVTestPlugin* CreatePluginClass()
{
	return new TwitterWhichTvTestPlugin;
}

LRESULT CALLBACK TwitterWhichTvTestPlugin::EventCallback(UINT Event, LPARAM lParam1, LPARAM lParam2, void* pClientData)
{
	TwitterWhichTvTestPlugin* pThis = static_cast<TwitterWhichTvTestPlugin*>(pClientData);
	bool fEnable = lParam1 != 0;
	switch (Event)
	{
	case TVTest::EVENT_PLUGINENABLE:
		pThis->InitSettings();
		return TRUE;
	case TVTest::EVENT_AUDIOSTREAMCHANGE:
		return TRUE;
	case TVTest::EVENT_SERVICEUPDATE:
		return TRUE;
	case TVTest::EVENT_COMMAND:
		// �R�}���h���I�����ꂽ
		return pThis->OnPluginCommand((int)lParam1);
	case TVTest::EVENT_PLUGINSETTINGS:
		return pThis->ShowDialog(reinterpret_cast<HWND>(lParam1), IDD_SETING);
	}
	return 0;
}