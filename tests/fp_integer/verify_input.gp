fd = fileopen("input_test/random_prime.txt", "r");
while(n = fileread(fd), {print(isprime(eval(n))); if (isprime(eval(n)) == 1,,quit(1))});
fileclose(fd);
quit(0);