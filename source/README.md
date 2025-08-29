<h1>PLEASE HELP ME WITH THE CODE!</h1>
For some reason the sd card doesn't work properly, so it would be great if you helped me fix it!<br><br>


<h1>Console and TimCode</h1>
TimCode and the console are linked, to run TimCode code, every line gets sent to console, and processed like a normal command<br>

To add or edit an existing command, go to console.cpp line 300, and there you will find a very long "if" statement.<br>
It checks the command name and processes it. To add a command, simply add "else if(commandf == "your_command_name")"<br><br>
(also i know, this is a stupid approach, but i don't know another way to do it.) <br>

<h1>Storage</h1>
This class uses SD.h library to manipulate the sd card. I don't recommend anyone touch this (even i am scared).
