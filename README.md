# Wibbly-Wobbly-Timey-Wimey

<center>
  <img src="_img/project_front_face.jpg" width="47%" alt="A picture of the project" style="border-radius: 5px;"/>
  <img src="_img/project_back_face.jpg" width="47%" alt="A picture of the project" style="border-radius: 5px;"/>
  <br />
  <i>The TARDIS that need to be defused.</i>
</center>
<br />

<div style="text-align: justify">
"Wibbly-Wobbly Timey-Wimey" is a bomb defusal escape game inspired by the popular television series <a href="https://fr.wikipedia.org/wiki/Doctor_Who" target="_blank">Doctor Who</a>. This game draws inspiration from the mechanics of <a href="https://keeptalkinggame.com/" target="_blank">Keep Talking And Nobody Explodes</a>. The primary objective of our game is to defuse the TARDIS by solving four small puzzles within a 45-minute time limit. To assist players in resolving these puzzles, the game includes a documentation guide.
</div>

## The project
<div style="text-align: justify">
This project is an engineering school assignment completed over one semester in groups of three. The instructions for this project were to create something playable, drawing inspiration from a movie or a series. Given my strong interest in escape games, the idea of developing a game similar to <a href="https://keeptalkinggame.com/" target="_blank">KTaNE</a> quickly crossed my mind. The challenge, however, was to connect this concept with a movie. Fortunately, one member of our group was a passionate fan of Doctor Who, and that's when the project truly took off!
</div>

## How to play
<div style="text-align: justify">
The game is designed to be played by two players: one manipulating the bomb, and the other reading the manual. Both players can see the bomb and the manual. If you want to increase the difficulty, you can play separately, with one person handling the bomb and the other reading the manual.
</div>

## The defuse manual
<div style="text-align: justify">
The manual and page layout were entirely created by my two friends. I'm not really proficient with Word and Doctor Who... Here is the manual:
</div>

- [Wibbly-Wobbly Timey-Wimey Manual.pdf](Wibbly-Wobbly_Timey-Wimey_Manual.pdf)


# Puzzle description and working principle

<p align="center">
  <img src="_img/tardis.png" alt="Schematic of the TARDIS" style="border-radius: 5px; max-height: 300px;"/>
  <br />
  <i>Schematic of the TARDIS.</i>
</p>

<div style="text-align: justify">
The game is composed of 4 defusable puzzles that need to be defused to win the game. Each puzzle is independent of the others, and every solution is in the manual.

<br /><br />
<center>
  <img src="_img/electronic.jpg" width=32% alt="The electronic components of the TARDIS" style="border-radius: 5px;"/>
  <img src="_img/electronic1.jpg" width=32% alt="The electronic components of the TARDIS" style="border-radius: 5px;"/>
  <img src="_img/electronic2.jpg" width=32% alt="The electronic components of the TARDIS" style="border-radius: 5px;"/>
  <br />
  <i>The electronic components of the TARDIS.</i>

</center>
<br />

This project uses an Arduino MEGA as the main microcontroller. Each module consists of a custom-made electronic circuit soldered onto perfboard. The entire system is powered by four lithium-ion 18650 batteries arranged in series, allowing the project to run continuously for 45 minutes.
</div>


## The Timer
<center>
    <video style="border-radius: 5px; max-height: 300px;" autoplay muted loop controls>
    <source src="_img/timer.mp4" type="video/mp4">
    </video>
</center>

<div style="text-align: justify">
The timer module is a simple 4-digit 7-segment display that indicates the remaining time of the game.
</div>

## The Joystick
<center>
    <video style="border-radius: 5px; max-height: 300px;" autoplay muted loop controls>
    <source src="_img/joystick.mp4" type="video/mp4">
    </video>
</center>

<div style="text-align: justify">
The joystick puzzle consists of a joystick and a 16-LED ring that displays a binary code. Based on the code shown by the ring, the user needs to identify the correct directional sequence in the manual and replicate it on the joystick to solve and unlock the puzzle.
</div>

## The LCD
<center>
    <video style="border-radius: 5px; max-height: 300px;" autoplay muted loop controls>
    <source src="_img/lcd.mp4" type="video/mp4">
    </video>
</center>

<div style="text-align: justify">
The LCD puzzle consists of an LCD screen and four buttons. Two symbols and a group of letters are displayed on the LCD. The user needs to decipher the word based on the two symbols, using the manual to unlock this puzzle.
</div>

## Wires
<p align="center">
  <img src="_img/wire.png" alt="wire module" style="border-radius: 5px; max-height: 300px;"/>
  <br />
</p>

<div style="text-align: justify">
To unlock this puzzle, you need to cut the correct combination of wires, as indicated in the manual. Be careful, as cutting a wire that was not meant to be cut will lock the puzzle forever! :D 
</div>

## The Gyroscope
<center>
    <video style="border-radius: 5px; max-height: 300px;" autoplay muted loop controls>
    <source src="_img/gyro.mp4" type="video/mp4">
    </video>
</center>

<div style="text-align: justify">
The gyroscope module is pretty easy to defuse. You need to rotate the TARDIS to move a dot on the LED matrix. This dot needs to be placed on the other static dot to validate a step. You will need to do this 5 times to defuse this module.
</div>

# 3D Project
<center>
    <video style="border-radius: 5px; max-height: 300px;" autoplay muted loop controls>
    <source src="_img/3D_project_build.mp4" type="video/mp4">
    </video>
</center>

<div style="text-align: justify">
The structure of the project is entirely 3D printed using a Sidewinder X1 and a Wanhao D12. One of the instructions for this project was that it needed to be demountable, so we couldn't use glue to attach components or anything (I really don't like glue anyway, ahah!).
</div>

## The base
<center>
  <img src="_img/structure.png" alt="3D model of base structure" style="border-radius: 5px; max-height: 300px;"/>
  <br />
  <i>3D model of the TARDIS base structure.</i>
</center>
<br />
<div style="text-align: justify">
The main structure of the TARDIS consists of two identical bases held together by four metal rods cut to size and press-fitted inside four holes in those bases. This structure is exceptionally strong and can support my weight (70 kg) in compression, and likely even more.
</div>

## Each faces
<center>
  <img src="_img/front_face.png" alt="3D model of the front face" style="border-radius: 5px; max-height: 300px;"/>
  <br />
  <i>3D model of the TARDIS front face.</i>
  <br />
  <br />
  <img src="_img/back_face.png" alt="3D model of the back face" style="border-radius: 5px; max-height: 300px;"/>
  <br />
  <i>3D model of the back TARDIS back face.</i>
</center>
<br />

<div style="text-align: justify">
Every module is distributed on the front and back surfaces of the TARDIS. Designing these surfaces was a nightmare, as each module had to fit into a very tight space. Additionally, we had to incorporate our clipping mechanism to ensure that everything securely holds onto these surfaces.
</div>

## Clipping method
<div style="text-align: justify">
Since glue was forbidden, I had to come up with an idea to secure every custom-made perfboard to the front and back faces. The initial concept involved creating small chamfers to insert the perfboard and secure it in place using clips. This method worked quite well for a single attachment, but after removing the board, the chamfers tended to wear out, rendering them ineffective for subsequent uses.

<br />
<p align="center">
  <img src="_img/old_clip.png" alt="The abandonned clipping method" style="border-radius: 5px; max-height: 300px;"/>
  <br />
  <i>The abandonned clipping method. </i>
</p>

I didn't like the idea of reprinting a 6-hour-long piece just because one clip is worn. So I came up with another idea. Each board is held in place by small 3D printed pieces that fit into notches.

<p align="center">
  <img src="_img/clip.png" alt="The really good clipping method" style="border-radius: 5px; max-height: 300px;"/>
  <br />
  <i>The really good (clever? 🫣) clipping method. </i>
</p>

Here is one of those pieces that is heated to become deformable, allowing us to install it inside those notches. Once installed, we let these pieces cool down and regain their stiffness. And TADAAAA! In the event that we need to remove a module, we can cut these small pieces with a cutting plier. If we want to reinstall the module, one piece takes up to three minutes to print (Wayyyy better than 6 hours), making it possible to reinstall the module.
</div>


# Conclusion
<div style="text-align: justify">
This project is by far the most advanced I have undertaken so far. I truly enjoyed imagining puzzles and bringing them to life using electronics and code. Additionally, we received a really good grade for this project! :D
</div>

# Licence
- romainflcht