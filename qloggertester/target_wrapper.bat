@echo off
SetLocal EnableDelayedExpansion
(set PATH=C:\Qt\5.11.1\mingw53_32\bin;!PATH!)
if defined QT_PLUGIN_PATH (
    set QT_PLUGIN_PATH=C:\Qt\5.11.1\mingw53_32\plugins;!QT_PLUGIN_PATH!
) else (
    set QT_PLUGIN_PATH=C:\Qt\5.11.1\mingw53_32\plugins
)
%*
EndLocal
