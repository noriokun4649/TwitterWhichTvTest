#pragma once

#define WIN32_LEAN_AND_MEAN             // Windows ヘッダーからほとんど使用されていない部分を除外する
// Windows ヘッダー ファイル
#include <windows.h>
#include <string>
#include <twitterlib/twitterlib.hpp>
#include <dshow.h>
#include <cstdint>
#include <tchar.h>
#define TVTEST_PLUGIN_CLASS_IMPLEMENT	// プラグインをクラスとして実装
#include "TVTestPlugin.h"
#include "strconv.h"