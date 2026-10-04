clear

if clang thread_test.c tfsm.c -o main; then
  echo "Compilation successful!"
  echo "run test..."
  ./main
else
  echo "failed compilation"
fi
