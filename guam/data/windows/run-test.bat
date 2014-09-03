@echo off
@echo. > tmp\debug.log
@set LOG4CPP_CONFIG=data\debug.properties
start tmp\build\test\test.exe 1>tmp\stdout.log 2>tmp\stderr.log
