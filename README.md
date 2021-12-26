# Combination-Circuit-Language

# Example Query 1

```
INPUT 20   IN1 IN2 IN3 IN4 IN5 IN6 IN7 IN8 IN9 IN10 IN11 IN12 IN13 IN14 IN15 IN16 IN17 IN18 IN19 IN20
OUTPUT 1   OUT1

MULTIPLEXER 4   0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1   IN1 IN2 IN3 IN4           temp1
MULTIPLEXER 4   0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1   IN5 IN6 IN7 IN8           temp2
MULTIPLEXER 4   0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1   IN9 IN10 IN11 IN12        temp3
MULTIPLEXER 4   0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1   IN13 IN14 IN15 IN16       temp4
MULTIPLEXER 4   0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1   IN17 IN18 IN19 IN20       temp5
MULTIPLEXER 4   0 1 0 1 0 1 0 1 0 1 0 1 0 1 0 1   temp1 temp2 temp3 temp4   temp6
MULTIPLEXER 2   0 1 0 1                           temp5 temp6               OUT1
```

Query 1 will be read from top to bottom. All Input(s), Output(s), and Circuit Instructions and variables will be read, parsed, and applied to return a truth table. 
For Query 1 particularly, this language will generate a 20 input bit combination, pass it through the Multiplexer circuit instructions, and generate 1 output. Therefore there will be 2^20 possible combinations of the input bits, and thus there will be 2^20 possible types of outputs generated from this particular query. 

# Example Query 2

```
INPUT 5 a b c d e
OUTPUT 2 y z
 AND a b f
 AND c d g
  OR f g h
 XOR h e y
  OR a c i
  OR b d j
NAND i j z
```
Query 2 will be read from top to bottom. All Input(s), Output(s), and Circuit Instructions and variables will be read, parsed, and applied to return a truth table. 
For Query 21 particularly, this language will generate a 5 input bit combination, pass it through the multiple circuit instructions, and generate 2 outputs. Therefore there will be 2^5 possible combinations of the input bits, and thus there will be 2^5 possible types of outputs generated from this particular query.

Output after running through Combination-Circuit-Language

```
0 0 0 0 0 | 0 1
0 0 0 0 1 | 1 1
0 0 0 1 0 | 0 1
0 0 0 1 1 | 1 1
0 0 1 0 0 | 0 1
0 0 1 0 1 | 1 1
0 0 1 1 0 | 1 0
0 0 1 1 1 | 0 0
0 1 0 0 0 | 0 1
0 1 0 0 1 | 1 1
0 1 0 1 0 | 0 1
0 1 0 1 1 | 1 1
0 1 1 0 0 | 0 0
0 1 1 0 1 | 1 0
0 1 1 1 0 | 1 0
0 1 1 1 1 | 0 0
1 0 0 0 0 | 0 1
1 0 0 0 1 | 1 1
1 0 0 1 0 | 0 0
1 0 0 1 1 | 1 0
1 0 1 0 0 | 0 1
1 0 1 0 1 | 1 1
1 0 1 1 0 | 1 0
1 0 1 1 1 | 0 0
1 1 0 0 0 | 1 0
1 1 0 0 1 | 0 0
1 1 0 1 0 | 1 0
1 1 0 1 1 | 0 0
1 1 1 0 0 | 1 0
1 1 1 0 1 | 0 0
1 1 1 1 0 | 1 0
1 1 1 1 1 | 0 0
```

# Supported Circuit Instructions
A parameter n is always an integer and has a special meaning. Input parameters
are indicated as i and output parameters are indicated as o. Ellipses (···) are used to indicate a
variable number of parameters.

•INPUT n i1 ···in
Declares n input variables. This directive must always occur first in a circuit description.
3

•OUTPUT n o1 ···on
Declares n output variables. This directive must always occur second in a circuit description.

•NOT i o
Represents a not gate in logic design. Computes o = i.

•AND i1 i2 o
Represents an and gate in logic design. Computes o = i1i2.

•OR i1 i2 o
Represents an or gate in logic design. Computes o = i1 + i2.

•NAND i1 i2 o
Represents a nand gate in logic design. Computes o = !(i1i2)

•NOR i1 i2 o
Represents a nor gate in logic design. Computes o = !(i1 + i2)

•XOR i1 i2 o
Represents an xor gate in logic design. Computes o = i1 ⊕ i2, where ⊕ indicates exclusive or.

•DECODER n i1 ···in o0 ···o2n−1
Represents an n : 2ndecoder gate in logic design. The first argument gives the number of
inputs, n. The next n parameters are the inputs, followed by 2nparameters indicating the
outputs.
The inputs are interpreted as an n-bit binary number s in the range 0, ···, 2n−1, where i1
is the most significant bit and inis the least significant bit. The output oswill be 1 and all
others will be 0.

•MULTIPLEXER n i0 ···i2n−1 i′1 ···i′n o
Represents a 2n: 1 multiplexer gate in logic design. The inputs to a multiplexer are either
regular inputs or selectors, indicated by i and i′, respectively. The first parameter, n, gives the
number of selectors. The next 2nparameters give the regular inputs, followed by n selector
inputs, and finally the output.
The selector inputs are interpreted as an n-bit binary number s in the range 0, ···, 2n−1.
The output is o = is.

•PASS i o
Represents the absence of a gate. Computes o = i. This may be used to convert a temporary
variable into an output variable.

# Project Credits

All code is written by Rohan Deshpande.
