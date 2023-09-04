input="garb34.pgm"
output1="pics/convX.pgm"
output2="pics/convY.pgm"
output3="pics/Mag.pgm"
output4="pics/Peaks.pgm"
sigma="1.0"
zertol="0.0001"

gcc -lm marrh.c -o marrh
./marrh $input $output1 $output2 $output3 $output4 $sigma $zertol