echo "compiling survey_dml_core"
gcc *.c -I /mnt/d/Program_Files/HLL_MATLAB/R2019b/extern/include -lm -o test.o
echo "testing   survey_dml_core:"
time ./test.o
