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

fd = fileopen("input_test/test_add.txt", "w");
filewrite(fd, 2);
filewrite(fd, lift(p_2));
filewrite(fd, lift(q_2));
filewrite(fd, lift(r1_2));
filewrite(fd, 255);
filewrite(fd, lift(p_255));
filewrite(fd, lift(q_255));
filewrite(fd, lift(r1_255));
fileclose(fd);

fd = fileopen("input_test/test_sub.txt", "w");
filewrite(fd, 2);
filewrite(fd, lift(p_2));
filewrite(fd, lift(q_2));
filewrite(fd, lift(r2_2));
filewrite(fd, 255);
filewrite(fd, lift(p_255));
filewrite(fd, lift(q_255));
filewrite(fd, lift(r2_255));
fileclose(fd);

c = 2
vp_2 = vector(512, i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_2 = Mod(Pol(vp_2), c);
vq_2 = vector(512, i, if (i == 1, (random % (c - 1)) + 1, random % c));
q_2 = Mod(Pol(vq_2), c);
r3_2 = Mod(p_2 * q_2, c);

c = 15
vp_255 = vector(10, i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_255 = Mod(Pol(vp_255), c);
vq_255 = vector(10, i, if (i == 1, (random % (c - 1)) + 1, random % c));
q_255 = Mod(Pol(vq_255), c);
r3_255 = Mod(p_255 * q_255, c);

fd = fileopen("input_test/test_mul.txt", "w");
filewrite(fd, 2);
filewrite(fd, lift(p_2));
filewrite(fd, lift(q_2));
filewrite(fd, lift(r3_2));
filewrite(fd, 15);
filewrite(fd, lift(p_255));
filewrite(fd, lift(q_255));
filewrite(fd, lift(r3_255));
fileclose(fd);

c = 2

fd = fileopen("input_test/test_div.txt", "w");
filewrite(fd, c);
v_p = vector(512, i, if (i == 1, (random % (c - 1)) + 1, random % c));
p = Mod(Pol(v_p), c);
filewrite(fd, lift(p));
threshold = random(50) + 50;
v_q = vector(256, i, if (i > threshold, random % c, 0));
q = Mod(Pol(v_q), c);
filewrite(fd, lift(q));
t = divrem(p,q);
filewrite(fd, lift(t[1]));
filewrite(fd, lift(t[2]));
fileclose(fd);

quit;