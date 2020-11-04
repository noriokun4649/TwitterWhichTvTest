
#include"stdafx.h"
#include "resource.h"
#include <time.h>

class TwitterWhichTvTestPlugin : public TVTest::CTVTestPlugin
{
	static LRESULT CALLBACK EventCallback(UINT Event, LPARAM lParam1, LPARAM lParam2, void* pClientData);

public :
	bool GetPluginInfo(TVTest::PluginInfo* pInfo) override;
	bool Initialize() override;
	bool Finalize() override;

private:
	bool pluginState;
	static const int PANEL_ID = 1;
};

bool TwitterWhichTvTestPlugin::GetPluginInfo(TVTest::PluginInfo* pInfo)
{
	pInfo->Type = TVTest::PLUGIN_TYPE_NORMAL;
	pInfo->Flags = 0;
	pInfo->pszPluginName = L"Twitter Which TvTest";
	pInfo->pszCopyright = L"(c) 2020 noriokun4649";
	pInfo->pszDescription = L"TvTestから直接TvTestのスクショ画像をTwitterへ投稿可能です。アカウントとの連携は設定からお願いします。";
	//TwitterGetRequestToken();
	return true;
}

bool TwitterWhichTvTestPlugin::Initialize() {
	// パネル項目を登録する
	TVTest::PanelItemInfo PanelInfo;

	PanelInfo.Size = sizeof(TVTest::PanelItemInfo);
	PanelInfo.Flags = 0;
	PanelInfo.Style = TVTest::PANEL_ITEM_STYLE_NEEDFOCUS;
	PanelInfo.ID = PANEL_ID;
	PanelInfo.pszIDText = L"Twitter";
	PanelInfo.pszTitle = L"Twitterパネル";
	PanelInfo.hbmIcon = (HBITMAP)::LoadImage(g_hinstDLL, MAKEINTRESOURCE(IDB_ICON),
		IMAGE_BITMAP, 0, 0, LR_CREATEDIBSECTION);

	bool fResult = m_pApp->RegisterPanelItem(&PanelInfo);
	::DeleteObject(PanelInfo.hbmIcon);
	if (!fResult) {
		m_pApp->AddLog(L"パネル項目を登録できません。", TVTest::LOG_TYPE_ERROR);
		return false;
	}
	return true;
}

bool TwitterWhichTvTestPlugin::Finalize() {
	return true;
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
		if (fEnable) {

		}
		else
		{
			pThis->pluginState = false;
		}
		return TRUE;
	case TVTest::EVENT_AUDIOSTREAMCHANGE:
		return TRUE;
	case TVTest::EVENT_SERVICEUPDATE:
		return TRUE;
	}
	return 0;
}