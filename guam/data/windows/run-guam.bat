@echo off
@echo. > tmp\debug.log
@set LOG4CPP_CONFIG=data\debug.properties
start tmp\build\guam\guam.exe 1>tmp\stdout.log 2>tmp\stderr.log
