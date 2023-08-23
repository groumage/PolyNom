fd = fileopen("input_test/random_irred_poly.txt", "r");
while(n = fileread(fd), {if (polisirreducible(eval(n)) == 1,,quit(1))});
fileclose(fd);
quit(0);