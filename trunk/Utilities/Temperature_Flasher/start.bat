echo off
set comportnumber=5
echo The COM port = %comportnumber% 
call flasher.bat %comportnumber% < enter.txt
pause