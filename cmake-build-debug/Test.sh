timeout 1 ./base32 -e tests/lorem_ipsum.txt output.txt && cmp output.txt tests/lorem_ipsum.txt.b32 && echo "test1 passed" || echo "test1 failed"
timeout 1 ./base32 -e tests/main.txt output.txt && cmp output.txt tests/mainout.txt && echo "test2 passed" || echo "test2 failed"
timeout 1 ./base32 -e tests/base32.png output && cmp output tests/base32.png.b32 && echo "test2 passed" || echo "test2 failed"
timeout 1 ./base32 -d tests/lorem_ipsum.txt.b32 output && cmp output tests/lorem_ipsum.txt && echo "test3 passed" || echo "test3 failed"
timeout 1 ./base32 -d tests/base32.png.b32 output && cmp output tests/base32.png  && echo "test4 passed" || echo "test4 failed"
timeout 1 ./base32 -d tests/pangram.txt.b32 output.txt && cmp output.txt tests/pangram.txt && echo "test5 passed" || echo "test5 failed"
timeout 1 ./base32 -d tests/rfc4648.txt.b32 output && cmp output tests/rfc4648.txt  && echo "test6 passed" || echo "test6 failed"
timeout 1 ./base32 -d tests/rfc4648.pdf.b32 output && cmp output tests/rfc4648.pdf && echo "test7 passed" || echo "test7 failed"
timeout 1 ./base32 -d tests/session_is_coming.png.b32 output && cmp output tests/session_is_coming.png && echo "test8 passed" || echo "test8 failed"
timeout 1 ./base32 -e tests/session_is_coming.png output.1 && timeout 1 ./base32 -d output.1 output && cmp output tests/session_is_coming.png && echo "test9 passed" || echo "test9 failed"
timeout 1 ./base32 -e tests/rfc4648.pdf output.1 && timeout 1 ./base32 -d output.1 output && cmp output tests/rfc4648.pdf && echo "test10 passed" || echo "test10 failed"


valgrind --tool=memcheck --leak-check=full --undef-value-errors=no --error-exitcode=111 ./base32 -d tests/session_is_coming.png.b32 output
valgrind --tool=memcheck --leak-check=full --undef-value-errors=no --error-exitcode=111 ./base32 -e tests/session_is_coming.png output
clang-tidy ../base32.cpp