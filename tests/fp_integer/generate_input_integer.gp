setrand(getwalltime());

x = random % 500;
y = random % 500;
[u, v, d] = gcdext(x, y);

fd = fileopen("input_test/test_gcd_ext.txt", "w");
filewrite(fd, x);
filewrite(fd, y);
filewrite(fd, u);
filewrite(fd, v);
filewrite(fd, d);
fileclose(fd);

quit;