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
vp_255 = vector(512, i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_255 = Mod(Pol(vp_255), c);
vq_255 = vector(512, i, if (i == 1, (random % (c - 1)) + 1, random % c));
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
vp_2 = vector(2048, i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_2 = Mod(Pol(vp_2), c);
threshold = random(50) + 50;
vq_2 = vector(1024, i, if (i > threshold, random % c, 0));
q_2 = Mod(Pol(vq_2), c);

c = 11
vp_11 = vector(1024, i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_11 = Mod(Pol(vp_11), c);
threshold = random(50) + 50;
vq_11 = vector(512, i, if (i > threshold, random % c, 0));
q_11 = Mod(Pol(vq_11), c);

fd = fileopen("input_test/test_div.txt", "w");
filewrite(fd, 2);
filewrite(fd, lift(p_2));
filewrite(fd, lift(q_2));
t_2 = divrem(p_2,q_2);
filewrite(fd, lift(t_2[1]));
filewrite(fd, lift(t_2[2]));
filewrite(fd, 11);
filewrite(fd, lift(p_11));
filewrite(fd, lift(q_11));
t_11 = divrem(p_11,q_11);
filewrite(fd, lift(t_11[1]));
filewrite(fd, lift(t_11[2]));
fileclose(fd);

c = 2;
vp_2 = vector(200, i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_2 = Mod(Pol(vp_2), c);
threshold = 1;
vq_2 = vector(100, i, if (i > threshold, random % c, 0));
q_2 = Mod(Pol(vq_2), c);
r_2 = gcd(p_2,q_2);

fd = fileopen("input_test/test_gcd.txt", "w");
filewrite(fd, c);
filewrite(fd, lift(p_2));
filewrite(fd, lift(q_2));
filewrite(fd, lift(r_2));
fileclose(fd);

c = 2;
vp_2 = vector(200, i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_2 = Mod(Pol(vp_2), c);
threshold = 1;
vq_2 = vector(100, i, if (i > threshold, random % c, 0));
q_2 = Mod(Pol(vq_2), c);
p_irred = ffinit(c, 21, 'x);
r = lift(divrem(Mod(p_2 * q_2, c), p_irred)[2])

fd = fileopen("input_test/test_mul_fq.txt", "w");
filewrite(fd, c);
filewrite(fd, lift(p_2));
filewrite(fd, lift(q_2));
filewrite(fd, lift(p_irred));
filewrite(fd, lift(r);)
fileclose(fd);

c = 2;
vp_2 = vector(10, i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_2 = Mod(Pol(vp_2), c);
threshold = 2;
vq_2 = vector(8, i, if (i > threshold, random % c, 0));
q_2 = Mod(Pol(vq_2), c);
u = gcdext(p_2,q_2)[1]
v = gcdext(p_2,q_2)[2]
d = gcdext(p_2,q_2)[3]

fd = fileopen("input_test/test_gcd_ext.txt", "w");
filewrite(fd, c);
filewrite(fd, lift(p_2));
filewrite(fd, lift(q_2));
filewrite(fd, lift(u));
filewrite(fd, lift(v));
filewrite(fd, lift(d));
fileclose(fd);

quit;

\\fd = fileopen("input_test/test_ext_gcd.txt", "w");
\\filewrite(fd, c);
\\    filewrite(fd, c);
\\    
\\    filewrite(fd, lift(P));
\\    filewrite(fd, lift(lift(Mod(lift(gcdext(p,q)[1]), P))));
\\    filewrite(fd, lift(lift(Mod(lift(gcdext(p,q)[2]), P))));
\\    filewrite(fd, lift(lift(Mod(lift(gcdext(p,q)[3]), P))));
\\};