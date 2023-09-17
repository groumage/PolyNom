## **0.14.0**&emsp;<sub><sup>2023-09-17 ([bb1f50d...db35a8a](https://github.com/groumage/PolynomArithmetic/compare/bb1f50d9d92d017dbd6f7c49d91e5864f1e126a0...db35a8a4c364ca5699962253ecf3bc479681e304?diff=split))</sup></sub>

### Features

- add function to count the number of bits set ([848fbc1](https://github.com/groumage/PolynomArithmetic/commit/848fbc1185b5249b90accaed56f131364606d306))
- catch errors of poly lib ([4902cc4](https://github.com/groumage/PolynomArithmetic/commit/4902cc49e882db5c4e60366e33dd77d9546e817c))
- generate random irreducible polynom in any field ([269e1a0](https://github.com/groumage/PolynomArithmetic/commit/269e1a0e00bbd6065a65eecf25cbc8a768984655))
- catch errors of list library ([78fe4b3](https://github.com/groumage/PolynomArithmetic/commit/78fe4b3c571c95fe332f64685b5cb87124d92c4e))
- improve meaning of list library errors ([5c805b6](https://github.com/groumage/PolynomArithmetic/commit/5c805b6b2f081849bdae512037fccd83e179efe0))

### Bug Fixes

- normalization of the representation of the zero polynom ([fea1780](https://github.com/groumage/PolynomArithmetic/commit/fea178032554c7f3f724be0934e5b6abd37f881c))
- repair parsing of the zero polynom ([7947bcf](https://github.com/groumage/PolynomArithmetic/commit/7947bcf7cb3e7a9a79739c0adff5bcb86bd05c9e))
- computation of the degree of a polynom ([35de03e](https://github.com/groumage/PolynomArithmetic/commit/35de03e9ab28279137f28ebae7bc9a7b2385fe83))
- repair "the coefficient that correspond to a degree" function ([25bd136](https://github.com/groumage/PolynomArithmetic/commit/25bd136ab65066849cd3ba4abdfba418c90e0d6a))
- repair irreducibility checking ([0ffa552](https://github.com/groumage/PolynomArithmetic/commit/0ffa552c4e1eeb4069153c01fa337e5c91d1e828))
- repair random generation of a polynom ([c8d180d](https://github.com/groumage/PolynomArithmetic/commit/c8d180dc1378e317e45f58f17095bfd55205f803))
- repair polynom initialization where all coefficients are zero ([a37eb11](https://github.com/groumage/PolynomArithmetic/commit/a37eb113cd4b4cf6c7cf1197e9836f292dcf13f2))

### Merges

- branch 'code\_coverage' into main ([b8a886c](https://github.com/groumage/PolynomArithmetic/commit/b8a886c716f3ec0813f2d6a4f7a6a9252aeed41e))

### Tests

- add unit tests for corner cases of polynom addition ([bb1f50d](https://github.com/groumage/PolynomArithmetic/commit/bb1f50d9d92d017dbd6f7c49d91e5864f1e126a0))
- add coefficient overflow error ([275e42e](https://github.com/groumage/PolynomArithmetic/commit/275e42ebfd0505ccf073e230adedeb7e08ebc289))
- add unit tests for corner cases of polynom substraction ([d94df4d](https://github.com/groumage/PolynomArithmetic/commit/d94df4dcbd68336447b037505ac75341a22199d9))
- add unit tests for polynom substraction ([b4dacbc](https://github.com/groumage/PolynomArithmetic/commit/b4dacbcd8b84c3b893952969ee909eabef50fb76))
- add unit tests for corner cases of polynom degree function ([fdb944c](https://github.com/groumage/PolynomArithmetic/commit/fdb944cc90e1ccea854b521482f2bd395345d5b7))
- add unit tests for parsing of zero polynom ([05e71ab](https://github.com/groumage/PolynomArithmetic/commit/05e71ab79d01184296c95ee8823095d782278196))
- change name of errors ([ee7ed3d](https://github.com/groumage/PolynomArithmetic/commit/ee7ed3d9e1d6a66894ce6c2895541b4043e9c46e))
- add unit tests for function assert, init, free and print ([194a89e](https://github.com/groumage/PolynomArithmetic/commit/194a89e96a313dc3f43ff9071f4a33ee6373bb43))
- add label for unit test of list ([738744f](https://github.com/groumage/PolynomArithmetic/commit/738744f89edbc5a8856629374f38294291f75356))
- add labels for unit test of integers ([ca0d9ec](https://github.com/groumage/PolynomArithmetic/commit/ca0d9eca87bda4e3916fda69ff4b1fb1edc4ef47))
- add label for unit tests ([e4db5c7](https://github.com/groumage/PolynomArithmetic/commit/e4db5c7ca178bd4c43dc770c11941936c6134e62))
- add script to make code coverage and easily run unit tests ([46df9d1](https://github.com/groumage/PolynomArithmetic/commit/46df9d1175dc15f9721ac5327add88200067b105))
- add unit tests of random irreducible polynom ([4a0a273](https://github.com/groumage/PolynomArithmetic/commit/4a0a2738ed14fb281c53fa1e975ccd86acfa3a65))
- update return value of list unit tests ([7a97a58](https://github.com/groumage/PolynomArithmetic/commit/7a97a58153045b81ec58de3eca13b1aa0a15de89))

<br>

## **0.13.0**&emsp;<sub><sup>2023-08-23 ([17b421f...b70226e](https://github.com/groumage/PolynomArithmetic/compare/17b421f0d728456c4103531a12ccab3d2b4649c6...b70226eaa175649736cbca5933183461d5b9cf48?diff=split))</sup></sub>

### Features

- add client frontend to use the integer library ([af2c95b](https://github.com/groumage/PolynomArithmetic/commit/af2c95b658c1f299f45400559491a70d7b6912d6))
- add primality test for mpz/int \+ generation of random mpz prime ([29b180f](https://github.com/groumage/PolynomArithmetic/commit/29b180f879c34c4d6752a955ea2da19d7e7ae882))
- add generation of irreducible polynoms and update print function ([5e7872e](https://github.com/groumage/PolynomArithmetic/commit/5e7872ed0539fdecafb9e8537396869cba9acae4))

### Bug Fixes

- repair integer primality test ([b0e14f6](https://github.com/groumage/PolynomArithmetic/commit/b0e14f646890375bc4bf3e36bfa52b2ae6db7302))

### Tests

- add unit tests of the client frontend ([97626d3](https://github.com/groumage/PolynomArithmetic/commit/97626d31fbba281be25d3a9c22b2197153ad187e))
- add tests labels and update script to run unit tests ([561731f](https://github.com/groumage/PolynomArithmetic/commit/561731f9dbf7620c3928f3f382e990b1e1968f8d))
- add unit tests for random prime ([644bcee](https://github.com/groumage/PolynomArithmetic/commit/644bceed70f96334a7dea7556e6c48e59ce1e601))
- add unit tests to check irreducibility of random polynom ([ab8e89e](https://github.com/groumage/PolynomArithmetic/commit/ab8e89ec994916341cb9947ee1c76433e2e2da89))
- add unit tests for checking irreducibility of random polynom ([f68b984](https://github.com/groumage/PolynomArithmetic/commit/f68b984ec833f62b857f5c5ed5cfc6fd19d1d85d))

<br>

## **0.12.0**&emsp;<sub><sup>2023-08-06 ([17b421f...940d20a](https://github.com/groumage/PolynomArithmetic/compare/17b421f0d728456c4103531a12ccab3d2b4649c6...940d20abe04dfc3e46e45afb860bb14e5f816f65?diff=split))</sup></sub>

### Features

- add a library to encrypt / decrypt messages with RSA cryptosystem ([8621b21](https://github.com/groumage/PolynomArithmetic/commit/8621b21689066d701244dbd812e2a7a21cfe94e8))

### Tests

- remove valgrind option leak\-check=full for unit testing ([7266931](https://github.com/groumage/PolynomArithmetic/commit/726693134d53b8d0acd60ca11a56036a5fa0bfc3))

<br>

## **0.11.0**&emsp;<sub><sup>2023-08-01 ([5dc074c...f82efa0](https://github.com/groumage/PolynomArithmetic/compare/5dc074cb7517494a5477b2cb5016a611825ee917...f82efa0071050f0dbb6675c693e5006150e1391a?diff=split))</sup></sub>

### Features

- add extended euclide algorithm for integers ([9edb7e1](https://github.com/groumage/PolynomArithmetic/commit/9edb7e1826841951b248e5859432d8fe8f6640a2))

### Tests

- add unit test for extended euclide algorithm for integers ([6a6269e](https://github.com/groumage/PolynomArithmetic/commit/6a6269e3fb884d594a678d5a8bd9f5c12462e50c))

<br>

## **0.10.0**&emsp;<sub><sup>2023-07-30 ([f3cbcc0...b7e0ae0](https://github.com/groumage/PolynomArithmetic/compare/f3cbcc09b58c6bc0d4a8693cc817b0e527c73848...b7e0ae0b66b5eceff5ac5fc9ce9f4f76cac17293?diff=split))</sup></sub>

### Features

- add generation of random prime integer ([d1ce9e9](https://github.com/groumage/PolynomArithmetic/commit/d1ce9e9fc6f8cea8bdfbf9098591778902c33053))

### Bug Fixes

- correct the parsing of the null polynom \(i\.e\., p\(x\) = 0\) ([165a7a4](https://github.com/groumage/PolynomArithmetic/commit/165a7a41042786546c6d1d8d6846026cf68f41f0))

### Tests

- create test suites ([8161b8b](https://github.com/groumage/PolynomArithmetic/commit/8161b8b3c5410782041cfd1f43af85c0cce093a4))
- enhance generation of input tests ([9bb1391](https://github.com/groumage/PolynomArithmetic/commit/9bb13916b131b5b5b71268e275dae52ea1b37eec))
- add unit test for random integer prime generation ([1dcd707](https://github.com/groumage/PolynomArithmetic/commit/1dcd70789a56c948b80c3d2f397a8180e8c7a852))

<br>

## **0.9.0**&emsp;<sub><sup>2023-07-04 ([e1c9505...459c067](https://github.com/groumage/PolynomArithmetic/compare/e1c9505a7471dfe595b4e932a2390356d4c59886...459c067cef19b2382a7604cf075e38f428ea5abe?diff=split))</sup></sub>

### Features

- add polynom extended GCD ([e1c9505](https://github.com/groumage/PolynomArithmetic/commit/e1c9505a7471dfe595b4e932a2390356d4c59886))

### Tests

- add polynom extended GCD unit tests ([148fd47](https://github.com/groumage/PolynomArithmetic/commit/148fd4732508c0c8a74df28ac1126956c7155d3b))

<br>

## **0.8.0**&emsp;<sub><sup>2023-07-01 ([b7d5a9b...dc6422f](https://github.com/groumage/PolynomArithmetic/compare/b7d5a9baa67e2e301e72dd4c4731793e856265b7...dc6422fce4e0c9974d1de5613d375c2c2711cb9c?diff=split))</sup></sub>

### Features

- add polynom multiplication over galois fields ([b7d5a9b](https://github.com/groumage/PolynomArithmetic/commit/b7d5a9baa67e2e301e72dd4c4731793e856265b7))

### Tests

- add unit tests of polynom multiplication over finite fields ([4ea693d](https://github.com/groumage/PolynomArithmetic/commit/4ea693db3a91908ccacab8fef62be321d56f4170))

<br>

## **0.7.0**&emsp;<sub><sup>2023-06-28 ([d186b8a...3478bdd](https://github.com/groumage/PolynomArithmetic/compare/d186b8aa5be055f29a57041cf85e97a8747ad80a...3478bdd6845022505c1032f0d5489cf5d7a3a7e4?diff=split))</sup></sub>

### Features

- add polynom GCD ([220e701](https://github.com/groumage/PolynomArithmetic/commit/220e701d96d5c3cf5a31f00f608a8aba04014dff))

### Bug Fixes

- repair polynom division ([d186b8a](https://github.com/groumage/PolynomArithmetic/commit/d186b8aa5be055f29a57041cf85e97a8747ad80a))
- repair function that compute the degree of a polynom ([054a29c](https://github.com/groumage/PolynomArithmetic/commit/054a29c6960ac0ab68cadee579e1b77cb7b39eba))

### Tests

- enhance multiplication and division unit tests ([0e2e541](https://github.com/groumage/PolynomArithmetic/commit/0e2e5416b260179f3d19ff9a365b1138d9976dbc))
- update CMakeList with test labels ([2bc8d63](https://github.com/groumage/PolynomArithmetic/commit/2bc8d638269f824b8b5d76d44cac696b3ebd40bd))
- add polynom GCD unit tests ([a937f0a](https://github.com/groumage/PolynomArithmetic/commit/a937f0a99479151a2be47724a608b2b6278ba8dc))
- generate input test for polynom GCD unit tests ([971b3f7](https://github.com/groumage/PolynomArithmetic/commit/971b3f711552602e5485c748cfdb2db535bd99d8))

<br>

## **0.6.0**&emsp;<sub><sup>2023-06-22 ([4a1289b...ad23c6f](https://github.com/groumage/PolynomArithmetic/compare/4a1289b9e52941f946a00f156eb76603701e346d...ad23c6f9d04e5a004f9f1611fba5a2b7850b9a04?diff=split))</sup></sub>

### Features

- add function to copy list ([4a1289b](https://github.com/groumage/PolynomArithmetic/commit/4a1289b9e52941f946a00f156eb76603701e346d))
- add polynom division ([2af5e15](https://github.com/groumage/PolynomArithmetic/commit/2af5e15351b8d3a5e0dbc40a55fb76020f59f6d1))

### Tests

- add polynom division unit tests ([e919a9b](https://github.com/groumage/PolynomArithmetic/commit/e919a9be1008066f0147baa2c6d8987166b87d96))

<br>

## **0.5.0**&emsp;<sub><sup>2023-06-21 ([2872cce...33dbece](https://github.com/groumage/PolynomArithmetic/compare/2872ccefa63b2f4be9353579205b620ef724f600...33dbeceae5af5aa7c1e615e46707a6d1bbee7e05?diff=split))</sup></sub>

### Features

- add polynom multiplication ([fd2b14b](https://github.com/groumage/PolynomArithmetic/commit/fd2b14b23612bc1d84719cb6780c066cf34202cc))

### Tests

- add unit tests for polynom multiplication ([aa0d81b](https://github.com/groumage/PolynomArithmetic/commit/aa0d81b0597d157a88b674ba9c39a55eb736e5d8))
- update CMakeLists for mulitplication unit tests ([c638466](https://github.com/groumage/PolynomArithmetic/commit/c638466be35869457b7f5bc835bc7a3ae3aad26e))
- enhance polynom multiplication unit tests ([368b686](https://github.com/groumage/PolynomArithmetic/commit/368b68639b15b7f1e331c7714a44f464251da3fe))

<br>

## **0.4.0**&emsp;<sub><sup>2023-06-12 ([9785424...162d06d](https://github.com/groumage/PolynomArithmetic/compare/9785424813a91cbf0c0c7ada5dd5008016a90570...162d06dc1c8e0da7f5decb96f4abbda6234790d0?diff=split))</sup></sub>

### Features

- add an error for poly library ([420b19b](https://github.com/groumage/PolynomArithmetic/commit/420b19b2dff3888a80b8dda1ee65a619febfd834))
- add polynom substraction ([1c61196](https://github.com/groumage/PolynomArithmetic/commit/1c61196c66e64ec21565a63b58ef09657ef3d62c))

### Tests

- update CMakeList files \+ refactor code architecture of unit tests ([9785424](https://github.com/groumage/PolynomArithmetic/commit/9785424813a91cbf0c0c7ada5dd5008016a90570))
- add unit tests for polynom parsing ([f77c677](https://github.com/groumage/PolynomArithmetic/commit/f77c67710b65b105c409653197f3256777c72205))
- add unit test for polynom substraction ([1e6b4b7](https://github.com/groumage/PolynomArithmetic/commit/1e6b4b72780ab0e089dc12dc2869d02a9f2a38bf))

<br>

## **0.3.0**&emsp;<sub><sup>2023-06-12 ([9337c40...fbe3f6b](https://github.com/groumage/PolynomArithmetic/compare/9337c4087999aeeeb1d0919daf5d02250c846ace...fbe3f6bb81250f7ee6fbd53e463f3fa8fd3250d4?diff=split))</sup></sub>

### Features

- upload list header and src files ([8a0beb2](https://github.com/groumage/PolynomArithmetic/commit/8a0beb2f4e16a5879586376ec56964143e45fa67))
- add list manipulation functions \(add\_after, get\_pos, get\_at\_pos\) ([1016296](https://github.com/groumage/PolynomArithmetic/commit/1016296bf2add41044b14b4cca3bfa2230a0bb16))
- add header of polynom library ([7172ba8](https://github.com/groumage/PolynomArithmetic/commit/7172ba85ebb555c459abf1c8924d9ada28b7f42d))
- add poly library ([d03df7e](https://github.com/groumage/PolynomArithmetic/commit/d03df7e6bcbcd668a39eb2e2ab9fbd851b52ab0d))

### Tests

- add list unit tests ([f58ad57](https://github.com/groumage/PolynomArithmetic/commit/f58ad5784bb95c49eb3b72d600f3f51f775b19d2))
- add mock malloc function for list library and unit tests ([0563d17](https://github.com/groumage/PolynomArithmetic/commit/0563d17a6cb9212f7c497e54ed8eebc5ccaf964b))
- more exhaustive unit tests for list library ([011eaef](https://github.com/groumage/PolynomArithmetic/commit/011eaef8e8bebb4f3f8cd4c819f75cf61107e0f7))
- add unit tests with mock malloc function for list library ([ac8c133](https://github.com/groumage/PolynomArithmetic/commit/ac8c13352578d725089f63f165349e903af74ca7))
- add unit test execution for list and poly libraries ([a7c3f5a](https://github.com/groumage/PolynomArithmetic/commit/a7c3f5af81cc072220b86cdba6b0051163b31dbe))
- add code coverage option for unit tests ([a687357](https://github.com/groumage/PolynomArithmetic/commit/a68735744635f5fb1141d45cd17e5aa93af10d9a))
- add unit test for polynom addition ([cce9e40](https://github.com/groumage/PolynomArithmetic/commit/cce9e40372b149b99f6a01e7ef7fe9f316246c08))
- add PariGP code to generate inputs for unit tests ([8052abd](https://github.com/groumage/PolynomArithmetic/commit/8052abd1a7a0a275cf69443b97c6c30d61b43603))
- add unit test for polynom parsing ([990669e](https://github.com/groumage/PolynomArithmetic/commit/990669eada2b4bc5e61f67d572f015fd39a9cef1))
- add unit tests for util functions ([faf9dd8](https://github.com/groumage/PolynomArithmetic/commit/faf9dd876e9c7d15e1c3f2a3d70941daf7b668a4))

<br>

## **0.2.0**&emsp;<sub><sup>2023-06-12 ([b775df2...15a31ce](https://github.com/groumage/PolynomArithmetic/compare/b775df27959f63b98e827adbc1df8ab19ad3ff03...15a31cebaea3c350f54448dbaf1d33504741aec3?diff=split))</sup></sub>

### Features

- upload list header and src files ([8a0beb2](https://github.com/groumage/PolynomArithmetic/commit/8a0beb2f4e16a5879586376ec56964143e45fa67))

### Tests

- add list unit tests ([f58ad57](https://github.com/groumage/PolynomArithmetic/commit/f58ad5784bb95c49eb3b72d600f3f51f775b19d2))

<br>

