<h1>About</h1>
This is an open source computer with a custom language called TimCode and a DOS-like operating system called HamburgerOS<br>

It uses the <b>esp32-s3</b> chip with the <b>ILI9341</b> tft screen as a display.<br>
<img src="/computer.jpg" height="200px"><br><br>

I made a custom programming language called <b>TimCode</b> for it so i can code the apps without compiling them.<br>
This also makes it easy to change the apps, as you don't have to change the firmware<br><br>

You can edit the project, or make a program in TimCode and i will make a video about it.<br>
Critique and questions appreciated. <br><br>


<h1>Price</h1>
<h6>I bought everything off aliexpress, so it's pretty cheap<br> Note: Prices are without shipping</h6>
<ul>
  <li><a href="https://aliexpress.ru/item/1005005046708221.html">ESP32 s3 - 345₽ / 4.29$</a></li>
  <li><a href="https://aliexpress.ru/item/1005004147439619.html">3.5in display - 471₽ / 5.86$</a></li>
  <li><a href="https://aliexpress.ru/item/1654409370.html">SD module - 36₽ / 0.45$</a></li>
  <li><h6>keyboard not ready yet</h6></li>
</ul><br><br>



<h1>Console commands</h1>
<h6>parameter(p) means optional.</h6>

<h2>Basic commands</h2

<h4>info - no parameters</h4>
Shows an ASCII art of a hamburger with info and time<br><br>

<h4>beep - tone(p), time(p)</h4>
Makes a square wave with provided parameters<br><br>

<h4>wait - time</h4>
Delays the system by a given amount of time (ms)<br><br>

<h4>mode - no parameters</h4>
Toggles beetwen Console and TimCode modes<br><br>

<h4>echo - ypos, string</h4>
Displays the text onscreen in the given y position (will be updated)<br><br>

<h4>clr - no parameters</h4>
Clears the screen and display the current mode (if out is enabled)<br><br>

<h4>noout - no parameters</h4>
Toggles beetwen out mode (true, false)<br><br>

<h4>goto - line</h4>
(used in TimCode) Sets the current line to provided<br><br>

<h4>timeset - second, minute, hour, day, month (single digit), year (two digits)</h4>
Sets the RTC module time to provided<br><br>

<h4>reset - no parameters</h4>
Resets the esp32<br><br><br>

<h2>Graphics commands</h2>

<h4>drawpoint - x, y, r, g, b, a</h4>
Draws a single point (pixel) onscreen<br><br>

<h4>drawline - x1, y1, x2, y2, r, g, b, a</h4>
Draws a line onscreen<br><br>

<h4>drawrect - x, y, w, h, r, g, b, a</h4>
Draws a rectangle with a given width and height<br><br>

<h4>fillrect - x, y, w, h, r, g, b, a</h4>
Draws a filled rectangle with a given width and height<br><br>

<h4>drawtri - x1, y1, x2, y2, x3, y3, r, g, b, a</h4>
Draws a triangle<br><br>

<h4>filltri - x1, y1, x2, y2, x3, y3, r, g, b, a</h4>
Draws a filled triangle<br><br>

<h4>drawcircle - x, y, rad, r,g,b,a</h4>
Draws a circle with a given radius<br><br>

<h4>fillcircle - x, y, rad, r,g,b,a</h4>
Draws a filled circle with a given radius<br><br>


<h2>SD commands</h2>

<h4>sdstart - no parameters</h4>
Starts the SD module<br><br>

<h4>sdlist - dir</h4>
Displays the directory contents<br><br>

<h4>sdmkdir - dir</h4>
Creates a directory<br><br>

<h4>sdrmdir - dir</h4>
Deletes a directory<br><br>

<h4>sdopen - dir</h4>
Opens a file and displays the contents in ASCII<br><br>

<h4>sdopen, sdappend - dir, content</h4>
Opens and writes to file. If file is not present, creates it. (basically the same)<br><br>

<h4>sdremove - dir</h4>
Deletes a file<br><br>
