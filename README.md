# face_recognition
C++ program that recognises faces from AT&amp;T Database of Faces by comparing N fixed but randomly chosed pixels of each picture

first lines in main:  <br />
nop - number of random pixels (1 to 92*112) <br />
people - number of people (1 to 40) <br />
s - ordinal number of person to compare with others (1 to 40) <br />
n - number of picture of person "s" to compare with others first pictures (2 to 10) <br />

in result: <br />
A smaller number means a greater match so if the "s" number is the smallest - program made a correct choice
