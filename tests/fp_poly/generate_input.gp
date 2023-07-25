setrand(getwalltime())

add_sub = if (short_test == 0, 2048, 50);
mul = if (short_test == 0, 512, 7);
div = if (short_test == 0, 1024, 50);
threshold_div = if (short_test == 0, 50, 10);
gcd_val = if (short_test == 0, 1024, 50);
threshold_gcd = if (short_test == 0, 50, 10);
mul_fq = if (short_test == 0, 1024, 50);
threshold_mul_fq = if (short_test == 0, 50, 10);
deg_pol_irred = if (short_test == 0, 51, 3);
gcd_ext = if (short_test == 0, 1024, 50);
threshold_gcd_ext = if (short_test == 0, 50, 10);

polynom_degree = vector(11);
polynom_degree[1] = add_sub;
polynom_degree[2] = mul;
polynom_degree[3] = div;
polynom_degree[4] = threshold_div;
polynom_degree[5] = gcd_val;
polynom_degree[6] = threshold_gcd;
polynom_degree[7] = mul_fq;
polynom_degree[8] = threshold_mul_fq;
polynom_degree[9] = deg_pol_irred;
polynom_degree[10] = gcd_ext;
polynom_degree[11] = threshold_gcd_ext;

c = 2
vp_2 = vector(polynom_degree[1], i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_2 = Mod(Pol(vp_2), c);
vq_2 = vector(polynom_degree[1], i, if (i == 1, (random % (c - 1)) + 1, random % c));
q_2 = Mod(Pol(vq_2), c);
r1_2 = Mod(p_2 + q_2, c);
r2_2 = Mod(p_2 - q_2, c);

c = 255
vp_255 = vector(polynom_degree[1], i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_255 = Mod(Pol(vp_255), c);
vq_255 = vector(polynom_degree[1], i, if (i == 1, (random % (c - 1)) + 1, random % c));
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
vp_2 = vector(polynom_degree[2], i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_2 = Mod(Pol(vp_2), c);
vq_2 = vector(polynom_degree[2], i, if (i == 1, (random % (c - 1)) + 1, random % c));
q_2 = Mod(Pol(vq_2), c);
r3_2 = Mod(p_2 * q_2, c);

c = 15
vp_255 = vector(polynom_degree[2], i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_255 = Mod(Pol(vp_255), c);
vq_255 = vector(polynom_degree[2], i, if (i == 1, (random % (c - 1)) + 1, random % c));
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
vp_2 = vector(polynom_degree[3], i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_2 = Mod(Pol(vp_2), c);
threshold = polynom_degree[4];
vq_2 = vector(polynom_degree[3]/2, i, if (i > threshold, random % c, 0));
q_2 = Mod(Pol(vq_2), c);

c = 11
vp_11 = vector(polynom_degree[3], i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_11 = Mod(Pol(vp_11), c);
threshold = polynom_degree[4];
vq_11 = vector(polynom_degree[3]/2, i, if (i > threshold, random % c, 0));
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
vp_2 = vector(polynom_degree[5], i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_2 = Mod(Pol(vp_2), c);
threshold = polynom_degree[6];
vq_2 = vector(polynom_degree[5]/2, i, if (i > threshold, random % c, 0));
q_2 = Mod(Pol(vq_2), c);
r_2 = gcd(p_2,q_2);

fd = fileopen("input_test/test_gcd.txt", "w");
filewrite(fd, c);
filewrite(fd, lift(p_2));
filewrite(fd, lift(q_2));
filewrite(fd, lift(r_2));
fileclose(fd);

c = 2;
vp_2 = vector(polynom_degree[7], i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_2 = Mod(Pol(vp_2), c);
threshold = polynom_degree[8];
vq_2 = vector(polynom_degree[7]/2, i, if (i > threshold, random % c, 0));
q_2 = Mod(Pol(vq_2), c);
p_irred = ffinit(c, polynom_degree[9], 'x);
r = lift(divrem(Mod(p_2 * q_2, c), p_irred)[2])

fd = fileopen("input_test/test_mul_fq.txt", "w");
filewrite(fd, c);
filewrite(fd, lift(p_2));
filewrite(fd, lift(q_2));
filewrite(fd, lift(p_irred));
filewrite(fd, lift(r);)
fileclose(fd);

c = 2;
vp_2 = vector(polynom_degree[10], i, if (i == 1, (random % (c - 1)) + 1, random % c));
p_2 = Mod(Pol(vp_2), c);
threshold = polynom_degree[11];
vq_2 = vector(polynom_degree[10]/2, i, if (i > threshold, random % c, 0));
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