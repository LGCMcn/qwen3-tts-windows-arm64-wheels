@echo off
cd /d "%~dp0"
"%~dp0runtime\python.exe" -I "%~dp0app.py" %*
if errorlevel 1 pause


