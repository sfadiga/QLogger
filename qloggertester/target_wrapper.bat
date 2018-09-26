@echo off
SetLocal EnableDelayedExpansion
(set PATH=D:\Dev\Qt\5.11.2\msvc2017_64\bin;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=D:\Dev\Qt\5.11.2\msvc2017_64\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=D:\Dev\Qt\5.11.2\msvc2017_64\plugins
)
%*
EndLocal
