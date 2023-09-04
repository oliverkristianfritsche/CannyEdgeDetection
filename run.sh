input="garb34.pgm"
output1="pics/convX.pgm"
output2="pics/convY.pgm"
output3="pics/Mag.pgm"
output4="pics/Peaks.pgm"
output5="pics/Final.pgm"
sigma="1.0"
zertol="0.0001"
percent=".025"


gcc -lm canny.c -o canny
./canny $input $output1 $output2 $output3 $output4 $output5 $sigma $zertol $percent