
<h3 align="center">EE14 Final Project</h3>
  <p align="center">
    A BlackJack game
    <br />
    <a href="https://docs.google.com/document/d/1wWvgXe7Ve40gX6k7Nhl1zVf8bp4OxohAfwSJsb4MSrk/edit#"><strong>Explore the docs »</strong></a>
    <br />
    <br />
  </p>
</div>

<!-- GETTING STARTED -->
## Getting Started

To set up this on Tufts halligan server. The best way to do this on your lab computer.

* Open command prompt and naviage to the Z drive (Tufts server) using Z:
* Create a folder to hold EE14 Project
* cd into the folder and type "code ." to open the directory on VSCode.
* Open VSCode terminal and type "git clone (url)".
* A web browser should pop off and tell you to sign in. (I suggest the sign in using code option)
* Follow the instruction then everything should be setup on the lab computer and your halligan server. 

Notes: Remember that compilation should still be done using Keil. Development using VSCode might be possible but file creation and compilation must be done in Keil.

## COMMIT RULES:
- [add] (msg)
- [delete] (msg)
- [fix] (msg)
- [test] (msg)

### Use case:
Add: Adding new piece of code\
Delete: Deleting existing code\
Fix: Fixing/modifying existing code\
Test: Code that does not change the functionality of the program (for example,
        debugging outputs, commenting out sections of code, etc.)\

Please inlcude 1 sentence of message for each commit that is readable and understandable by everyone!

### Folder usage:
DO NOT EVER MODIFY THE TEMPLATE FILE\
Duplicate the template file if you want to start a test on something.\
FINAL_PROJ will be where the final production code is located \
PRINT_TEST will be where tests related to printing function on UART be \
Feel free to duplicate a new folder to test out stuff.
Notes: If you are familiar with git brance, feel free to use it as alternative. HOWEVER, using folder and copy code that is guaranteed to be corrected to FINAL_PROJ will be a better option to avoid pull request mess.
