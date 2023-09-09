setrand(getwalltime())

c = 2
vp_2 = vector(2048, i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_2 = Mod(Pol(vp_2), c);
vq_2 = vector(2048, i, if (i == 1, (random % (c - 1)) + 1, random % c));
q_2 = Mod(Pol(vq_2), c);
r1_2 = Mod(p_2 + q_2, c);
r2_2 = Mod(p_2 - q_2, c);

c = 255
vp_255 = vector(2048, i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_255 = Mod(Pol(vp_255), c);
vq_255 = vector(2048, i, if (i == 1, (random % (c - 1)) + 1, random % c));
q_255 = Mod(Pol(vq_255), c);
r1_255 = Mod(p_255 + q_255, c);
r2_255 = Mod(p_255 - q_255, c);

fd = fileopen("../tests/fp_poly/input_test/test_add.txt", "w");
filewrite(fd, 2);
filewrite(fd, lift(p_2));
filewrite(fd, lift(q_2));
filewrite(fd, lift(r1_2));
filewrite(fd, 255);
filewrite(fd, lift(p_255));
filewrite(fd, lift(q_255));
filewrite(fd, lift(r1_255));
fileclose(fd);

fd = fileopen("../tests/fp_poly/input_test/test_sub.txt", "w");
filewrite(fd, 2);
filewrite(fd, lift(p_2));
filewrite(fd, lift(q_2));
filewrite(fd, lift(r2_2));
filewrite(fd, 255);
filewrite(fd, lift(p_255));
filewrite(fd, lift(q_255));
filewrite(fd, lift(r2_255));
fileclose(fd);

quit;