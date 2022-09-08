# face_recognition
C++ program that recognises faces from AT&amp;T Database of Faces by comparing N fixed but randomly chosed pixels of each picture

first lines in main:
nop - number of random pixels (1 to 92*112)
people - number of people (1 to 40)
s - ordinal number of person to compare with others (1 to 40)
n - number of picture of person "s" to compare with others first pictures (1 to 10)

in result:
A smaller number means a greater match so if the "s" number is the smallest - program made a correct choice
